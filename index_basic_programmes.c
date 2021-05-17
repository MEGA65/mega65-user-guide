#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

int main(int argc,char **argv)
{
  FILE *f=fopen(argv[1],"r");

  if (!f) {
    fprintf(stderr,"ERROR: Could not read '%s'\n",argv[1]);
    exit(-1);
  }

  int in_code=0;
  int code_font=0;
  
  char line[8192];
  line[0]=0; fgets(line,8192,f);
  while(line[0]) {
    if (!strncmp("\\verbatimfont{\\codefont}",line,strlen("\\verbatimfont{\\codefont}"))) code_font=1;
    else if (!strncmp("\\begin{verbatim}",line,strlen("\\begin{verbatim}"))) {
      if (code_font==1) in_code=1;
    } else if (!strncmp("\\end{verbatim}",line,strlen("\\end{verbatim}"))) {
      in_code=0;
    } else {
      code_font=0;
      if (in_code) {
	fprintf(stdout,"%s",line);
      }
    }
    
    line[0]=0; fgets(line,8192,f);
  }

  fclose(f);
}
