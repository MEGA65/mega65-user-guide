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
  echo "file=$file"
  LOG=`basename -s .pdf $file`.log
  echo LOG=$LOG
  PAGES=`tail -1 $LOG | sed 's/.*\((.*)\).*/\1/'`
  echo "<li><a href=\"$file\">$file</a> $PAGES</li>" >> index.html
done

echo "</ul><p><b><u>Log files</u></b>:</p><ul>" >> index.html
ls -1S *.log | sed 's:\(.*\):<li><a href="\1">\1</a></li>:' >> index.html
echo "</ul></html>" >> index.html
cat index.html
