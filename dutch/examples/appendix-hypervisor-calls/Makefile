ACME = acme
ACME_FLAGS = --format cbm --cpu m65 --color --strict-segments

SOURCES := $(wildcard *.asm)
TARGETS := $(SOURCES:.asm=.prg)

.PHONY: all clean

all: $(TARGETS)

clean:
	-rm -f *.prg *.rpt *.sym

%.prg: %.asm common.inc
	$(ACME) $(ACME_FLAGS) --outfile $@ --report $*.rpt --symbollist $*.sym $<
# 	$(ACME) $(ACME_FLAGS) --outfile $@ $<
