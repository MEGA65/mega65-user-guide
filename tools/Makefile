CC=gcc
CFLAGS=
PYTHON=python3

COMPILED_BINARIES = \
	petcat

petcat: petcat.c
	$(CC) -g -Wall -o $@ $< $(CFLAGS)

.PHONY: clean all

all: $(COMPILED_BINARIES)

clean:
	rm -f $(COMPILED_BINARIES)
	rm -rf *.dSYM
