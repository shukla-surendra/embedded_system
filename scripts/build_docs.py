#!/usr/bin/env python3
"""
Pure-Python Markdown → HTML documentation builder.
No dependencies beyond the Python standard library.

Converts every .md file in the project to a styled HTML page with:
  - Collapsible sidebar navigation showing the full project tree
  - Breadcrumb trail per page
  - Syntax-highlighted code blocks (CSS-only)
  - Tables, blockquotes, lists, inline formatting
  - .md links automatically rewritten to .html

Usage (called by Makefile):
    python3 scripts/build_docs.py          # build to site/
    python3 scripts/build_docs.py --serve  # build + serve on :8080
"""

import argparse
import http.server
import os
import re
import html as _html
import shutil
import socketserver
import threading
from collections import defaultdict
from datetime import datetime
from pathlib import Path

# ── Config ────────────────────────────────────────────────────────────────────

ROOT      = Path(__file__).resolve().parent.parent
SITE      = ROOT / "site"
SKIP_DIRS = {'.git', 'site', 'scripts', '__pycache__', '.claude', 'node_modules', '.venv'}
PORT      = 8080
BUILD_TS  = datetime.now().strftime('%Y-%m-%d %H:%M')

# ── Name helpers ──────────────────────────────────────────────────────────────

def fmt_dir(name: str) -> str:
    """'02_microcontrollers' → '02 · Microcontrollers'"""
    m = re.match(r'^(\d+)[_-](.+)$', name)
    if m:
        return f"{m.group(1)} · {m.group(2).replace('_', ' ').title()}"
    return name.replace('_', ' ').title()


def fmt_file(stem: str) -> str:
    """'05_assembly_programming_guide' → '05. Assembly Programming Guide'"""
    if stem.lower() in ('readme', 'index'):
        return 'Overview'
    m = re.match(r'^(\d+)[_-](.+)$', stem)
    if m:
        return f"{m.group(1)}. {m.group(2).replace('_', ' ').title()}"
    return stem.replace('_', ' ').title()

# ── Markdown → HTML ───────────────────────────────────────────────────────────

_PLACEHOLDER = '\x00\x01{}\x01\x00'
_PH_RE       = re.compile(r'\x00\x01(\d+)\x01\x00')

_BLOCK_RE = re.compile(
    r'^(#{1,6}\s|`{3,}|~{3,}|[-*_](\s*[-*_]){2,}\s*$|[-*+]\s|\d+[.)]\s|>\s?|\|)'
)


def _is_block_start(line: str) -> bool:
    return bool(_BLOCK_RE.match(line))


def inline(text: str) -> str:
    """Convert inline Markdown to HTML, protecting code spans from other rules."""
    spans: list[str] = []

    def save(m: re.Match) -> str:
        inner = (m.group(1) or m.group(2) or '').strip()
        spans.append(f'<code class="inline-code">{_html.escape(inner, quote=False)}</code>')
        return _PLACEHOLDER.format(len(spans) - 1)

    text = re.sub(r'``(.+?)``|`([^`\n]+?)`', save, text, flags=re.DOTALL)
    text = _html.escape(text, quote=False)

    text = re.sub(r'\*\*\*(.+?)\*\*\*', r'<strong><em>\1</em></strong>', text)
    text = re.sub(r'\*\*(.+?)\*\*',     r'<strong>\1</strong>', text)
    text = re.sub(r'__(.+?)__',          r'<strong>\1</strong>', text)
    text = re.sub(r'\*([^*\n]+?)\*',     r'<em>\1</em>', text)
    text = re.sub(r'_([^_\n]+?)_',       r'<em>\1</em>', text)
    text = re.sub(r'~~(.+?)~~',          r'<del>\1</del>', text)

    text = re.sub(
        r'!\[([^\]]*)\]\(([^)]+)\)',
        lambda m: f'<img alt="{m.group(1)}" src="{m.group(2)}" loading="lazy">',
        text,
    )

    def link_sub(m: re.Match) -> str:
        label = m.group(1)
        href  = m.group(2)
        if not href.startswith(('http://', 'https://', '#', 'mailto:')):
            if href.endswith('.md'):
                href = href[:-3] + '.html'
        return f'<a href="{href}">{label}</a>'

    text = re.sub(r'\[([^\]]+)\]\(([^)]+)\)', link_sub, text)
    text = _PH_RE.sub(lambda m: spans[int(m.group(1))], text)
    return text


