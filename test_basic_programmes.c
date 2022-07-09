#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

void parse_basic_text(char* s)
{
  int quote_mode = 0;
  char token[8192];
  int tlen = 0;
  fprintf(stderr,"  %s\n",s);
  
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\"') {
      quote_mode ^= 1;
      tlen = 0;
    }
    if (!quote_mode) {
      if (isalnum(s[i]))
        token[tlen++] = s[i];
      else {
        token[tlen] = 0;
        if (tlen) {
          if (isalpha(token[0])) {
	    //            mark_cmd_used(token);
          }
          // Stop scanning after a REM statement
          if (!strcmp(token, "REM"))
            break;
        }
        tlen = 0;
      }
    }
  }
  if (tlen) {
    token[tlen] = 0;
    if (isalpha(token[0])) {
      //      mark_cmd_used(token);
    }
  }
}

char last_filename[1024];
int prog_num=0;
int start_program(char *filename)
{
  if (strcmp(filename,last_filename)) {
    prog_num=1;
    strcpy(last_filename,filename);
  } else prog_num++;

  fprintf(stderr,"DEBUG: Recording program '%s.%d.bas'\n",
	  filename,prog_num);
  return 0;
}

int end_program(void)
{
  return 0;
}

void parse_tex_file(char *filename)
{
  FILE* f = fopen(filename, "r");

  if (!f) {
    fprintf(stderr, "ERROR: Could not read '%s'\n", filename);
    exit(-1);
  }

  int in_code = 0;
  int code_font = 0;

  char line[8192];

  in_code = 0;
  code_font = 0;

  line[0] = 0;
  fgets(line, 8192, f);
  while (line[0]) {

    if (!strncmp("\\verbatimfont{\\codefont}", line, strlen("\\verbatimfont{\\codefont}"))) {
      code_font = 1;
    }
    else if (!strncmp("\\begin{verbatim}", line, strlen("\\begin{verbatim}"))) {
      fprintf(stdout, "%s", line);
      if (code_font == 1) {
        fprintf(stderr, "BEGIN\n");
        in_code = 1;
	start_program(filename);
      }
    }
    else if (!strncmp("\\begin{screenoutput}", line, strlen("\\begin{screentext}"))) {
        fprintf(stderr, "BEGIN\n");
	in_code=1;
	start_program(filename);
    }
    else if (!strncmp("\\end{screenoutput}", line, strlen("\\end{screentext}"))) {
      fprintf(stderr, "END\n");
      
      if (in_code) end_program();
      
      in_code = 0;

    }
    else if (!strncmp("\\end{verbatim}", line, strlen("\\end{verbatim}"))) {
        fprintf(stderr, "END\n");

      if (in_code) end_program();
      
      in_code = 0;

    }
    else {
      code_font = 0;
      if (in_code) {
        // Parse example program to look for known keywords
        parse_basic_text(line);
      }
    }

    line[0] = 0;
    fgets(line, 8192, f);
  }
}

int main(int argc, char** argv)
{
  for(int f=1;f<argc;f++) parse_tex_file(argv[f]);
}
