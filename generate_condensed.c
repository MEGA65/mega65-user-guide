#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

FILE* rp;
FILE* wp;

char Line[60000];

int skip(const char *text)
{
   size_t len;

   len = strlen(text);
   return (!strncmp(Line,text,len));
}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input.tex> <output.tex>\n", argv[0]);
    exit(1);
  }

  rp = fopen(argv[1], "r");
  if (!rp) {
    fprintf(stderr, "*** Could not open %s\n", argv[1]);
    exit(1);
  }

  wp = fopen(argv[2], "w");
  if (!wp) {
    fprintf(stderr, "*** Could not open %s\n", argv[2]);
    exit(1);
  }

  while (!feof(rp)) {
    fgets(Line, sizeof(Line), rp);
    if (skip("\\newpage"))
      continue;
    if (skip("\\item [Token:"))
      continue;
    if (skip("\\input{appendix-keywords"))
      continue;
    if (skip("\\input{dutch/appendix-keywords"))
      continue;
    fputs(Line, wp);
  }

  return 0;
}