def md_to_html(text: str) -> str:
    """Convert a full Markdown document to an HTML fragment."""
    # Strip YAML frontmatter if present
    text = re.sub(r'\A---\s*\n.*?\n---\s*\n', '', text, flags=re.DOTALL)
    lines = text.expandtabs(4).split('\n')
    out: list[str] = []
    i, n = 0, len(lines)

    while i < n:
        raw = lines[i]
        line = raw.rstrip()

        # ── Fenced code block ──────────────────────────────────────────────
        fm = re.match(r'^(`{3,}|~{3,})\s*(\S*)', line)
        if fm:
            fence = fm.group(1)[:3]
            lang  = fm.group(2).lower()
            code_lines: list[str] = []
            i += 1
            while i < n and not lines[i].startswith(fence):
                code_lines.append(lines[i])
                i += 1
            code  = _html.escape('\n'.join(code_lines), quote=False)
            cls   = f' class="language-{lang}"' if lang else ''
            out.append(f'<pre><code{cls}>{code}</code></pre>')
            i += 1
            continue

        # ── ATX heading ───────────────────────────────────────────────────
        hm = re.match(r'^(#{1,6})\s+(.*?)(?:\s+#+\s*)?$', line)
        if hm:
            lvl  = len(hm.group(1))
            txt  = hm.group(2)
            slug = re.sub(r'[^\w\s-]', '', txt).strip().lower()
            slug = re.sub(r'\s+', '-', slug)
            out.append(f'<h{lvl} id="{slug}">{inline(txt)}</h{lvl}>')
            i += 1
            continue

        # ── Horizontal rule ───────────────────────────────────────────────
        if re.match(r'^(?:[-*_]\s*){3,}$', line.strip()) and line.strip():
            out.append('<hr>')
            i += 1
            continue

        # ── Blockquote ────────────────────────────────────────────────────
        if re.match(r'^>\s?', line):
            bq: list[str] = []
            while i < n and (re.match(r'^>\s?', lines[i]) or
                              (bq and not lines[i].strip())):
                bq.append(re.sub(r'^>\s?', '', lines[i]))
                i += 1
            out.append(f'<blockquote>{md_to_html(chr(10).join(bq))}</blockquote>')
            continue

        # ── Table ─────────────────────────────────────────────────────────
        next_line = lines[i + 1].rstrip() if i + 1 < n else ''
        if '|' in line and re.match(r'^\|?[\s|:-]+\|', next_line):
            rows: list[str] = []
            while i < n and '|' in lines[i]:
                rows.append(lines[i].rstrip())
                i += 1

            def cells(row: str) -> list[str]:
                return [c.strip() for c in row.strip('|').split('|')]

            if len(rows) >= 2:
                head = ''.join(f'<th>{inline(c)}</th>' for c in cells(rows[0]))
                body = ''.join(
                    '<tr>' + ''.join(f'<td>{inline(c)}</td>' for c in cells(r)) + '</tr>'
                    for r in rows[2:]
                )
                out.append(
                    f'<div class="table-wrap">'
                    f'<table><thead><tr>{head}</tr></thead><tbody>{body}</tbody></table>'
                    f'</div>'
                )
            continue

        # ── Unordered list ────────────────────────────────────────────────
        if re.match(r'^[-*+]\s', line):
            items: list[str] = []
            while i < n and re.match(r'^[-*+]\s', lines[i]):
                items.append(f'<li>{inline(lines[i][2:].strip())}</li>')
                i += 1
            out.append('<ul>' + ''.join(items) + '</ul>')
            continue

        # ── Ordered list ──────────────────────────────────────────────────
        if re.match(r'^\d+[.)]\s', line):
            items = []
            while i < n and re.match(r'^\d+[.)]\s', lines[i]):
                content = re.sub(r'^\d+[.)]\s+', '', lines[i])
                items.append(f'<li>{inline(content)}</li>')
                i += 1
            out.append('<ol>' + ''.join(items) + '</ol>')
            continue

        # ── Blank line ────────────────────────────────────────────────────
        if not line.strip():
            i += 1
            continue

        # ── Paragraph ─────────────────────────────────────────────────────
        para: list[str] = []
        i0 = i
        while i < n:
            l = lines[i].rstrip()
            if not l.strip():
                break
            if _is_block_start(l):
                break
            para.append(l)
            i += 1
        if para:
            out.append(f'<p>{inline(" ".join(para))}</p>')
        elif i == i0:
            i += 1  # safety: no rule matched, skip line to prevent infinite loop

    return '\n'.join(out)

