.PHONY: userguide.pdf all clean

all:	userguide.pdf

userguide.pdf: userguide.tex *.tex Makefile references.bib
	latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make userguide.tex

clean:
	latexmk -CA
