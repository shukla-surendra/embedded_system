PORT     := 8080
SITE_DIR := site
SCRIPT   := scripts/build_docs.py

.PHONY: docs docs-build docs-serve docs-clean help

## docs: build HTML site and start local server at http://localhost:8080
docs: docs-serve

## docs-build: convert all .md files to HTML (no server)
docs-build:
	@python3 $(SCRIPT)

## docs-serve: build then start the HTTP server
docs-serve:
	@python3 $(SCRIPT) --serve --port $(PORT)

## docs-clean: remove generated site/ directory
docs-clean:
	@rm -rf $(SITE_DIR)
	@echo "Removed $(SITE_DIR)/"

## help: list available targets
help:
	@grep -E '^## ' $(MAKEFILE_LIST) | sed 's/^## /  /'
