.PHONY: userguide.pdf nexys4-setup.pdf all clean

all:	userguide.pdf nexys4-setup.pdf

userguide.pdf: *.tex Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make userguide.tex

nexys4-setup.pdf: *.tex Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make nexys4-setup.tex


clean:
	latexmk -CA
