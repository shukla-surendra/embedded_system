# Radio From First Principles — Stage-by-Stage Lab Guide

Build your understanding of radio using only wire, coils, capacitors, resistors, diodes, and transistors. No ICs. No modules. No black boxes.

Each stage is a complete, standalone practical guide — circuit diagrams, exact component values, step-by-step build instructions, calculations, and troubleshooting.

Work through them in order. Each stage builds directly on the previous one.

---

## The One Idea Behind All of Radio

> A wire carrying a **changing current** radiates energy into space as an electromagnetic wave.
> A changing electromagnetic wave passing a wire **induces a current** in it.

Transmitter = make changing current → radiate.
Receiver = wave arrives → induced current → detect it.

Everything in these 10 stages is engineering built on this single physical fact.

---

## Stage Map

```
STAGE 1          STAGE 2          STAGE 3          STAGE 4          STAGE 5
Spark & Hertz → Crystal Set   → LC Resonance  → Diode Detector → First Amplifier
(wave exists)   (receive,        (tuning,         (detect the      (gain, drive
                no battery)      Q factor)         signal)          a speaker)
                    ↓
STAGE 6          STAGE 7          STAGE 8          STAGE 9          STAGE 10
Regenerative  → Colpitts      → Mixer &       → Complete      → FM & Beyond
Receiver         Oscillator      Heterodyne       Superhet         (FM detect,
(far stations,  (transmit your   (frequency       (full AM radio   SSB, BFO,
1 transistor)    own signal)      shifting)        from scratch)    path to ICs)
```

---

## Stage Index

| Stage | File | What You Build | Key Concept Learned |
|-------|------|---------------|---------------------|
| 1 | [stage_01_spark_and_hertz.md](stage_01_spark_and_hertz.md) | Leyden jar, induction coil, spark experiment | EM waves exist — a spark IS a radio transmission |
| 2 | [stage_02_crystal_set.md](stage_02_crystal_set.md) | Full crystal set receiver | Resonance selects frequency; diode detects it; no battery needed |
| 3 | [stage_03_lc_resonance_deep_dive.md](stage_03_lc_resonance_deep_dive.md) | Multiple LC circuits, observe resonance | Resonant frequency formula derived; Q factor; core materials |
| 4 | [stage_04_diode_detector.md](stage_04_diode_detector.md) | Diode detector + galena cat's whisker | Why germanium beats silicon; envelope detection waveforms |
| 5 | [stage_05_first_amplifier_transistor.md](stage_05_first_amplifier_transistor.md) | Common-emitter amplifier; crystal set + amp | Transistor biasing; gain calculation; drives a speaker |
| 6 | [stage_06_regenerative_receiver.md](stage_06_regenerative_receiver.md) | Regenerative receiver (1 transistor) | Positive feedback multiplies Q; hear stations 500+ km away |
| 7 | [stage_07_colpitts_oscillator.md](stage_07_colpitts_oscillator.md) | Colpitts oscillator; AM voice transmitter | Barkhausen criterion; sustained oscillation; AM modulation |
| 8 | [stage_08_mixer_and_heterodyne.md](stage_08_mixer_and_heterodyne.md) | Diode ring mixer; transistor mixer | cos(A)×cos(B) identity; frequency translation; IF concept |
| 9 | [stage_09_complete_superhet.md](stage_09_complete_superhet.md) | Complete AM superheterodyne radio | All stages combined; alignment; AGC; full working radio |
| 10 | [stage_10_fm_and_beyond.md](stage_10_fm_and_beyond.md) | FM detector; BFO for SSB/CW | FM vs AM noise immunity; Foster-Seeley; PLL; path to ICs |

---

## Components You Need for All 10 Stages

### Semiconductors
| Part | Qty | Purpose | Where to buy |
|------|-----|---------|-------------|
| BC547 or 2N2222 NPN transistor | 10 | All amplifier and oscillator stages | Mouser, eBay, any electronics store |
| BC557 PNP transistor | 2 | Push-pull audio output (Stage 9) | Same as above |
| OA91 or 1N34A germanium diode | 6 | Detector, mixer (MUST be germanium) | eBay: "OA91 germanium diode" |
| BAT43 Schottky diode | 4 | Alternative detector/mixer | Mouser, Farnell |
| 1N4148 silicon diode | 5 | Power supply protection, clipping | Everywhere |

