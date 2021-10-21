#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#define MAX_COMMANDS 1024
char* commands[MAX_COMMANDS];
int cmd_count = 0;

char* types[MAX_COMMANDS];

int cmd_used[MAX_COMMANDS];

void reset_cmd_usage(void)
{
  fprintf(stderr, "RESETCMDS\n");
  for (int i = 0; i < cmd_count; i++)
    cmd_used[i] = 0;
}

void mark_cmd_used(char* s)
{
  for (int i = 0; i < cmd_count; i++)
    if (!strcmp(s, commands[i])) {
      fprintf(stderr, " CMD #%d used '%s'\n", i, commands[i]);
      cmd_used[i]++;
    }
}

void parse_basic_text(char* s)
{
  int quote_mode = 0;
  char token[8192];
  int tlen = 0;
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
            mark_cmd_used(token);
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
      mark_cmd_used(token);
    }
  }
}

const char *Keytext[5] =
{
   "\\index{BASIC 65 Commands!",
   "\\index{BASIC 65 Functions!",
   "\\index{BASIC 65 Operators!",
   "\\index{BASIC 65 System Commands!",
   "\\index{BASIC 65 System Variables!"
};

int main(int argc, char** argv)
{
  int k;
  FILE* f = fopen(argv[1], "r");

  if (!f) {
    fprintf(stderr, "ERROR: Could not read '%s'\n", argv[1]);
    exit(-1);
  }

  int in_code = 0;
  int code_font = 0;

  char line[8192];

  /*
    Pass 1: Find list of BASIC 65 commands
   */
  line[0] = 0;
  fgets(line, 8192, f);
  // TODO fix this. change k < 3 to k < 5 to index more things, but it has a segmentation fault
  while (line[0]) for (k=0 ; k < 3 ; ++k) {

    if (!strncmp(Keytext[k], line, strlen(Keytext[k]))) {
      char command[8192];
      strcpy(command, &line[strlen(Keytext[k])]);
      for (int i = 0; command[i]; i++) {
        if (!isalnum(command[i])) {
          command[i] = 0;
          break;
        }
      }
      int i = 0;
      for (i = 0; i < cmd_count; i++) {
        if (!strcmp(command, commands[i]))
          break;
      }
      if (i == cmd_count) {
        if (isalpha(command[0]))
          commands[cmd_count] = strdup(command);
          types[cmd_count++] = strdup(Keytext[k]);
      }
    }

    line[0] = 0;
    fgets(line, 8192, f);
  }

  fclose(f);
  fprintf(stderr, "Found %d BASIC keywords.\n", cmd_count);
  for (int i = 0; i < cmd_count; i++)
    fprintf(stderr, "  %s\n", commands[i]);

  /*
    Pass 2: Find example programs, and note which commands should be indexed following each example program.
   */
  f = fopen(argv[1], "r");
  if (!f) {
    fprintf(stderr, "ERROR: Could not read '%s'\n", argv[1]);
    exit(-1);
  }

  in_code = 0;
  code_font = 0;
  reset_cmd_usage();

  line[0] = 0;
  fgets(line, 8192, f);
  while (line[0]) {

    if (!strncmp("\\verbatimfont{\\codefont}", line, strlen("\\verbatimfont{\\codefont}"))) {
      fprintf(stdout, "%s", line);
      code_font = 1;
    }
    else if (!strncmp("\\begin{verbatim}", line, strlen("\\begin{verbatim}"))) {
      fprintf(stdout, "%s", line);
      if (code_font == 1) {
        fprintf(stderr, "BEGIN\n");
        reset_cmd_usage();
        in_code = 1;
      }
    }
    else if (!strncmp("\\end{verbatim}", line, strlen("\\end{verbatim}"))) {
      fprintf(stdout, "%s", line);

      fprintf(stderr, "OUTPUT INDEX\n");

      in_code = 0;

      // Skip any existing index entries
      line[0] = 0;
      fgets(line, 8192, f);
      while (!strncmp("\\index{BASIC 65 ", line, strlen("\\index{BASIC 65 "))) {
        line[0] = 0;
        fgets(line, 8192, f);
      }

      // Dump index list
      for (int i = 0; i < cmd_count; i++) {
        if (cmd_used[i]) {
          fprintf(stderr, "  '%s'\n", commands[i]);
          fprintf(stdout, "%s%s!Examples}\n", types[i], commands[i]);
        }
      }

      fprintf(stdout, "%s", line);
    }
    else {
      fprintf(stdout, "%s", line);
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
