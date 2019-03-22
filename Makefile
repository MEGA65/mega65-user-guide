.PHONY: userguide.pdf nexys4-setup.pdf all clean

all:	userguide.pdf nexys4-setup.pdf

# Make sure we update the register information files before typesetting
userguide.pdf: *.tex Makefile references.bib document-memory ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	./document-memory ../mega65-core/src/vhdl/*.vhdl ../mega65-core/src/vhdl/*/*.vhdl
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make userguide.tex

nexys4-setup.pdf: *.tex Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make nexys4-setup.tex

document-memory:	document-memory.c Makefile
	gcc -Wall -g -o document-memory document-memory.c

clean:
	latexmk -CA