# ── File collection ───────────────────────────────────────────────────────────

def collect() -> list[tuple[Path, Path]]:
    """Returns sorted list of (src_md, dest_html) pairs."""
    pairs: list[tuple[Path, Path]] = []
    for dirpath, dirnames, filenames in os.walk(ROOT):
        dirnames[:] = sorted(d for d in dirnames if d not in SKIP_DIRS)
        for fname in sorted(filenames):
            if fname.endswith('.md'):
                src  = Path(dirpath) / fname
                rel  = src.relative_to(ROOT)
                dest = SITE / rel.with_suffix('.html')
                pairs.append((src, dest))
    return pairs

# ── Navigation ────────────────────────────────────────────────────────────────

def build_nav(pairs: list[tuple[Path, Path]], current: Path | None = None) -> str:
    """Build sidebar HTML. Uses <details> for collapsible sections."""
    groups: dict[str, list[tuple[Path, Path]]] = defaultdict(list)
    root_items: list[tuple[Path, Path]] = []

    for src, dest in pairs:
        rel   = src.relative_to(ROOT)
        parts = rel.parts
        if len(parts) == 1:
            root_items.append((src, dest))
        else:
            groups[parts[0]].append((src, dest))

    h: list[str] = ['<nav id="sidenav">']

    # Root-level files (e.g. README.md → index.html)
    for src, dest in root_items:
        if src.stem.lower() == 'readme':
            continue
        href     = '/' + dest.relative_to(SITE).as_posix()
        is_cur   = ' active' if dest == current else ''
        h.append(f'<a class="nav-link{is_cur}" href="{href}">{fmt_file(src.stem)}</a>')

    # One <details> block per subdirectory
    for top in sorted(groups):
        items   = groups[top]
        is_open = any(dest == current for _, dest in items)
        h.append(f'<details{"  open" if is_open else ""}>')
        h.append(f'<summary>{fmt_dir(top)}</summary>')
        h.append('<div class="nav-items">')
        for src, dest in items:
            href   = '/' + dest.relative_to(SITE).as_posix()
            is_cur = ' active' if dest == current else ''
            label  = fmt_file(src.stem)
            # Indent nested files (subdirectory inside a section)
            depth  = len(src.relative_to(ROOT).parts) - 2
            pad    = ' nav-deep' if depth > 0 else ''
            h.append(f'<a class="nav-link{is_cur}{pad}" href="{href}">{label}</a>')
        h.append('</div></details>')

    h.append('</nav>')
    return '\n'.join(h)

# ── Breadcrumbs ───────────────────────────────────────────────────────────────

def make_breadcrumbs(src: Path) -> list[tuple[str, str]]:
    """Returns [(label, href), ...] chain from project root to current page."""
    rel   = src.relative_to(ROOT)
    parts = rel.parts
    crumbs: list[tuple[str, str]] = [('Home', '/index.html')]

    if len(parts) > 1:
        section = parts[0]
        # link to the section README if it exists
        readme = SITE / section / 'README.html'
        href   = '/' + (SITE / section / 'README.html').relative_to(SITE).as_posix()
        crumbs.append((fmt_dir(section), href))

    if len(parts) > 2:
        # nested subdirectory
        sub    = parts[-2]
        href   = '/' + (SITE / parts[0] / sub / 'README.html').relative_to(SITE).as_posix()
        crumbs.append((fmt_dir(sub), href))

    if src.stem.lower() != 'readme':
        crumbs.append((fmt_file(src.stem), ''))

    return crumbs

