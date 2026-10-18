# Translated book build - mirrors the infrastructure of the root-Makefile.
#
# Usage (from language subdir (i.e. dutch/) or via `make -C <language>`):
#   make                        all six books (mega65-book first)
#   make mega65-userguide       one book (alias; also for other book names)
#   make mega65-userguide-cmyk  cmyk-variant for printing
#   make chapter-<hoofdstuk>    separate chapter as smoke test (build-chapter.sh)
#   make regtables              regenerate regtables (document-memory + --apply)
#   make regtables-verify       structure check of regtables
#   make deps                   refresh per-book dependency lists (build/*.deps)
#   make clean                  clean all build arcefacts
#
# From repo-root: `make dutch-books` (or `make dutch`).
#
# ATTENTION:
#  - Error checking (FOUTPATROON) is also present in tools/build-book-sh;
#    change both places at the same time.
#  - Do not build with -h: the books share gitinfo.tex in the repo-root.
#  - Generates files from English sources (keymap tables, examples,
#    basic65-derivates) have their rules in the root-Makefile; those are called
#    with $(MAKE) -C .. so that this knowledge remains in one spot.
#  - The translated instruction-set/opcode tables are the exception: their
#    rules live here, because the inst.*/mode.* sources and the output language
#    are specific to this directory. Only the generator binary and the English
#    .opc/.cycles files are delegated to the root-Makefile.

ROOT := ..
BUILD_DIR := build
REPOPATH ?= .
THISLANG := dutch

# BibTeX looks for the bibliography style file in dutch/IEEEtran-nl.bst, The recipe runs
# latexmk with the repo-root as the working directory, so the path must be explicitly
#specified via kpathsea. The closing ':' keeps the default search path above it
# (IEEEtran.bst from TeX Live, references.bib in the root). Note: $(ROOT) is relative
# here and the recipe changes directory to the root, so use an absolute path via
# $(abspath ..).
export BSTINPUTS := $(abspath ..)/$(THISLANG):

BOOKS := mega65-book \
	mega65-userguide \
	mega65-developer-guide \
	mega65-assembly-reference \
	mega65-chipset-reference \
	mega65-basic65-reference

PDFS := $(addsuffix .pdf,$(BOOKS))
CMYK_PDFS := $(addsuffix -cmyk.pdf,$(BOOKS))

FOUTPATROON = ^! [A-Za-z]|Emergency stop|Undefined control sequence|No pages of output

