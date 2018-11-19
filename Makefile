.PHONY: userguide.pdf all clean

all:	userguide.pdf

userguide.pdf: *.tex Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make userguide.tex

clean:
	latexmk -CA
