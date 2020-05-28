.PHONY: sandbox.pdf userguide.pdf referenceguide.pdf hardwareguide.pdf mega65-book.pdf all clean

all:	userguide.pdf referenceguide.pdf hardwareguide.pdf mega65-book.pdf

instruction_set: instruction_set.c Makefile
	gcc -Wall -g -o instruction_set instruction_set.c 

instructionset-4510.tex:	instruction_set
	./instruction_set instruction_sets/4510.opc > instructionset-4510.tex

instructionset-6502.tex:	instruction_set
	./instruction_set instruction_sets/6502.opc > instructionset-6502.tex

images/illustrations/flashmenu-flowchart.pdf:	images/illustrations/flashmenu-flowchart.dot
	dot -Tpdf images/illustrations/flashmenu-flowchart.dot  > images/illustrations/flashmenu-flowchart.pdf

# Make sure we update the register information files before typesetting
userguide.pdf: *.tex Makefile references.bib
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make userguide.tex

sandbox.pdf: *.tex Makefile references.bib
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make sandbox.tex


referenceguide.pdf: *.tex Makefile references.bib document-memory ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make referenceguide.tex

hardwareguide.pdf: *.tex Makefile references.bib
	./getgitinfo
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make hardwareguide.tex

mega65-book.pdf: *.tex Makefile references.bib document-memory instructionset-4510.tex ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	./getgitinfo
	./document-memory -q ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mega65-book.tex


document-memory:	document-memory.c Makefile
	gcc -Wall -g -o document-memory document-memory.c

clean:
	latexmk -CA
