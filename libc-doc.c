#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_SIZE (4*1024*1024)
char buff[MAX_FILE_SIZE];
char token[MAX_FILE_SIZE];

void parse_token(char **s,char *t)
{
  while(**s==' ') (*s)++;
  while(**s=='\t') (*s)++;
  
  if (**s!='{') {
    (*s)[32]=0;
    fprintf(stderr,"ERROR: Missing { at '%s...'\n",*s);
    exit(-1);
  }
  (*s)++;

  int tl=0;
  while(**s) {
    if ((**s)=='}') {
      (*s)++;
      t[tl]=0;
      return;
    }
    else {
      t[tl++]=**s;
      (*s)++;
    }
  }
}

int main(int argc,char **argv)
{
  FILE *f=fopen(argv[1],"r");
  int len=fread(buff,1,MAX_FILE_SIZE,f);
  fprintf(stderr,"Read %d bytes.\n",len);
  buff[len]=0;
  
  char *s=strstr(buff,"\\m65libsummary");
  while (s) {
    s+=strlen("\\m65libsummary");

    // Skip spaces
    while(s[0]==' ') s++;
    parse_token(&s,token);
    printf("\\subsection{%s}\n",token);
    printf("\\index{%s}\n",token);
    parse_token(&s,token);
    printf("\\begin{description}[leftmargin=2cm,style=nextline]\n"
	   "\\item [Desription:] {%s}\n",token);
    while(strncmp(s,"\\m65libsummary",strlen("\\m65libsummary"))) {
      s=strstr(s,"\\m65lib");
      if (!s) break;
      if (!strncmp(s,"\\m65libparam",strlen("\\m65libparam"))) {
	s+=strlen("\\m65libparam");
	parse_token(&s,token);
	printf("\\item [%s:]",token);
	parse_token(&s,token);
	printf(" {%s}\n",token);
      } else if (!strncmp(s,"\\m65libretval",strlen("\\m65libretval"))) {
	s+=strlen("\\m65libretval");
      } else if (!strncmp(s,"\\m65libsyntax",strlen("\\m65libsyntax"))) {
	s+=strlen("\\m65libsyntax");
      } else if (!strncmp(s,"\\m65libremarks",strlen("\\m65libremarks"))) {
	s+=strlen("\\m65libremarks");
      } else if (!strncmp(s,"\\m65libsummary",strlen("\\m65libsummary"))) {
	s+=strlen("\\m65libsummary");
      } else {
	s[32]=0;
	fprintf(stderr,"Unknown block type '%s...'\n",s);
	exit(-1);
      }
    }
    
    // Find next instance
    if (s)
      s=strstr(s,"\\m65libsummary");
  }
  
}


