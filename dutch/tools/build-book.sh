#!/bin/sh
# Bouw een deelboek als volledige PDF. De masters dutch/<boek>.tex zijn
# bronbestand (de vertaling is bevoren; de segmentpipeline is gearchiveerd
# in dutch/attic/). Bewerken gebeurt direct in de dutch/*.tex-bestanden.
# Gebruik: build-book.sh <boek> [dutch|src]
#   bijv.  ./build-book.sh mega65-userguide src
#   bijv.  ./build-book.sh mega65-userguide dutch
set -e

BOOK="${1:-mega65-userguide}"
MODE="${2:-dutch}"

ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"

# Het bibliografiestijlbestand staat in dutch/, niet in de repo-root; de
# afsluitende ':' houdt het standaardzoekpad (IEEEtran.bst, references.bib).
export BSTINPUTS="$ROOT/dutch:"

if [ "$MODE" = "src" ]; then
    # Engelse bron: eigen jobname, de uitvoer blijft in de repo-root.
    WRAPPER="dutch/build/${BOOK}-build.tex"
    printf '\\def\\printmanual{true}\n\\input{%s}\n\\end{document}\n' "$BOOK" > "$WRAPPER"
    OUTDIR=""
    LOGSRC="${BOOK}-build.log"
else
    # Vertaling: dezelfde jobname en uitvoermap als de Makefile-route, zodat
    # er per taal één aux is (dutch/mega65-book.aux) en de twee routes
    # elkaars buildgegevens niet overschrijven. Die aux bepaalt de titels
    # en paginanummers in de kruisverwijzingen van de deelboeken.
    WRAPPER="dutch/build/${BOOK}.tex"
    printf '\\input{dutch/%s}\n\\end{document}\n' "$BOOK" > "$WRAPPER"
    OUTDIR="-outdir=dutch"
    LOGSRC="dutch/${BOOK}.log"
fi

echo "==> bouwen van ${BOOK} (${MODE})"
latexmk -pdf $OUTDIR -pdflatex="xelatex -interaction=nonstopmode" \
    "$WRAPPER" 2>&1 | tail -n 25

LOG="dutch/build/${BOOK}-build.log"
if [ -f "$LOGSRC" ]; then
    mv -f "$LOGSRC" "$LOG" 2>/dev/null || true
fi
echo "==> log: $LOG"

# "^! " met spatie: echte TeX-fouten beginnen met "! <boodschap>"; een losse
# "!" kan ontstaan uit omgebroken "Missing character"-regels (U+000A in font).
# "No pages of output": xelatex is voortijdig gestopt (bijv. gedood) zonder
# foutmelding — dit valt anders door de maas en geeft een valse "build OK".
FOUTPATROON="^! [A-Za-z]|Emergency stop|Undefined control sequence|No pages of output"
if grep -q -E "$FOUTPATROON" "$LOG" 2>/dev/null; then
    echo "==> FOUTEN gevonden in de build."
    grep -n -E "$FOUTPATROON" "$LOG" | head -n 20
    exit 1
fi
echo "==> build OK"
