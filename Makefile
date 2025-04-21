CC=gcc

REPOPATH=.

BOOKS=	mega65-book.pdf \
	mega65-userguide.pdf \
	mega65-userguide-lulu.pdf \
	mega65-developer-guide.pdf \
	mega65-chipset-reference.pdf \
	mega65-assembly-reference.pdf \
	mega65-basic65-reference.pdf \
	mega65-basic-programming.pdf \
	styleguide.pdf

GENERATED_TEX_FILES= 	document-memory \
			instructionset-4510.tex \
		     	instructionset-6502.tex \
		     	instructionset-45GS02.tex \
			opcodetable-6502.tex \
			opcodetable-4510.tex \
		 	api-conio.tex \
			appendix-basic65-indexed.tex \
			appendix-basic65-condensed.tex \
			examples/iec-data-logger-2.tex \
			examples/iec-data-logger-2.txt \
			examples/iec-data-logger.tex \
			examples/iec-data-logger.txt \
			examples/ledcycle.tex \
			examples/ledcycle.txt \
			keymap_table_1.tex \
			keymap_table_2.tex \
			keymap_table_3.tex \
			keymap_table_4.tex \
			keymap_table_5.tex \
			unicode_mapping.tex
			#images/illustrations/screen-40x25-addresses16-80.pdf

DOCUMENT_MEMORY_FILES= \
			regtable_AUDIO.MEGA65.tex \
			regtable_AUDIOMIX.MEGA65.tex \
			regtable_AUTOIEC.MEGA65.tex \
			regtable_AUXFPGA.MEGA65.tex \
			regtable_CIA1.C64.tex \
			regtable_CIA1.MEGA65.tex \
			regtable_CIA2.C64.tex \
			regtable_CIA2.MEGA65.tex \
			regtable_CPU.C64.tex \
			regtable_CPU.MEGA65.tex \
			regtable_DEBUG.MEGA65.tex \
			regtable_DMA.C65.tex \
			regtable_DMA.MEGA65.tex \
			regtable_ETH.MEGA65.tex \
			regtable_ETHCOMMAND.MEGA65.tex \
			regtable_F011.MEGA65.tex \
			regtable_FDC.C65.tex \
			regtable_FDC.MEGA65.tex \
			regtable_FPGA.MEGA65.tex \
			regtable_FSERIAL.MEGA65.tex \
			regtable_HCPU.MEGA65.tex \
			regtable_KBD.MEGA65.tex \
			regtable_MATH.MEGA65.tex \
			regtable_MISC.MEGA65.tex \
			regtable_MISCIO.MEGA65.tex \
			regtable_NONE.MEGA65.tex \
			regtable_QSPI.MEGA65.tex \
			regtable_RTC.MEGA65.tex \
			regtable_RTC-R4.MEGA65.tex \
			regtable_RTC-GROV.MEGA65.tex \
			regtable_SD.MEGA65.tex \
			regtable_SDFDC.MEGA65.tex \
			regtable_SID.MEGA65.tex \
			regtable_SUMMARY.C65.tex \
			regtable_SUMMARY.MEGA65.tex \
			regtable_SYSCTL.MEGA65.tex \
			regtable_THUMB.MEGA65.tex \
			regtable_TOUCH.MEGA65.tex \
			regtable_TOUGH.MEGA65.tex \
			regtable_UART.C65.tex \
			regtable_UART.MEGA65.tex \
			regtable_UARTMISC.MEGA65.tex \
			regtable_UUID.MEGA65.tex \
			regtable_VIC-II.C64.tex \
			regtable_VIC-III.C65.tex \
			regtable_VIC-IV.MEGA65.tex

COMPILED_BINARIES= 	document-memory \
			generate_condensed \
			keymap \
			index_basic_programmes \
			test_basic_programmes \
			instruction_set \
			libc-doc \
			prg2tex

BIBLIOGRAPHIES=	mega65-assembly-reference.bbl \
			mega65-basic65-reference.bbl \
			mega65-book.bbl \
			mega65-chipset-reference.bbl \
			mega65-developer-guide.bbl

LISTING_FILES=	mega65-assembly-reference.listing \
			mega65-basic65-reference.listing \
			mega65-book.listing \
			mega65-chipset-reference.listing \
			mega65-developer-guide.listing \
			mega65-userguide-lulu.listing \
			mega65-userguide.listing \
			styleguide.listing

.PHONY: $(BOOKS) all clean generate-diagrams

all:	$(BOOKS)

books:	$(BOOKS)

screen-maps:	screen-maps.c Makefile
	$(CC) -Wall -o screen-maps screen-maps.c -lhpdf

keymap:	keymap.c Makefile
	$(CC) -Wall -g -o keymap keymap.c

images/illustrations/screen-40x25-addresses16-80.pdf:	screen-maps
	./screen-maps

prg2tex:	prg2tex.c
	$(CC) -Wall -o prg2tex prg2tex.c

libc-doc:	libc-doc.c
	$(CC) -g -Wall -o libc-doc libc-doc.c

EXAMPLEDIR=	examples
EXAMPLES=	$(EXAMPLEDIR)/ledcycle.tex \
		$(EXAMPLEDIR)/iec-data-logger.tex \
		$(EXAMPLEDIR)/iec-data-logger-2.tex
