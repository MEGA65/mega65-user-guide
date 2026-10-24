#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Regtabellen lokaliseren voor de MEGA65-chipset naslaggids.

Werkwijze (vergelijkbaar met localize-basic65.py):

1. --extract  : extraheert elke \item{\bf{NAAM}\index{...}} OMSCHRIJVING-regel
   uit de chipset-regtabellen naar dutch/tools/regtables-nl/descriptions.tsv
   (id, bestand, regelnummer, NAAM, bronomschrijving).
2. Vertaling  : vertaler-subagents schrijven per batch (id\\tnl) naar
   dutch/tools/regtables-nl/batch-XX.tsv; samen te voegen tot nl.tsv.
3. --apply    : schrijft dutch/regtable_*.tex met vertaalde omschrijvingen;
   tabelrijen (HEX/DEC/DB7..DB0, \\footnotesize-regels), \\index-tokens en de
   {\bf{NAAM}}\\index{...}}-prefices blijven byte-identiek. Daarnaast worden
   tabelkoppen (Signal/Description) en de "continued"-markeringen vertaald.
4. --verify   : structurele controle — elk niet-omschrijvingsdeel moet
   byte-gelijk zijn aan het bronbestand; aantal onvertaalde items wordt geteld
   (bewust onvertaald = mnemoniek-omschrijvingsrijen mogen Engels blijven).

