CC=gcc

BOOKS=	mega65-book.pdf \
	mega65-userguide.pdf \
	mega65-developer-guide.pdf \
	mega65-chipset-reference.pdf \
	mega65-basic10-reference.pdf \
	mega65-basic-programming.pdf \

GENERATED_TEX_FILES= 	document-memory \
			instructionset-4510.tex \
		     	instructionset-6502.tex \
		     	instructionset-45GS02.tex \
		 	api-conio.tex \
			images/illustrations/screen-40x25-addresses16-80.pdf


.PHONY: $(BOOKS) all clean

all:	$(BOOKS)

books:	$(BOOKS)

screen-maps:	screen-maps.c Makefile
	$(CC) -Wall -o screen-maps screen-maps.c -lhpdf

images/illustrations/screen-40x25-addresses16-80.pdf:	screen-maps
	./screen-maps

prg2tex:	prg2tex.c
	$(CC) -Wall -o prg2tex prg2tex.c

libc-doc:	libc-doc.c
	$(CC) -g -Wall -o libc-doc libc-doc.c

EXAMPLEDIR=	examples
EXAMPLES=	$(EXAMPLEDIR)/ledcycle.tex

%.tex:	%.prg prg2tex Makefile
	./prg2tex -u $<

api-conio.tex:	libc-doc ../mega65-libc/cc65/include/conio.h
	./libc-doc ../mega65-libc/cc65/include/conio.h > api-conio.tex

instruction_set: instruction_set.c Makefile
	$(CC) -Wall -g -o instruction_set instruction_set.c

instructionset-45GS02.tex:	instruction_set
	./instruction_set instruction_sets/45GS02.opc > instructionset-45GS02.tex

instructionset-4510.tex:	instruction_set
	./instruction_set instruction_sets/4510.opc > instructionset-4510.tex

instructionset-6502.tex:	instruction_set
	./instruction_set instruction_sets/6502.opc > instructionset-6502.tex

#images/illustrations/flashmenu-flowchart.pdf:	images/illustrations/flashmenu-flowchart.dot
#	dot -Tpdf images/illustrations/flashmenu-flowchart.dot  > images/illustrations/flashmenu-flowchart.pdf

# Make sure we update the register information files before typesetting
mega65-userguide.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-userguide.tex

mega65-chipset-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-chipset-reference.tex

mega65-basic10-reference.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic10-reference.tex

#mega65-basic10-programming.pdf: *.tex $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
#	./getgitinfo
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-basic10-programming.tex


sandbox.pdf: *.tex $(EXAMPLES) Makefile references.bib
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make sandbox.tex

#wrong.pdf: *.tex $(EXAMPLES) Makefile references.bib
#	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make wrong.tex

referenceguide.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl  $(GENERATED_TEX_FILES)
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make referenceguide.tex

mega65-developer-guide.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory  $(GENERATED_TEX_FILES) ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -f -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-developer-guide.tex

hardwareguide.pdf: *.tex  $(EXAMPLES) Makefile references.bib  $(GENERATED_TEX_FILES)
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make hardwareguide.tex

mega65-book.pdf: *.tex $(EXAMPLES) Makefile references.bib document-memory $(GENERATED_TEX_FILES) ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-book.tex

mega65-book-cmyk.pdf:	mega65-book.pdf
	gs -dSAFER -dBATCH -dNOPAUSE -dNOCACHE -sDEVICE=pdfwrite -sColorConversionStrategy=CMYK -dProcessColorModel=/DeviceCMYK -sOutputFile=mega65-book-cmyk.pdf mega65-book.pdf

document-memory:	document-memory.c Makefile
	$(CC) -Wall -g -o document-memory document-memory.c

clean:
	latexmk -CATED