# ── CSS ───────────────────────────────────────────────────────────────────────

CSS = """
/* ── Reset ──────────────────────────────────────────────────────────── */
*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

/* ── Variables ───────────────────────────────────────────────────────── */
:root {
  --sb-w:       260px;
  --sb-bg:      #0d1117;
  --sb-text:    #8b949e;
  --sb-active:  #58a6ff;
  --sb-hover:   #161b22;
  --sb-border:  #21262d;
  --accent:     #58a6ff;
  --bg:         #ffffff;
  --text:       #24292f;
  --muted:      #57606a;
  --border:     #d0d7de;
  --code-bg:    #161b22;
  --code-text:  #e6edf3;
  --pre-bg:     #0d1117;
  --tbl-head:   #f6f8fa;
  --tbl-alt:    #f6f8fa;
  --bq-border:  #58a6ff;
  --bq-bg:      #f1f8ff;
  --radius:     6px;
  --shadow:     0 1px 3px rgba(0,0,0,.12);
}

/* ── Layout ──────────────────────────────────────────────────────────── */
html, body { height: 100%; }
body {
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Helvetica, Arial, sans-serif;
  font-size: 16px;
  line-height: 1.7;
  color: var(--text);
  background: var(--bg);
}

#layout {
  display: flex;
  min-height: 100vh;
}

/* ── Sidebar ─────────────────────────────────────────────────────────── */
#sidebar {
  width: var(--sb-w);
  flex-shrink: 0;
  background: var(--sb-bg);
  color: var(--sb-text);
  display: flex;
  flex-direction: column;
  position: fixed;
  top: 0; left: 0;
  height: 100vh;
  overflow-y: auto;
  overflow-x: hidden;
  scrollbar-width: thin;
  scrollbar-color: #30363d transparent;
  z-index: 200;
  transition: transform .25s ease;
}
#sidebar::-webkit-scrollbar { width: 4px; }
#sidebar::-webkit-scrollbar-thumb { background: #30363d; border-radius: 4px; }

#sidebar-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 18px 16px 14px;
  border-bottom: 1px solid var(--sb-border);
  position: sticky;
  top: 0;
  background: var(--sb-bg);
  z-index: 1;
}

.logo {
  font-size: .78rem;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: .08em;
  color: #e6edf3;
  line-height: 1.3;
}
.logo span { color: var(--accent); }

#toggle-btn {
  background: none;
  border: none;
  color: var(--sb-text);
  cursor: pointer;
  font-size: 1.1rem;
  padding: 4px 6px;
  border-radius: 4px;
  line-height: 1;
}
#toggle-btn:hover { background: var(--sb-hover); color: #e6edf3; }

#sidenav { padding: 10px 0 40px; }

#sidenav .nav-home {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 7px 16px;
  font-size: .82rem;
  font-weight: 600;
  color: #e6edf3;
  text-decoration: none;
}
#sidenav .nav-home:hover { color: var(--accent); }

/* Section <details> */
#sidenav details { border-bottom: 1px solid var(--sb-border); }
#sidenav details summary {
  list-style: none;
  padding: 9px 16px;
  font-size: .78rem;
  font-weight: 700;
  text-transform: uppercase;
  letter-spacing: .07em;
  color: #8b949e;
  cursor: pointer;
  user-select: none;
  display: flex;
  align-items: center;
  gap: 6px;
}
#sidenav details summary::-webkit-details-marker { display: none; }
#sidenav details summary::before {
  content: "›";
  display: inline-block;
  transition: transform .2s;
  font-size: 1rem;
  color: #484f58;
}
#sidenav details[open] summary::before { transform: rotate(90deg); }
#sidenav details summary:hover { color: #e6edf3; }

.nav-items { padding: 2px 0 8px; }

.nav-link {
  display: block;
  padding: 5px 16px 5px 28px;
  font-size: .82rem;
  color: var(--sb-text);
  text-decoration: none;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  border-left: 2px solid transparent;
  transition: background .15s, color .15s;
}
.nav-link:hover {
  background: var(--sb-hover);
  color: #e6edf3;
}
.nav-link.active {
  color: var(--accent);
  border-left-color: var(--accent);
  background: #0d2340;
  font-weight: 600;
}
.nav-link.nav-deep { padding-left: 40px; font-size: .79rem; }

/* ── Main content ────────────────────────────────────────────────────── */
#main {
  margin-left: var(--sb-w);
  flex: 1;
  min-width: 0;
  display: flex;
  flex-direction: column;
}

#topbar {
  position: sticky;
  top: 0;
  background: rgba(255,255,255,.95);
  backdrop-filter: blur(6px);
  border-bottom: 1px solid var(--border);
  padding: 10px 40px;
  z-index: 100;
  display: flex;
  align-items: center;
  gap: 12px;
}

#mobile-toggle {
  display: none;
  background: none;
  border: 1px solid var(--border);
  border-radius: 4px;
  padding: 4px 8px;
  cursor: pointer;
  font-size: .9rem;
}

.breadcrumb {
  font-size: .82rem;
  color: var(--muted);
}
.breadcrumb a { color: var(--muted); text-decoration: none; }
.breadcrumb a:hover { color: var(--accent); text-decoration: underline; }
.breadcrumb .sep { margin: 0 6px; color: var(--border); }
.breadcrumb .current { color: var(--text); font-weight: 500; }

/* ── Prose ───────────────────────────────────────────────────────────── */
.prose {
  max-width: 880px;
  padding: 40px 40px 60px;
  flex: 1;
}

.prose h1 {
  font-size: 2rem;
  font-weight: 700;
  border-bottom: 2px solid var(--border);
  padding-bottom: 12px;
  margin-bottom: 24px;
  line-height: 1.3;
}
.prose h2 {
  font-size: 1.45rem;
  font-weight: 700;
  border-bottom: 1px solid var(--border);
  padding-bottom: 8px;
  margin: 40px 0 16px;
}
.prose h3 { font-size: 1.2rem;  font-weight: 600; margin: 28px 0 12px; }
.prose h4 { font-size: 1.05rem; font-weight: 600; margin: 22px 0 10px; }
.prose h5, .prose h6 { font-size: .95rem; font-weight: 600; margin: 18px 0 8px; }

.prose h1, .prose h2, .prose h3,
.prose h4, .prose h5, .prose h6 { color: #0d1117; }

.prose p       { margin-bottom: 16px; }
.prose ul, .prose ol { margin: 0 0 16px 1.6em; }
.prose li      { margin-bottom: 4px; }
.prose li > ul, .prose li > ol { margin-top: 4px; margin-bottom: 4px; }

.prose a       { color: var(--accent); text-decoration: none; }
.prose a:hover { text-decoration: underline; }

.prose strong  { font-weight: 700; color: #0d1117; }
.prose em      { font-style: italic; }
.prose del     { text-decoration: line-through; color: var(--muted); }

.prose hr {
  border: none;
  border-top: 1px solid var(--border);
  margin: 32px 0;
}

.prose img {
  max-width: 100%;
  height: auto;
  border-radius: var(--radius);
  box-shadow: var(--shadow);
}

/* ── Inline code ─────────────────────────────────────────────────────── */
.inline-code {
  font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
  font-size: .85em;
  background: #f0f3f6;
  color: #cf222e;
  border: 1px solid #d0d7de;
  border-radius: 4px;
  padding: 1px 5px;
}

/* ── Code blocks ─────────────────────────────────────────────────────── */
.prose pre {
  background: var(--pre-bg);
  border: 1px solid #30363d;
  border-radius: var(--radius);
  padding: 20px 22px;
  overflow-x: auto;
  margin: 16px 0 20px;
  line-height: 1.6;
  position: relative;
}

.prose pre code {
  font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
  font-size: .85rem;
  color: var(--code-text);
  background: none;
  border: none;
  padding: 0;
  white-space: pre;
}

/* Language label via CSS attr() isn't widely supported,
   so we use a data-lang trick added by the builder. */
.prose pre[data-lang]::after {
  content: attr(data-lang);
  position: absolute;
  top: 8px; right: 12px;
  font-size: .68rem;
  font-family: "SFMono-Regular", monospace;
  color: #484f58;
  text-transform: uppercase;
  letter-spacing: .06em;
}

/* ── Syntax colouring (no JS highlighter — keyword CSS classes) ───────
   Pandoc/basic highlighting adds .kw .dt .dv .st .co etc. if available.
   Otherwise the code is just uniformly styled, which is still readable. */
.prose pre code .kw  { color: #ff7b72; }   /* keyword */
.prose pre code .dt  { color: #79c0ff; }   /* data type */
.prose pre code .dv  { color: #79c0ff; }   /* decimal value */
.prose pre code .st  { color: #a5d6ff; }   /* string */
.prose pre code .co  { color: #8b949e; font-style: italic; }  /* comment */
.prose pre code .op  { color: #ffa657; }   /* operator */
.prose pre code .bn  { color: #79c0ff; }   /* base-N number */
.prose pre code .fl  { color: #79c0ff; }   /* float */
.prose pre code .ch  { color: #a5d6ff; }   /* char */
.prose pre code .fn  { color: #d2a8ff; }   /* function */

/* ── Tables ──────────────────────────────────────────────────────────── */
.table-wrap   { overflow-x: auto; margin: 16px 0 20px; }
.prose table  { border-collapse: collapse; width: 100%; font-size: .9rem; }
.prose thead tr { background: var(--tbl-head); }
.prose th, .prose td {
  border: 1px solid var(--border);
  padding: 8px 14px;
  text-align: left;
}
.prose th     { font-weight: 600; white-space: nowrap; }
.prose tbody tr:nth-child(even) { background: #fafbfc; }
.prose tbody tr:hover { background: #f0f6ff; }

/* ── Blockquotes ─────────────────────────────────────────────────────── */
.prose blockquote {
  border-left: 4px solid var(--bq-border);
  background: var(--bq-bg);
  margin: 16px 0;
  padding: 12px 20px;
  border-radius: 0 var(--radius) var(--radius) 0;
  color: var(--muted);
  font-style: italic;
}
.prose blockquote p { margin: 0; }

/* ── Footer ──────────────────────────────────────────────────────────── */
.site-footer {
  padding: 20px 40px;
  font-size: .78rem;
  color: var(--muted);
  border-top: 1px solid var(--border);
}

/* ── Index page cards ────────────────────────────────────────────────── */
.card-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(240px, 1fr));
  gap: 16px;
  margin: 24px 0;
}
.card {
  border: 1px solid var(--border);
  border-radius: var(--radius);
  padding: 18px 20px;
  text-decoration: none;
  color: var(--text);
  transition: box-shadow .15s, border-color .15s;
}
.card:hover {
  border-color: var(--accent);
  box-shadow: 0 4px 12px rgba(88,166,255,.15);
}
.card-num  { font-size: .72rem; color: var(--muted); font-weight: 700; text-transform: uppercase; }
.card-name { font-size: 1rem; font-weight: 600; margin: 4px 0 6px; color: #0d1117; }
.card-desc { font-size: .82rem; color: var(--muted); }

/* ── Sidebar collapsed ───────────────────────────────────────────────── */
#sidebar.collapsed  { transform: translateX(calc(-1 * var(--sb-w))); }
#main.sb-collapsed  { margin-left: 0; }

/* ── Responsive ──────────────────────────────────────────────────────── */
@media (max-width: 768px) {
  #sidebar            { transform: translateX(calc(-1 * var(--sb-w))); }
  #sidebar.open       { transform: translateX(0); box-shadow: 4px 0 24px rgba(0,0,0,.4); }
  #main               { margin-left: 0; }
  #mobile-toggle      { display: block; }
  .prose              { padding: 24px 20px 48px; }
  #topbar             { padding: 8px 16px; }
}
"""

