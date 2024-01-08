#!/bin/bash

grep -B 999 "%% MAILMERGE_BEGIN_REPEATED" registration_code_template.tex > mm_top.tex
grep -A 999 "%% MAILMERGE_END_REPEATED"   registration_code_template.tex > mm_bottom.tex
grep -B 999 "%% MAILMERGE_END_REPEATED"   registration_code_template.tex | grep -A 999 "%% MAILMERGE_BEGIN_REPEATED" > mm_repeated.tex

cat mm_top.tex > mm.tex
while read p; do
    echo "$p"
    cat mm_repeated.tex | sed "s/CODEGOESHERE/${p}/g" >> mm.tex
done <codes.txt
cat mm_bottom.tex >> mm.tex

latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" -use-make mm.tex

echo "Output will be written to mm.pdf"

gs \
 -sOutputFile=mm-greyscale.pdf \
 -sDEVICE=pdfwrite \
 -sColorConversionStrategy=Gray \
 -dProcessColorModel=/DeviceGray \
 -dCompatibilityLevel=1.4 \
 -dNOPAUSE \
 -dBATCH \
 mm.pdf

echo "Greyscale version written to mm-greyscale.pdf"
