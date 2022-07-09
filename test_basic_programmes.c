#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include <openssl/sha.h>

#define MAX_PROG_LINES 1024
char prog_lines[MAX_PROG_LINES][1024];
int prog_line_count=0;

int basic_hints=0;
int asm_hints=0;
int c_hints=0;

void parse_basic_text(char* s)
{
  if (prog_line_count<MAX_PROG_LINES) {
    strcpy(prog_lines[prog_line_count++],s);
  } else {
    if (prog_line_count==MAX_PROG_LINES) {
      fprintf(stderr,"ERROR: Program listing looks too long. Missing \\end{screenoutput}, perhaps?\n");
      fprintf(stderr,"First few lines of program are:\n");
      for(int i=0;i<10;i++)
	fprintf(stderr,"  %s\n",s);
    }
  }
  prog_line_count++;

  if (atoi(s)>0) basic_hints++;
  if (strstr(s,"POKE")) basic_hints++;
  if (strstr(s,"DIM")) basic_hints++;
  if (strstr(s,"PEEK")) basic_hints++;
  if (strstr(s,"PRINT")) basic_hints++;
  if (strstr(s,"PRINT USING")) basic_hints+=10;
  if (strstr(s,"REM")) basic_hints++;
  if (strstr(s,"RUN")) basic_hints++;
  if (strstr(s,"IF")) basic_hints++;
  if (strstr(s,"THEN")) basic_hints++;
  if (strstr(s,"GOTO")) basic_hints++;
  if (strstr(s,"GOSUB")) basic_hints++;
  if (strstr(s,"BANK")) basic_hints++;
  if (strstr(s,"$=")) basic_hints++;
  if (strstr(s,"?")) basic_hints++;
  if (strstr(s,"<>")) basic_hints++;
  if (strstr(s,"LOAD")) basic_hints++;
  if (strstr(s,"SAVE")) basic_hints++;
  if (strstr(s,"BOOT")) basic_hints++;
  if (strstr(s,"BOX")) basic_hints++;
  if (strstr(s,"FOR")) basic_hints++;
  if (strstr(s,"NEXT")) basic_hints++;
  if (strstr(s,"CATALOG")) basic_hints++;
  if (strstr(s,"DIR")) basic_hints++;
  if (strstr(s,"MONITOR")) basic_hints++;
  if (strstr(s,",")&&(!strstr(s,"("))&&(!strstr(s,")"))&&(!strstr(s,";"))) basic_hints++;
  if (strstr(s,"?SYNTAX ERROR")) basic_hints++;
  if (strstr(s,"READY.")) basic_hints++;
  if (strstr(s,", U")) basic_hints++;
  if (strstr(s,",U")) basic_hints++;
  if (strstr(s,", D")) basic_hints++;
  if (strstr(s,",D")) basic_hints++;
  if (strstr(s,"LET")) basic_hints++;
  if (strstr(s,"$ =")) basic_hints++;
  if (strstr(s,"% = ")) basic_hints++;
  if (strstr(s,"DELETE")) basic_hints++;
  if (strstr(s,"A$")) basic_hints++;
  if (strstr(s,"INPUT")) basic_hints++;
  if (strstr(s,"SYS")) basic_hints++;
  if (strstr(s,"DEF FN")) basic_hints++;
  if (strstr(s,"USR(")) basic_hints++;

  if (strstr(s,"LDA")) asm_hints++;
  if (strstr(s,"LDX")) asm_hints++;
  if (strstr(s,"LDY")) asm_hints++;
  if (strstr(s,"LDZ")) asm_hints++;
  if (strstr(s,"STA")) asm_hints++;
  if (strstr(s,"STX")) asm_hints++;
  if (strstr(s,"STY")) asm_hints++;
  if (strstr(s,"STZ")) asm_hints++;
  if (strstr(s,"EOM")) asm_hints++;
  if (strstr(s,"NOP")) asm_hints++;
  if (strstr(s,"CLC")) asm_hints++;
  if (strstr(s,"ADC")) asm_hints++;
  if (strstr(s,"JMP")) asm_hints++;
  if (strstr(s,"JSR")) asm_hints++;
  if (strstr(s,"BIT")) asm_hints++;
  if (strstr(s,"BMI")) asm_hints++;
  if (strstr(s,"BPL")) asm_hints++;
  if (strstr(s,"BCC")) asm_hints++;
  if (strstr(s,"BCS")) asm_hints++;
  if (strstr(s,"BNE")) asm_hints++;
  if (strstr(s,"BEQ")) asm_hints++;
  if (strstr(s,"BVC")) asm_hints++;
  if (strstr(s,"SEI")) asm_hints++;
  if (strstr(s,"CLI")) asm_hints++;
  if (strstr(s,"TAX")) asm_hints++;
  if (strstr(s,"TAY")) asm_hints++;
  if (strstr(s,"TAZ")) asm_hints++;
  if (strstr(s,"TXA")) asm_hints++;
  if (strstr(s,"TYA")) asm_hints++;
  if (strstr(s,"TZA")) asm_hints++;
  if (strstr(s,"LDQ")) asm_hints++;
  if (strstr(s,"STQ")) asm_hints++;
  if (strstr(s,"[")&&strstr(s,"]")) asm_hints++;
  if (strstr(s,"#")&&(!strstr(s,","))) asm_hints++;
  if (strstr(s,"#$")) asm_hints++;
  if (strstr(s,".segmentdef")) asm_hints++;
  if (strstr(s,",X)")) asm_hints++;
  if (strstr(s,"),Y")) asm_hints++;
  if (strstr(s,"),Z")) asm_hints++;
  if (strstr(s,"],Z")) c_hints++;
  if (strstr(s,",Y")) asm_hints++;
  if (strstr(s,",Z")) asm_hints++;
  if (strstr(s,"{")) c_hints++;
  if (strstr(s,"}")) c_hints++;
  if (strstr(s,"//")) c_hints++;
  if (strstr(s,"[$")) c_hints++;
  if (strstr(s,"/*")) c_hints++;
  if (strstr(s,"struct")) c_hints++;
  if (strstr(s,"char*")) c_hints++;
  if (strstr(s,"int*")) c_hints++;
  
  
}