# ── HTML page template ────────────────────────────────────────────────────────

def render_page(title: str, body: str, nav: str, crumbs: list[tuple[str, str]]) -> str:
    bc_parts: list[str] = []
    for idx, (label, href) in enumerate(crumbs):
        if idx:
            bc_parts.append('<span class="sep">›</span>')
        if href and idx < len(crumbs) - 1:
            bc_parts.append(f'<a href="{href}">{_html.escape(label)}</a>')
        else:
            cls = ' class="current"' if idx == len(crumbs) - 1 else ''
            bc_parts.append(f'<span{cls}>{_html.escape(label)}</span>')
    bc_html = ' '.join(bc_parts)

    return f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>{_html.escape(title)} — DIY Embedded Systems</title>
  <style>{CSS}</style>
</head>
<body>

<div id="layout">

  <!-- Sidebar -->
  <aside id="sidebar">
    <div id="sidebar-header">
      <div class="logo"><span>Embedded</span><br>Systems Docs</div>
      <button id="toggle-btn" aria-label="collapse sidebar">‹</button>
    </div>
    {nav}
  </aside>

  <!-- Main -->
  <div id="main">
    <div id="topbar">
      <button id="mobile-toggle" aria-label="open menu">☰</button>
      <nav class="breadcrumb" aria-label="breadcrumb">{bc_html}</nav>
    </div>

    <div class="prose">
{body}
    </div>

    <footer class="site-footer">
      Built {BUILD_TS} &nbsp;·&nbsp; DIY Embedded Systems &amp; Drone Electronics
    </footer>
  </div>

