#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

FILE* rp;
FILE* wp;

char Line[60000];

int main(int argc, char* argv[])
{
  rp = fopen("appendix-basic65-indexed.tex", "r");
  if (!rp) {
    fprintf(stderr, "*** Could not open appendix-basic65-indexed.tex\n");
    exit(1);
  }

  wp = fopen("appendix-basic65-condensed.tex", "w");
  if (!wp) {
    fprintf(stderr, "*** Could not open appendix-basic65-condensed.tex\n");
    exit(1);
  }

  while (!feof(rp)) {
    fgets(Line, sizeof(Line), rp);
    if (!strncmp(Line, "\\newpage", 8))
      continue;
    if (!strncmp(Line, "\\item [Token:", 13))
      continue;
    fputs(Line, wp);
  }

  return 0;
}