# Translated source files
FRONTCOVERS := frontcover.tex $(wildcard frontcover/*.pdf)

# English source files using by the translated books
ROOT_STATIC := $(ROOT)/fonts.tex \
	$(ROOT)/lstlang0.sty \
	$(ROOT)/references.bib \
	$(wildcard $(ROOT)/elements/*.tex)

# Generated English files (rules are in the root-Makefile)
ROOT_GEN := $(addprefix $(ROOT)/, \
	keymap_table_1.tex keymap_table_2.tex keymap_table_3.tex \
	keymap_table_4.tex keymap_table_5.tex unicode_mapping.tex \
	examples/iec-data-logger.tex examples/iec-data-logger-2.tex)

# Translated instruction-set/opcode tables. The generator and the English
# .opc/.cycles files are shared with the English books, so those stay in the
# root-Makefile (the root also owns compiling instruction_set); only the
# inst.*/mode.* descriptions and the generated strings are Dutch (--lang nl).
INSTSETS_DIR := instruction_sets
INSTR_TABLES := instructionset-4510.tex instructionset-6502.tex \
	instructionset-45GS02.tex opcodetable-4510.tex opcodetable-6502.tex
INSTR_GEN := $(INSTR_TABLES)
INSTR_SRC := $(wildcard $(INSTSETS_DIR)/inst.*) $(wildcard $(INSTSETS_DIR)/mode.*)
# The English opcode tables themselves (they are inputs, and must be newer).
INSTR_OPC := $(wildcard $(ROOT)/instruction_sets/*.opc) \
	$(wildcard $(ROOT)/instruction_sets/*.cycles)

# Per-book source lists. build/<book>.deps is generated from the .fls of the
# last build (tools/make-book-deps.py) and pulled in with -include. It only
# *contributes* prerequisites: it has no recipe, so the pattern rule below
# stays in charge, and its own mtime is irrelevant - it can never make a book
# look out of date. The .fls is the truth here: chapters pull in regtables,
# keymap tables and instruction-set/opcode tables indirectly, so the \input
# list of the book master alone is not complete.
DEPS_FILES := $(addprefix $(BUILD_DIR)/,$(addsuffix .deps,$(BOOKS)))
-include $(DEPS_FILES)

# Sources shared by all books (i.e. everything outside the per-book lists):
# the shared header, the covers (pdfpages reads them directly, so they never
# show up in the .fls) and the English files whose generation rules are in the
# root-Makefile.
SHARED_DEPS := $(wildcard common-header-*.tex) $(FRONTCOVERS) \
	$(ROOT_STATIC) $(ROOT_GEN) $(INSTR_GEN) $(INSTR_SRC)

.DELETE_ON_ERROR:

# The -include of build/<book>.deps below happens before the first rule, and
# make would take the first target from an included file as the default goal.
# Pin it: a bare `make` must build all six books.
.DEFAULT_GOAL := all

.PHONY: all clean regtables regtables-verify deps check-book-% \
	$(BOOKS) $(addsuffix -cmyk,$(BOOKS))

all: $(PDFS)

# ---------------------------------------------------------------------------
# Books: wrapper + latexmk + error check, same check as in
# tools/build-book.sh. latexmk runs in the repo-root so that the aux files
# (cross-references) land there; the log moves to dutch/build/.
# ---------------------------------------------------------------------------
mega65-%.pdf: | check-book-% $(SHARED_DEPS)
	cd $(ROOT) && ./getgitinfo
	mkdir -p $(BUILD_DIR)
	printf '\\input{%s}\n\\end{document}\n' '$(THISLANG)/mega65-$*' > $(BUILD_DIR)/mega65-$*.tex
	@rc=0; (cd $(ROOT) && latexmk -outdir=$(THISLANG) -pdf -use-make \
		-pdflatex="xelatex -interaction=nonstopmode" $(THISLANG)/$(BUILD_DIR)/mega65-$*.tex) || rc=$$?; \
	mv -f mega65-$*.log $(BUILD_DIR)/ 2>/dev/null || true; \
	test -f $(BUILD_DIR)/mega65-$*.log || { \
		echo "==> FOUT: geen $(BUILD_DIR)/$*.log (latexmk/xelatex niet gedraaid?)"; \
		exit 1; }; \
	if grep -q -E '$(FOUTPATROON)' $(BUILD_DIR)/mega65-$*.log; then \
		echo "==> FOUTEN gevonden in $(BUILD_DIR)/mega65-$*.log:"; \
		grep -n -E '$(FOUTPATROON)' $(BUILD_DIR)/mega65-$*.log | head -20; \
		exit 1; \
	elif [ $$rc -ne 0 ]; then \
		echo "==> latexmk faalde (exit $$rc); laatste regels van de log:"; \
		tail -20 $(BUILD_DIR)/mega65-$*.log; \
		exit $$rc; \
	else \
		python3 tools/make-book-deps.py $*; \
		touch $@; \
		if [ "$*" = "book" ]; then \
			cp -f mega65-book.aux $(CROSSREF_SEEN) 2>/dev/null || true; \
			touch $(CROSSREF_CHECKED) 2>/dev/null || true; \
		fi; \
		echo "==> build OK: $@"; \
	fi

# Note: the `touch $@` above ensures that the build remains idempotent. latexmk
# does nothing besides changing an mtime (content identical) en the leaves the
# pdf untouched; without the touch, make would continue to see that target as
# outdated at every next run.

# Mistyped book names: the pattern rule above matches any mega65-*.pdf, so a
# typo would silently build a wrapper for a book that does not exist. Check the
# master exists, and warn when a book's dependency list is missing (nothing to
# compare against, so `make` would report "up to date" without building).
.PHONY: check-book-%
check-book-%:
	@$(if $(wildcard mega65-$*.tex),,$(error onbekend boek 'mega65-$*' - geldige namen: $(BOOKS)))
	@$(if $(wildcard $(BUILD_DIR)/mega65-$*.deps),,:;$(if $(wildcard mega65-$*.pdf),echo "==> LET OP: $(BUILD_DIR)/mega65-$*.deps ontbreekt; bronbestandenlijst onbekend (draai 'make deps' na een build)"))

# The sub-books read their chapter titles/page numbers from the big book's aux
# (dutch/common-header-dutch.tex; TROUBLESHOOTING.md trap 10). That aux is all
# they need - not the 1500-page pdf.
#
# Two small markers keep that cheap and correct:
#
#   mega65-book.stamp   "which aux version have the sub-books consumed?" The
#                       sub-books depend on it, and it is only re-touched when
#                       the aux content really changed - so a rebuild of the big
#                       book that leaves the page numbers alone does not cascade
#                       into the other five books.
#   mega65-book.checked "have the big book's sources changed since we last had a
#                       look?" Its rule (sources come from build/mega65-book.deps)
#                       only warns; it is order-only, so it can never trigger a
#                       rebuild, and it is touched after each run so the warning
#                       appears once per source change instead of every `make`.
#
# A sub-book build therefore no longer drags in a rebuild of the big book - that
# cost 1500 pages for what is usually an unchanged aux. Editing a chapter that is
# shared with the big book builds only the books that contain it, and warns that
# the cross-references should be refreshed with `make mega65-book`.
#
# A full `make` stays strict: mega65-book is the first goal, and if its build
# changes the aux, the stamp rules make every sub-book rebuild itself.
CROSSREF_STAMP := $(BUILD_DIR)/mega65-book.stamp
CROSSREF_PREV := $(BUILD_DIR)/mega65-book.aux.prev
CROSSREF_CHECKED := $(BUILD_DIR)/mega65-book.checked
CROSSREF_SEEN := $(CROSSREF_CHECKED).aux

$(CROSSREF_STAMP): mega65-book.aux
	@if [ ! -f $(CROSSREF_STAMP) ] || ! cmp -s mega65-book.aux $(CROSSREF_PREV); then \
		cp -f mega65-book.aux $(CROSSREF_PREV) 2>/dev/null || true; \
		touch $@; \
		echo "==> mega65-book: aux gewijzigd, deelboeken worden opnieuw gebouwd"; \
	fi

$(CROSSREF_CHECKED): mega65-book.aux
	@if [ ! -f mega65-book.aux ]; then \
		echo "==> LET OP: mega65-book.aux ontbreekt; dit boek valt terug op de Engelse kruisverwijzingen (draai 'make mega65-book')"; \
	elif cmp -s mega65-book.aux $(CROSSREF_SEEN); then \
		echo "==> LET OP: de kruisverwijzingen van dit boek kunnen verouderd zijn."; \
		echo "==>          De bronnen van het grote boek zijn gewijzigd sinds zijn laatste build."; \
		echo "==>          Draai 'make mega65-book' om ze te verversen (of 'make' voor alles)."; \
	else \
		cp -f mega65-book.aux $(CROSSREF_SEEN) 2>/dev/null || true; \
	fi; \
	touch $@

$(filter-out mega65-book.pdf,$(PDFS)): $(CROSSREF_STAMP) | $(CROSSREF_CHECKED)

# Aliassen per boek: make mega65-userguide
$(BOOKS): %: %.pdf

# ---------------------------------------------------------------------------
# CMYK (drukwerk), dezelfde Ghostscript-instellingen als de root-Makefile
# ---------------------------------------------------------------------------
$(ROOT)/%-cmyk.pdf: %.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite \
		-sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK \
		-sOutputFile=$@ $<

$(addsuffix -cmyk,$(BOOKS)): %-cmyk: %-cmyk.pdf

# ---------------------------------------------------------------------------
# Files generated from English: root-Makefile is owner; delegate to there
# (including tool-dependencies so that freshness is correct - same chains 
# as in the root-Makefile).
# ---------------------------------------------------------------------------
$(ROOT)/keymap: $(ROOT)/keymap.c $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) keymap

$(ROOT)/keymap_table_1.tex $(ROOT)/keymap_table_2.tex \
	$(ROOT)/keymap_table_3.tex $(ROOT)/keymap_table_4.tex \
	$(ROOT)/keymap_table_5.tex $(ROOT)/unicode_mapping.tex: $(ROOT)/keymap
	$(MAKE) -C $(ROOT) $(notdir $@)

$(ROOT)/prg2tex: $(ROOT)/prg2tex.c $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) prg2tex

$(ROOT)/examples/iec-data-logger.tex: $(ROOT)/examples/iec-data-logger.prg \
		$(ROOT)/prg2tex $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) $(patsubst $(ROOT)/%,%,$@)

$(ROOT)/examples/iec-data-logger-2.tex: $(ROOT)/examples/iec-data-logger-2.prg \
		$(ROOT)/prg2tex $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) $(patsubst $(ROOT)/%,%,$@)

$(ROOT)/index_basic_programmes: $(ROOT)/index_basic_programmes.c $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) index_basic_programmes

$(ROOT)/generate_condensed: $(ROOT)/generate_condensed.c $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) generate_condensed

# Translated instruction-set/opcode tables (owner: this Makefile; the English
# .opc/.cycles files and the generator binary stay in the root-Makefile).
$(ROOT)/instruction_set: $(ROOT)/instruction_set.c $(ROOT)/Makefile
	$(MAKE) -C $(ROOT) instruction_set

instructionset-45GS02.tex: $(ROOT)/instruction_set $(INSTR_OPC) $(INSTR_SRC)
	$(ROOT)/instruction_set --lang nl --sets-dir $(INSTSETS_DIR) \
		$(ROOT)/instruction_sets/45GS02.opc

instructionset-4510.tex opcodetable-4510.tex: $(ROOT)/instruction_set $(INSTR_OPC) $(INSTR_SRC)
	$(ROOT)/instruction_set --lang nl --sets-dir $(INSTSETS_DIR) \
		$(ROOT)/instruction_sets/4510.opc $(ROOT)/instruction_sets/45GS02.opc

instructionset-6502.tex opcodetable-6502.tex: $(ROOT)/instruction_set $(INSTR_OPC) $(INSTR_SRC)
	$(ROOT)/instruction_set --lang nl --sets-dir $(INSTSETS_DIR) \
		$(ROOT)/instruction_sets/6502.opc

# Basic65 derived files: rules are in the root-Makefile (with correct NL chain;
appendix-basic65-indexed.tex: appendix-basic65.tex $(ROOT)/index_basic_programmes
	$(ROOT)/index_basic_programmes -l nl appendix-basic65.tex > $@

appendix-basic65-condensed.tex: appendix-basic65.tex $(ROOT)/generate_condensed
	$(ROOT)/generate_condensed appendix-basic65.tex $@

# ---------------------------------------------------------------------------
# Maintenance: per-book dependency lists (build/<book>.deps) from the .fls.
# Not part of `all`: the lists are refreshed automatically after every
# successful book build; this target is for a manual refresh.
# ---------------------------------------------------------------------------
deps:
	python3 tools/make-book-deps.py

# ---------------------------------------------------------------------------
# Maintenance: localize regtables. Not included in `all`; the translated
# build uses the translated regtable_*.tex as source
# ---------------------------------------------------------------------------
regtables:
	cd $(ROOT) && $(MAKE) document-memory
	cd $(ROOT) && ./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	cd $(ROOT) && python3 $(THISLANG)/tools/localize-regtables.py --apply --missing-ok

regtables-verify:
	cd $(ROOT) && python3 $(THISLANG)/tools/localize-regtables.py --verify

# ---------------------------------------------------------------------------
# Smoke test: one single chapter
# ---------------------------------------------------------------------------
chapter-%: %.tex
	./tools/build-chapter.sh $* $(THISLANG)

clean:
	rm -f -- *.pdf ./$(BUILD_DIR)/*
