#include <stdio.h>
#include <string.h>

#define MAX_FILE_SIZE (4*1024*1024)
char buff[MAX_FILE_SIZE];
char token[MAX_FILE_SIZE];

void parse_token(char **s,char *t)
{
  if (**s!='{') {
    fprintf(stderr,"ERROR: Missing {\n");
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
    printf("Function '%s'\n",token);
    
    // Find next instance
    s=strstr(s,"\\m65libsummary");
  }
  
}


