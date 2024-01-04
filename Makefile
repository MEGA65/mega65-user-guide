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

GENERATED_TEX_FILES= 	document-memory \
			instructionset-4510.tex \
		     	instructionset-6502.tex \
		     	instructionset-45GS02.tex \
			opcodetable-6502.tex \
			opcodetable-4510.tex \
		 	api-conio.tex \
			appendix-basic65-indexed.tex \
			appendix-basic65-condensed.tex \
			examples/ledcycle.tex \
			examples/ledcycle.txt \
			keymap_table_1.tex \
			keymap_table_2.tex \
			keymap_table_3.tex \
			keymap_table_4.tex \
			keymap_table_5.tex \
			unicode_mapping.tex
			#images/illustrations/screen-40x25-addresses16-80.pdf

COMPILED_BINARIES= 	document-memory \
			generate_condensed \
			keymap \
			index_basic_programmes \
			test_basic_programmes \
			instruction_set \
			libc-doc \
			prg2tex

.PHONY: $(BOOKS) all clean

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
EXAMPLES=	$(EXAMPLEDIR)/ledcycle.tex
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

mega65-chipset-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	inkscape --export-type=eps Images/IEC-Timing-Diagrams/*.svg
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-chipset-reference.tex

mega65-assembly-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-assembly-reference.tex

mega65-basic65-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic65-reference.tex

#mega65-basic65-programming.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
#	./getgitinfo
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic65-programming.tex


sandbox.pdf: *.tex $(EXAMPLES) Makefile references.bib
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	inkscape --export-type=eps Images/IEC-Timing-Diagrams/*.svg
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make sandbox.tex

#wrong.pdf: *.tex $(EXAMPLES) Makefile references.bib
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make wrong.tex

referenceguide.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make referenceguide.tex

mega65-developer-guide.pdf: *.tex $(EXAMPLES) $(HYPPO_EXAMPLES) lstlang0.sty Makefile references.bib document-memory  $(GENERATED_TEX_FILES) $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	inkscape --export-type=eps Images/IEC-Timing-Diagrams/*.svg
	latexmk -f -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-developer-guide.tex

hardwareguide.pdf: *.tex  $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make hardwareguide.tex

mega65-book.pdf: *.tex $(EXAMPLES) $(HYPPO_EXAMPLES) lstlang0.sty Makefile references.bib document-memory $(GENERATED_TEX_FILES) $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q $(REPOPATH)/mega65-core/src/vhdl/*.vhdl $(REPOPATH)/mega65-core/src/vhdl/*/*.vhdl
	inkscape --export-type=eps Images/IEC-Timing-Diagrams/*.svg
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

clean:
	latexmk -CA

realclean: clean
	rm -f gitinfo.tex
	rm -f $(GENERATED_TEX_FILES)
	rm -f $(COMPILED_BINARIES)

format:
	find . -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' | xargs clang-format --style=file -i