HYPPO_EXAMPLES= $(wildcard $(EXAMPLEDIR)/appendix-hypervisor-calls/*.asm)

%.tex:	%.prg prg2tex Makefile
	./prg2tex -u $<

keymap_table_1.tex keymap_table_2.tex keymap_table_3.tex keymap_table_4.tex keymap_table_5.tex unicode_mapping.tex:	keymap
	./keymap $(REPOPATH)/mega65-core/src/vhdl/matrix_to_ascii.vhdl

api-conio.tex:	libc-doc $(REPOPATH)/mega65-libc/include/mega65/conio.h
	./libc-doc $(REPOPATH)/mega65-libc/include/mega65/conio.h > api-conio.tex

instruction_set: instruction_set.c Makefile
	$(CC) -Wall -g -o instruction_set instruction_set.c

instructionset-45GS02.tex:	instruction_sets/* instruction_set
	./instruction_set instruction_sets/45GS02.opc

instructionset-4510.tex opcodetable-4510.tex:	instruction_sets/* instruction_set
	./instruction_set instruction_sets/4510.opc instruction_sets/45GS02.opc

instructionset-6502.tex opcodetable-6502.tex:	instruction_sets/* instruction_set
	./instruction_set instruction_sets/6502.opc

#images/illustrations/flashmenu-flowchart.pdf:	images/illustrations/flashmenu-flowchart.dot
#	dot -Tpdf images/illustrations/flashmenu-flowchart.dot  > images/illustrations/flashmenu-flowchart.pdf

# Make sure we update the register information files before typesetting
mega65-userguide.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-userguide.tex

mega65-userguide-lulu.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-userguide-lulu.tex

mega65-chipset-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-chipset-reference.tex

mega65-assembly-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-assembly-reference.tex

mega65-basic65-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic65-reference.tex

#mega65-basic65-programming.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
#	./getgitinfo
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic65-programming.tex

styleguide.pdf: *.tex Makefile references.bib
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make styleguide.tex


sandbox.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make sandbox.tex

registration_code_template.pdf: *.tex $(EXAMPLES) Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make registration_code_template.tex

#wrong.pdf: *.tex $(EXAMPLES) Makefile references.bib
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make wrong.tex

referenceguide.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make referenceguide.tex

mega65-developer-guide.pdf: *.tex $(EXAMPLES) $(HYPPO_EXAMPLES) lstlang0.sty Makefile references.bib document-memory  $(GENERATED_TEX_FILES) $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -f -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-developer-guide.tex

hardwareguide.pdf: *.tex  $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make hardwareguide.tex

mega65-book.pdf: *.tex $(EXAMPLES) $(HYPPO_EXAMPLES) lstlang0.sty Makefile references.bib document-memory $(GENERATED_TEX_FILES) $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-book.tex

mega65-book-cmyk.pdf:	mega65-book.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-book-cmyk.pdf mega65-book.pdf

mega65-userguide-cmyk.pdf:	mega65-userguide.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-userguide-cmyk.pdf mega65-userguide.pdf

mega65-developer-guide-cmyk.pdf:	mega65-developer-guide.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-developer-guide-cmyk.pdf mega65-developer-guide.pdf

mega65-chipset-reference-cmyk.pdf:	mega65-chipset-reference.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-chipset-reference-cmyk.pdf mega65-chipset-reference.pdf

mega65-assembly-reference-cmyk.pdf:	mega65-assembly-reference.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-assembly-reference-cmyk.pdf mega65-assembly-reference.pdf

mega65-basic65-reference-cmyk.pdf:	mega65-basic65-reference.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-basic65-reference-cmyk.pdf mega65-basic65-reference.pdf

mega65-basic-programming-cmyk.pdf:	mega65-basic-programming.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-basic-programming-cmyk.pdf mega65-basic-programming.pdf

document-memory:	document-memory.c Makefile
	$(CC) -Wall -g -o document-memory document-memory.c

index_basic_programmes:	index_basic_programmes.c Makefile
	$(CC) -Wall -g -o index_basic_programmes index_basic_programmes.c

test_basic_programmes:	test_basic_programmes.c Makefile
	$(CC) -Wall -g -o test_basic_programmes test_basic_programmes.c -lssl -lcrypto

appendix-basic65-indexed.tex:	appendix-basic65.tex index_basic_programmes
	./index_basic_programmes appendix-basic65.tex > appendix-basic65-indexed.tex

generate_condensed: generate_condensed.c Makefile
	 $(CC) -Wall -g -o generate_condensed generate_condensed.c

appendix-basic65-condensed.tex: appendix-basic65-indexed.tex generate_condensed
	./generate_condensed

# Use this to build EPS files from SVG diagrams. This requires Inkscape 1.3 or later.
# Please commit both generated EPS and SVG files to the repo. This is currently
# a manual step because we need to upgrade the version of Inkscape on the Jenkins host.
generate-diagrams:
	inkscape --export-overwrite --export-type=eps images/IEC-Timing-Diagrams/*.svg

clean:
	latexmk -CA
	rm -f $(BIBLIOGRAPHIES)

realclean: clean
	rm -f gitinfo.tex
	rm -rf *.dSYM
	rm -rf __pycache__
	rm -f $(GENERATED_TEX_FILES)
	rm -f $(COMPILED_BINARIES)
	rm -f $(LISTING_FILES)
	rm -f $(DOCUMENT_MEMORY_FILES)

format:
	find . -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | xargs clang-format --style=file -i
