#!/bin/sh
# Bouw een (vertaald) hoofdstuk als zelfstandige PDF.
# Gebruik: build-chapter.sh <hoofdstuk-naam> <bron-of-dutch>
#   bijv.  ./build-chapter.sh beginninginbasic src
#   bijv.  ./build-chapter.sh beginninginbasic dutch
set -e

CHAPTER="$1"
MODE="${2:-dutch}"

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

# Zie dutch/tools/build-book.sh: het bibliografiestijlbestand staat in dutch/.
export BSTINPUTS="$ROOT/dutch:"

if [ "$MODE" = "src" ]; then
    HEADER="common-header"
    INPUT="$CHAPTER"
else
    HEADER="dutch/common-header-dutch"
    INPUT="dutch/$CHAPTER"
fi

WRAPPER="dutch/build/${CHAPTER}-build.tex"
EXTRA=""
if [ -f "basic-fragments.tex" ] && grep -q 'filenamedefinition\|drivedefinition\|unitdefinition' "$ROOT/dutch/$CHAPTER.tex" 2>/dev/null; then
    if [ "$MODE" = "dutch" ]; then
        EXTRA="\\input{dutch/basic-fragments-nl}"
    else
        EXTRA="\\input{basic-fragments}"
    fi
fi
cat > "$WRAPPER" <<EOF
\\input{${HEADER}}
${EXTRA}
\\input{${INPUT}}
\\end{document}
EOF

echo "==> bouwen van ${CHAPTER} (${MODE})"
latexmk -pdf -pdflatex="xelatex -interaction=nonstopmode" \
    "$WRAPPER" 2>&1 | tail -n 25

LOG="dutch/build/${CHAPTER}-build.log"
if [ -f "${CHAPTER}-build.log" ]; then
    mv -f "${CHAPTER}-build.log" "$LOG" 2>/dev/null || true
fi
echo "==> log: $LOG"

# "^! " met spatie: echte TeX-fouten beginnen met "! <boodschap>"; een losse
# "!" kan ontstaan uit omgebroken "Missing character"-regels (U+000A in font).
if grep -q -E "^! [A-Za-z]|Emergency stop|Undefined control sequence" "$LOG" 2>/dev/null; then
    echo "==> FOUTEN gevonden in de build."
    grep -n -E "^! [A-Za-z]|Emergency stop|Undefined control sequence" "$LOG" | head -n 20
    exit 1
fi
echo "==> build OK"