</div>

<script>
(function () {{
  var sidebar = document.getElementById('sidebar');
  var main    = document.getElementById('main');

  // Desktop collapse/expand
  document.getElementById('toggle-btn').addEventListener('click', function () {{
    var c = sidebar.classList.toggle('collapsed');
    main.classList.toggle('sb-collapsed', c);
    this.textContent = c ? '›' : '‹';
  }});

  // Mobile open/close
  var mob = document.getElementById('mobile-toggle');
  if (mob) {{
    mob.addEventListener('click', function () {{
      sidebar.classList.toggle('open');
    }});
    document.addEventListener('click', function (e) {{
      if (!sidebar.contains(e.target) && !mob.contains(e.target)) {{
        sidebar.classList.remove('open');
      }}
    }});
  }}

  // Add data-lang to pre blocks for the language label
  document.querySelectorAll('pre > code[class]').forEach(function (code) {{
    var m = code.className.match(/language-(\\w+)/);
    if (m) code.parentElement.setAttribute('data-lang', m[1]);
  }});

  // Scroll active nav item into view
  var active = document.querySelector('.nav-link.active');
  if (active) active.scrollIntoView({{block: 'nearest'}});
}})();
</script>

</body>
</html>"""


# ── Index page ────────────────────────────────────────────────────────────────

def build_index_page(pairs: list[tuple[Path, Path]], nav: str) -> str:
    """Generate a landing page with module cards."""
    # Find top-level section directories
    sections: dict[str, list[tuple[Path, Path]]] = defaultdict(list)
    for src, dest in pairs:
        rel = src.relative_to(ROOT)
        if len(rel.parts) >= 2:
            sections[rel.parts[0]].append((src, dest))

    cards: list[str] = []
    module_descriptions = {
        '01_electronics_fundamentals': 'Voltage, current, components, circuits, radio & signals',
        '02_microcontrollers':         'MCU architecture, ARM, RISC-V, assembly programming',
        '03_esp32':                    'ESP32 pinout, toolchain, GPIO, UART, WiFi',
        '04_communication_protocols':  'UART, I2C, SPI, nRF24L01 wireless',
        '05_sensors_and_modules':      'IMU, barometer, ultrasonic, compass, OLED',
        '06_power_systems':            'Regulators, LiPo, power design',
        '07_drone_electronics':        'Motors, ESC, flight controller, PID, radio control',
        '08_projects':                 'Real build logs and complete project code',
    }

    for sec in sorted(sections):
        m   = re.match(r'^(\d+)[_-](.+)$', sec)
        num = m.group(1) if m else ''
        name = fmt_dir(sec)
        desc = module_descriptions.get(sec, '')
        # link to README if exists, else first file
        readme = SITE / sec / 'README.html'
        items  = sections[sec]
        href   = '/' + readme.relative_to(SITE).as_posix()
        cards.append(
            f'<a class="card" href="{href}">'
            f'<div class="card-num">Module {num}</div>'
            f'<div class="card-name">{_html.escape(name.split(" · ", 1)[-1])}</div>'
            f'<div class="card-desc">{_html.escape(desc)}</div>'
            f'</a>'
        )

    # Convert root README body
    readme_src = ROOT / 'README.md'
    if readme_src.exists():
        intro = md_to_html(readme_src.read_text(encoding='utf-8'))
    else:
        intro = '<p>Welcome to the Embedded Systems documentation.</p>'

    body = f'{intro}\n<div class="card-grid">\n' + '\n'.join(cards) + '\n</div>'

    crumbs: list[tuple[str, str]] = [('Home', '')]
    return render_page('DIY Embedded Systems', body, nav, crumbs)


# ── Main build ────────────────────────────────────────────────────────────────

def build() -> None:
    pairs = collect()

    print(f'  Found {len(pairs)} markdown files')
    print(f'  Output → {SITE}')

    # Clean and recreate site directory
    if SITE.exists():
        shutil.rmtree(SITE)
    SITE.mkdir(parents=True)

    # Pre-build nav (without active page — will regenerate per-page below)
    base_nav = build_nav(pairs, current=None)

    errors = 0
    for src, dest in pairs:
        try:
            dest.parent.mkdir(parents=True, exist_ok=True)
            text   = src.read_text(encoding='utf-8', errors='replace')
            body   = md_to_html(text)
            nav    = build_nav(pairs, current=dest)
            crumbs = make_breadcrumbs(src)
            title  = fmt_file(src.stem)
            page   = render_page(title, body, nav, crumbs)
            dest.write_text(page, encoding='utf-8')
            rel = src.relative_to(ROOT)
            print(f'  [ok]  {rel}')
        except Exception as exc:
            print(f'  [ERR] {src.relative_to(ROOT)}: {exc}', flush=True)
            errors += 1

    # Build root index.html
    root_nav  = build_nav(pairs, current=SITE / 'index.html')
    index_html = build_index_page(pairs, root_nav)
    (SITE / 'index.html').write_text(index_html, encoding='utf-8')
    print(f'  [ok]  index.html')

    print(f'\n  Done — {len(pairs) + 1} pages built{"  (" + str(errors) + " errors)" if errors else ""}')


def serve(port: int = PORT) -> None:
    os.chdir(SITE)

    class Handler(http.server.SimpleHTTPRequestHandler):
        def log_message(self, fmt, *args):
            pass  # suppress per-request logs; keep output clean

    with socketserver.TCPServer(('', port), Handler) as httpd:
        httpd.allow_reuse_address = True
        url = f'http://localhost:{port}'
        print(f'\n  Serving docs at  {url}')
        print(f'  Press Ctrl+C to stop.\n')
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print('\n  Server stopped.')


# ── Entry point ───────────────────────────────────────────────────────────────

if __name__ == '__main__':
    ap = argparse.ArgumentParser(description='Build embedded-system HTML docs')
    ap.add_argument('--serve', action='store_true', help='serve after building')
    ap.add_argument('--port',  type=int, default=PORT, help=f'port (default {PORT})')
    args = ap.parse_args()

    print('\nBuilding docs...')
    build()

    if args.serve:
        serve(args.port)
