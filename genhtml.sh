#!/bin/bash
echo "<html><h1>MEGA65 Documentation</h1>" > index.html
echo '<p><u>Last updated at</u> <b>' >> index.html
date >> index.html
echo "</b></p><pre>" >> index.html
git log -1 >> index.html
echo "</pre><ul>" >> index.html

echo "</ul><p><b><u>PDF files</u></b>:</p><ul>" >> index.html
PDFS=`ls -S *.pdf`

for file in $PDFS; do
  LOG=`basename -s .pdf $file`.log
  PAGES=`tail -1 $LOG | sed 's/.*\((.*)\).*/\1/'`
  echo "<li><a href=\"$file\">$file</a> $PAGES</li>" >> index.html
done

echo "</ul><p><b><u>Log files</u></b>:</p><ul>" >> index.html
LOGS=`ls -S *.log`

for file in $LOGS; do
  echo "file=$file"
  SIZE=`ls -lh $file | cut -d' ' -f 6`
  echo "<li><a href=\"$file\">$file</a> ($SIZE)</li>" >> index.html
done

echo "</ul></html>" >> index.html
cat index.html
