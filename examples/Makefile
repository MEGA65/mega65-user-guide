PETCAT=../tools/petcat
PETCAT_ARGS=-nh -65

BASIC_EXAMPLES = \
	iec-data-logger.bas \
	iec-data-logger-2.bas \
	ledcycle.bas

$(PETCAT):
	make -C ../tools petcat

%.bas: %.prg $(PETCAT)
	$(PETCAT) $(PETCAT_ARGS) $< >$@

.PHONY: all clean

all: $(BASIC_EXAMPLES)

clean:
	rm -f $(BASIC_EXAMPLES)
