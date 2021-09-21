#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_SIZE (4 * 1024 * 1024)
char buff[MAX_FILE_SIZE];
char token[MAX_FILE_SIZE];

void parse_token(char** s, char* t)
{
  int verbatim_mode = 0;

  while (**s == ' ')
    (*s)++;
  while (**s == '\t')
    (*s)++;

  if (**s != '{') {
    (*s)[32] = 0;
    fprintf(stderr, "ERROR: Missing { at '%s...'\n", *s);
    exit(-1);
  }
  (*s)++;

  int tl = 0;
  while (**s) {
    // %< .... %> delineates a verbatim latex section, where no escaping of any sort happens.
    if (verbatim_mode) {
      if ((*s)[0] == '%' && (*s)[1] == '>') {
        (*s) += 2;
        verbatim_mode = 0;
      }
      else {
        t[tl++] = **s;
        (*s)++;
      }
    }
    else {
      if ((*s)[0] == '%' && (*s)[1] == '<') {
        (*s) += 2;
        verbatim_mode = 1;
        continue;
      }

      if ((**s) == '}') {
        (*s)++;
        t[tl] = 0;
        return;
      }
      else {
        switch (**s) {
        case '$':
        case '_':
          t[tl++] = '\\';
          t[tl++] = **s;
          break;

        default:
          t[tl++] = **s;
        }
        (*s)++;
      }
    }
  }
}

int main(int argc, char** argv)
{
  FILE* f = fopen(argv[1], "r");
  int len = fread(buff, 1, MAX_FILE_SIZE, f);
  fprintf(stderr, "Read %d bytes.\n", len);
  buff[len] = 0;

  char* s = strstr(buff, "\\m65libsummary");
  while (s) {
    s += strlen("\\m65libsummary");

    // Skip spaces
    while (s[0] == ' ')
      s++;
    parse_token(&s, token);
    printf("\n\\subsection{%s}\n", token);
    printf("\\index{%s}\n", token);
    parse_token(&s, token);
    printf("\\begin{description}[leftmargin=2.4cm,style=nextline]\n"
           "\\item [Description:] {%s}\n",
        token);
    int last_was_param = 0;
    while (strncmp(s, "\\m65libsummary", strlen("\\m65libsummary"))) {
      int is_param = 0;
      s = strstr(s, "\\m65lib");
      if (!s)
        break;
      if (!strncmp(s, "\\m65libparam", strlen("\\m65libparam"))) {
        s += strlen("\\m65libparam");
        parse_token(&s, token);
        if (!last_was_param) {
          last_was_param = 1;
          printf("\\item [Parameters:]\n\\begin{description}\n");
        }
        printf("\\item [{\\em %s}:]", token);
        is_param = 1;
        parse_token(&s, token);
        printf(" {%s}\n", token);
      }
      else if (!strncmp(s, "\\m65libretval", strlen("\\m65libretval"))) {
        s += strlen("\\m65libretval");
        if (last_was_param)
          printf("\\end{description}\n");
        parse_token(&s, token);
        printf("\\item [Return Value:] {%s}\n", token);
      }
      else if (!strncmp(s, "\\m65libsyntax", strlen("\\m65libsyntax"))) {
        s += strlen("\\m65libsyntax");
        if (last_was_param)
          printf("\\end{description}\n");
        parse_token(&s, token);
        printf("\\item [Syntax:] \\stw{%s}\n", token);
      }
      else if (!strncmp(s, "\\m65libremarks", strlen("\\m65libremarks"))) {
        s += strlen("\\m65libremarks");
        if (last_was_param)
          printf("\\end{description}\n");
        parse_token(&s, token);
        printf("\\item [Notes:] {%s}\n", token);
      }
      else if (!strncmp(s, "\\m65libsummary", strlen("\\m65libsummary"))) {
        // Just catch so we don't report an error.
        if (last_was_param)
          printf("\\end{description}\n");
      }
      else {
        s[32] = 0;
        fprintf(stderr, "Unknown block type '%s...'\n", s);
        exit(-1);
      }
      last_was_param = is_param;
    }

    printf("\\end{description}\n");

    // Find next instance
    if (s)
      s = strstr(s, "\\m65libsummary");
    else
      break;
  }
}