### Passive Components
| Part | Qty | Purpose | Notes |
|------|-----|---------|-------|
| Enamelled copper wire, 0.3 mm | 50 m | Winding all coils | eBay: "magnet wire 0.3mm" |
| Enamelled copper wire, 0.5 mm | 10 m | Power coils, low-resistance windings | eBay: "magnet wire 0.5mm" |
| Variable capacitor, 2-gang 365 pF | 1 | Crystal set + superhet tuning | Salvage from old AM radio, eBay |
| Trimmer capacitors 5–65 pF | 10 | IF transformer alignment, LO trimming | eBay, Mouser |
| PVC pipe 50 mm diameter | 30 cm | Coil formers | Plumbing supply |
| Ferrite rod 10 mm × 100 mm | 2 | Antenna coil, improves inductance | eBay: "ferrite rod AM radio" |
| Ferrite toroids (T50-2 or similar) | 4 | Transformer cores for mixer | eBay: "powdered iron toroid T50" |
| Ceramic capacitors assorted | 50 | All stages (1 pF – 100 nF) | Assortment pack |
| Electrolytic capacitors assorted | 20 | Power supply decoupling, audio coupling | Assortment pack |
| Resistors assorted ¼W | 100 | All stages (100 Ω – 1 MΩ) | Assortment pack |
| Potentiometer 10 kΩ | 3 | Regeneration control, volume | Any |
| Potentiometer 1 kΩ | 2 | Bias adjustment | Any |

### Specialist Parts
| Part | Qty | Purpose | Where to buy |
|------|-----|---------|-------------|
| IF transformer 455 kHz (yellow can) | 2 | IF filter in superhet (Stage 9) | eBay: "455kHz IF transformer yellow" |
| IF transformer 455 kHz (black can) | 2 | Detector transformer (Stage 9) | eBay: "455kHz IF transformer black" |
| Ceramic filter CFW455 | 1 | 455 kHz IF bandpass filter — passive component | eBay: "CFW455 ceramic filter" |
| Crystal earphone 2kΩ | 1 | Stages 2–6 audio output (needs no amp) | eBay: "crystal earphone" |
| 8Ω speaker, small | 1 | Stage 9 audio output | Scrap any small device |
| Galena crystal specimen | 1 | Stage 4 cat's whisker experiment | eBay: "galena crystal specimen" |

### Tools
| Tool | Needed for | Minimum cost |
|------|-----------|-------------|
| Multimeter | Every stage | £8 |
| Soldering iron + solder | Stage 5 onwards | £10 |
| Breadboard | All stages | £3 |
| Oscilloscope or DSO138 kit | Stage 3 onwards — very helpful | £12 (DSO138 kit) |
| Non-metallic alignment tool | Stage 9 IF alignment | Make from pencil or buy £2 |

### Total estimated cost: £25–50 / $30–60

---

## How to Use These Documents

**Each stage document contains:**
- What you will learn (goals)
- Full theory with derivations
- Complete ASCII circuit schematics
- All component values with calculations shown
- Step-by-step build sequence
- What to measure and observe
- Notebook recording template
- Troubleshooting table
- Key insight — connecting what you built to modern radio chips

**Do not skip stages.** Each experiment builds physical intuition that makes the next stage clear. If you skip to Stage 9 without building Stage 2, the schematic is just symbols. If you build Stage 2 first, Stage 9 is obviously just a more capable version of what you already understand.

---

## The Outcome

After completing all 10 stages you will be able to:

- Explain why any wireless system works, not just repeat that it does
- Read a radio IC datasheet and recognise every internal block
- Understand what your ESP32's WiFi and Bluetooth hardware is doing internally
- Diagnose interference problems on your drone RC link from first principles
- Build any simple wireless project without needing a magic module
- Know why certain antenna placements work and others fail

The ESP32 chip on your drone contains a complete superheterodyne radio receiver (Stages 8–9), a crystal-controlled local oscillator (Stage 7), a transistor-based power amplifier (Stage 7), and a demodulator (Stage 10) — all on a single die. You will have built every one of those blocks by hand.

---

## Recommended Reading Alongside the Stages

| Book | Stage | Why |
|------|-------|-----|
| *The Radio Amateur's Handbook* (ARRL) | All | The reference — practical and thorough |
| *Experimental Methods in RF Design* (Hayward, Campbell, Larkin) | 6–10 | Best discrete RF design book available |
| *The Art of Electronics* (Horowitz & Hill) | 5–10 | Deep transistor circuit understanding |
| *Radiotron Designer's Handbook* (RCA, free PDF) | 1–8 | Historical — explains all the original techniques |