Python 3.6-compatibel.
"""
from __future__ import print_function
import argparse
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
OUTDIR = os.path.join(HERE, "regtables-nl")

REGTABLES = [
    "regtable_AUDIO.MEGA65.tex",
    "regtable_AUTOIEC.MEGA65.tex",
    "regtable_CIA1.C64.tex",
    "regtable_CIA1.MEGA65.tex",
    "regtable_CIA2.C64.tex",
    "regtable_CIA2.MEGA65.tex",
    "regtable_DMA.C65.tex",
    "regtable_DMA.MEGA65.tex",
    "regtable_ETHCOMMAND.MEGA65.tex",
    "regtable_ETH.MEGA65.tex",
    "regtable_FDC.C65.tex",
    "regtable_FDC.MEGA65.tex",
    "regtable_RTC-GROV.MEGA65.tex",
    "regtable_RTC.MEGA65.tex",
    "regtable_RTC-R4.MEGA65.tex",
    "regtable_SDFDC.MEGA65.tex",
    "regtable_SD.MEGA65.tex",
    "regtable_SID.MEGA65.tex",
    "regtable_TOUCH.MEGA65.tex",
    "regtable_UART.C65.tex",
    "regtable_UARTMISC.MEGA65.tex",
    "regtable_VIC-II.C64.tex",
    "regtable_VIC-III.C65.tex",
    "regtable_VIC-IV.MEGA65.tex",
    "regtable_UUID.MEGA65.tex",
]

# Regel-niveau-vervangingen (tabelkoppen en vervolg-markeringen); de
# kolomnamen HEX/DEC en de bitkolommen DB0..DB7 blijven Engels.
TABLE_NL = [
    ("\\bf{Signal}} & {\\bf{Description}}",
     "\\bf{Signaal}} & {\\bf{Omschrijving}}"),
    ("\\bf{Name}} & {\\bf{Description}}",
     "\\bf{Naam}} & {\\bf{Omschrijving}}"),
    ("\\multicolumn{3}{l@{}}{\\ldots continued}\\\\",
     "\\multicolumn{3}{l@{}}{\\ldots vervolgd}\\\\"),
    ("\\multicolumn{3}{l@{}}{continued \\ldots}\\\\",
     "\\multicolumn{3}{l@{}}{vervolgd \\ldots}\\\\"),
]

# Omschrijvingstekst in Description-kolommen van tabellen zonder itemize
# (regtable_ETHCOMMAND.MEGA65.tex en regtable_UUID.MEGA65.tex).
ROW_NL = {
    "regtable_ETHCOMMAND.MEGA65.tex": [
        ("Immediately stop transmitting the current ethernet frame.  Will cause a partially sent frame to be received, most likely resulting in the loss of that frame.",
         "Stop onmiddellijk met het verzenden van het huidige Ethernet-frame.  De zender stuurt een gedeeltelijk verzonden frame, waardoor dit frame zeer waarschijnlijk verloren gaat."),
        ("Transmit packet", "Zend pakket"),
        ("Disable the effects of RXONLYONE",
         "Schakel de effecten van RXONLYONE uit"),
        ("Select VIC-IV debug stream via ethernet when \\$D6E1.3 is set",
         "Selecteer de VIC-IV-foutopsporingsstroom via Ethernet wanneer \\$D6E1.3 is gezet"),
        ("Select CPU debug stream via ethernet when \\$D6E1.3 is set",
         "Selecteer de CPU-foutopsporingsstroom via Ethernet wanneer \\$D6E1.3 is gezet"),
        ("Receive exactly one ethernet frame only, and keep all signals states (for debugging ethernet sub-system)",
         "Ontvang precies één Ethernet-frame en behoud alle signaaltoestanden (voor het opsporen van fouten in het Ethernet-subsysteem)"),
        ("Select ~1KiB frames for video/cpu debug stream frames (for receivers that do not support MTUs of greater than 2KiB)",
         "Selecteer frames van ~1 KiB voor de video-/CPU-foutopsporingsstroom (voor ontvangers die MTU's groter dan 2 KiB niet ondersteunen)"),
        ("Select ~2KiB frames for video/cpu debug stream frames, for optimal performance.",
         "Selecteer frames van ~2 KiB voor de video-/CPU-foutopsporingsstroom, voor optimale prestaties."),
    ],
    "regtable_UUID.MEGA65.tex": [
        ("64-bit UUID. Can be used to seed ethernet MAC address",
         "64-bits UUID. Kan worden gebruikt om het Ethernet-MAC-adres in te zaaien"),
    ],
    # Vertaalde bit-/itemnamen "Unused" -> "Ongebruikt" (inclusief index-token).
    # Langste patronen eerst: de index-varianten vóór de kale bitnaam, zodat
    # een eerdere vervanging de latere niet blokkeert.
    "regtable_RTC-R4.MEGA65.tex": [
        ("{\\footnotesize Unused\\index{Registers!Unused}}",
         "{\\footnotesize Ongebruikt\\index{Registers!Ongebruikt}}"),
        ("{\\bf{Unused}\\index{Registers!Unused}}",
         "{\\bf{Ongebruikt}\\index{Registers!Ongebruikt}}"),
        ("\\footnotesize Unused", "\\footnotesize Ongebruikt"),
    ],
    "regtable_RTC.MEGA65.tex": [
        ("{\\bf{Unused}\\index{Registers!Unused}}",
         "{\\bf{Ongebruikt}\\index{Registers!Ongebruikt}}"),
        ("\\footnotesize Unused", "\\footnotesize Ongebruikt"),
    ],
}

def item_match(line):
    """Splits een \item-regel in (pref_braces_gesloten, omschrijving).

    Geef (prefix, omschrijving) terug als de regel een item-deskriptie is.
    """
    p = line.find("}} ")
    if p >= 0 and "\\index{" in line[:p]:
        return line[:p + 2], line[p + 3:]
    return None

def load_descriptions():
    """Lees descriptions.tsv => lijst (id, bestand, regelnr, naam, bron)."""
    rows = []
    path = os.path.join(OUTDIR, "descriptions.tsv")
    with open(path, encoding="utf-8") as fh:
        for raw in fh:
            raw = raw.rstrip("\n")
            if not raw:
                continue
            parts = raw.split("\t")
            if len(parts) != 5:
                print("ongeldige regel in descriptions.tsv:", raw[:80])
                sys.exit(1)
            rows.append(tuple(parts))
    return rows

def extract():
    if not os.path.isdir(OUTDIR):
        os.makedirs(OUTDIR)
    rows = []
    for f in REGTABLES:
        with open(f, encoding="utf-8") as fh:
            for lineno, raw in enumerate(fh, 1):
                line = raw.rstrip("\n")
                m = item_match(line)
                if m:
                    prefix, desc = m
                    name = prefix.split("\\bf{", 1)[1].split("}", 1)[0]
                    rows.append((f, lineno, name, desc))
    out = os.path.join(OUTDIR, "descriptions.tsv")
    with open(out, "w", encoding="utf-8") as fh:
        for i, (f, ln, name, desc) in enumerate(rows, 1):
            fh.write("%s\t%s\t%s\t%s\t%s\n" % (i, f, ln, name, desc))
    print("catalogus: %d items -> %s" % (len(rows), out))

def load_nl():
    """Lees alle batch-XX.tsv en nl.tsv => {id: vertaling}."""
    nl = {}
    if os.path.exists(os.path.join(OUTDIR, "nl.tsv")):
        for raw in open(os.path.join(OUTDIR, "nl.tsv"), encoding="utf-8"):
            raw = raw.rstrip("\n")
            if not raw:
                continue
            if "\t" in raw:
                i, _, t = raw.partition("\t")
                nl.setdefault(i, t)
    for f in sorted(os.listdir(OUTDIR)):
        if re.fullmatch(r"nl-batch-\d+\.tsv", f):
            for raw in open(os.path.join(OUTDIR, f), encoding="utf-8"):
                raw = raw.rstrip("\n")
                if not raw or "\t" not in raw:
                    continue
                i, _, t = raw.partition("\t")
                nl.setdefault(i, t)
    return nl

def apply_step(missing_ok):
    rows = load_descriptions()
    nl = load_nl()
    by_file = {}
    for (i, f, ln, name, desc) in rows:
        by_file.setdefault(f, []).append((int(i), int(ln), desc, nl.get(i)))
    missing = 0
    for f, items in sorted(by_file.items()):
        trans = dict((ln, nl) for (i, ln, desc, nl) in items if nl)
        out = []
        with open(f, encoding="utf-8") as fh:
            for lineno, raw in enumerate(fh, 1):
                line = raw.rstrip("\n")
                t = trans.get(lineno)
                if t is not None:
                    prefix, _ = item_match(line)
                    line = prefix + " " + t
                for a, b in TABLE_NL:
                    line = line.replace(a, b)
                for a, b in ROW_NL.get(f, []):
                    if a in line:
                        line = line.replace(a, b)
                out.append(line)
        tail = ""
        with open(f, encoding="utf-8") as fh:
            src = fh.read()
        if src.endswith("\n"):
            tail = "\n"
        dst = os.path.normpath(os.path.join(HERE, "..", f))
        with open(dst, "w", encoding="utf-8") as fh:
            for line in out:
                fh.write(line + tail)
        print("geschreven:", dst)
    for f in REGTABLES:
        if f in by_file:
            continue
        out = []
        with open(f, encoding="utf-8") as fh:
            for lineno, raw in enumerate(fh, 1):
                line = raw.rstrip("\n")
                for a, b in TABLE_NL:
                    line = line.replace(a, b)
                for a, b in ROW_NL.get(f, []):
                    if a in line:
                        line = line.replace(a, b)
                out.append(line)
        dst = os.path.normpath(os.path.join(HERE, "..", f))
        with open(dst, "w", encoding="utf-8") as fh:
            fh.write("\n".join(out) + "\n")
        print("geschreven:", dst)
    ids_no_nl = [i for (i, f, ln, name, desc) in rows if not nl.get(i)]
    print("items: %d totaal, %d onvertaald" % (len(rows), len(ids_no_nl)))
    if ids_no_nl and not missing_ok:
        print("let op: onvertaalde items aanwezig (--apply mist --missing-ok niet)")
        return 1
    return 0

def verify():
    rows = load_descriptions()
    nl = load_nl()
    by_file = {}
    for (i, f, ln, name, desc) in rows:
        by_file.setdefault(f, []).append((int(i), int(ln), desc, nl.get(i), name))
    problems = []
    missing = []
    for f, items in sorted(by_file.items()):
        dst = os.path.normpath(os.path.join(HERE, "..", f))
        trans = dict((ln, nl) for (i, ln, desc, nl, name) in items if nl)
        if not os.path.exists(dst):
            problems.append("ontbrekende vertaalde regtabel: " + dst)
            continue
        with open(f, encoding="utf-8") as fh:
            src_lines = [l.rstrip("\n") for l in fh]
        with open(dst, encoding="utf-8") as fh:
            dst_lines = [l.rstrip("\n") for l in fh]
        if len(src_lines) != len(dst_lines):
            problems.append("%s: regel-aantal verschilt (bron %d, nl %d)"
                            % (f, len(src_lines), len(dst_lines)))
            continue
        for lineno, (s, d) in enumerate(zip(src_lines, dst_lines), 1):
            expected = s
            for a, b in TABLE_NL:
                expected = expected.replace(a, b)
            for a, b in ROW_NL.get(f, []):
                if a in expected:
                    expected = expected.replace(a, b)
            m = item_match(s)
            if m and lineno in trans:
                d_exp = m[0] + " " + trans[lineno]
                for a, b in TABLE_NL:
                    d_exp = d_exp.replace(a, b)
                for a, b in ROW_NL.get(f, []):
                    if a in d_exp:
                        d_exp = d_exp.replace(a, b)
                if d != d_exp:
                    problems.append("%s:%d: omschrijving verschilt" % (f, lineno))
            else:
                if expected != d:
                    problems.append("%s:%d: structuur verschilt\n  bron: %s\n  nl  : %s"
                                    % (f, lineno, expected[:120], d[:120]))
        for (i, ln, desc, nl_, name) in items:
            if not nl_:
                missing.append("%s:%d %s" % (f, ln, name))
    for p in problems:
        print("PROBLEEM:", p)
    print("structurele controle: %s" %
          ("OK" if not problems else "%d problemen" % len(problems)))
    print("onvertaalde omschrijvingen: %d" % len(missing))
    for m in missing:
        print("  onvertaald:", m)
    return 1 if problems else 0

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--extract", action="store_true")
    g.add_argument("--apply", action="store_true")
    g.add_argument("--verify", action="store_true")
    ap.add_argument("--missing-ok", action="store_true",
                    help="sta onvertaalde omschrijvingen toe")
    a = ap.parse_args()
    if a.extract:
        extract()
    elif a.apply:
        sys.exit(apply_step(a.missing_ok))
    else:
        sys.exit(verify())

if __name__ == "__main__":
    main()