char last_filename[1024];
int prog_num=0;
int start_program(char *filename)
{
  prog_line_count=0;
  
  if (strcmp(filename,last_filename)) {
    prog_num=1;
    strcpy(last_filename,filename);
  } else prog_num++;

  //  fprintf(stderr,"DEBUG: Recording program '%s.%d.bas'\n",
  //	  filename,prog_num);
  return 0;
}

int end_program(void)
{
  if (prog_line_count<MAX_PROG_LINES) {

    // Ignore listings that are unambiguously classified for the moment
    if (basic_hints+asm_hints+c_hints) {
      if ((basic_hints+asm_hints==0)||
	  (basic_hints+c_hints==0)||
	  (asm_hints+c_hints==0))
	{
	  basic_hints=0;
	  asm_hints=0;
	  c_hints=0;
	  return 0;
	}
    }

    if (basic_hints>asm_hints&&basic_hints>c_hints) {
      fprintf(stderr,"\nCLASSIFICATION: %d BASIC, %d ASM, %d C\n",
	      basic_hints,asm_hints,c_hints);
      fprintf(stderr,"LISTING:\n");
      SHA512_CTX ctx;
      SHA512_Init(&ctx);     
      for(int i=0;i<prog_line_count;i++) {
	fprintf(stderr,"  %s",prog_lines[i]);
	SHA512_Update(&ctx,prog_lines[i],strlen(prog_lines[i]));
      }
      fprintf(stderr,"\n");
      unsigned char hash[256];
      SHA512_Final(hash,&ctx);
      fprintf(stderr,"HASH:");
      for(int i=0;i<16;i++) fprintf(stderr,"%02x",hash[i]);
      fprintf(stderr,"\n");
    }
  }

  basic_hints=0;
  asm_hints=0;
  c_hints=0;
  prog_line_count=0;
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
      if (code_font == 1) {
	//        fprintf(stderr, "BEGIN\n");
        in_code = 1;
	start_program(filename);
      }
    }
    else if (!strncmp("\\begin{screenoutput}", line, strlen("\\begin{screentext}"))) {
      //        fprintf(stderr, "BEGIN\n");
	in_code=1;
	start_program(filename);
    }
    else if (!strncmp("\\end{screenoutput}", line, strlen("\\end{screentext}"))) {
      //      fprintf(stderr, "END\n");
      
      if (in_code) end_program();
      
      in_code = 0;

    }
    else if (!strncmp("\\end{verbatim}", line, strlen("\\end{verbatim}"))) {
      //        fprintf(stderr, "END\n");

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
