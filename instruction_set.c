#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

struct opcode {
  char* abbrev;
  char* mode;
  int mode_num;
  int instr_num;
  unsigned int bytes;
};

#define MAX_MODES 32
struct modeinfo {
  char* description;
  int bytes;
  int cycles;
  char* cycle_notes;
  char* memory_equation;
  char* long_description;
  char* long_title;
};

struct modeinfo modeinfo[MAX_MODES];
char* modes[MAX_MODES];
int mode_count = 1; // 0: implied

char* instrs[256];
int instruction_count = 0;

struct opcode opcodes[256];

// Collected information for opcode table generation
int cycle_counts = 0;
char* cycle_count_list[1024];
unsigned int cycle_count_list_bytes[1024];

char* instruction_names[256];

// static variables and arrays

//   0     1     2     3     4     5     6
//   nflag,zflag,iflag,cflag,dflag,vflag,eflag;
char pflags[7][16];

char* instruction;
int CPU;
int extra_cycles;
size_t len;
char* is_unintended;
char insfilename[1024];
char short_description[256];
char long_description[8192];
char action[256];
char bction[256];
char flags[256];

char* StrDup(const char* s)
{
  char* r;
  if (s == NULL)
    return strdup(" ");
  r = strdup(s);
  if (r == NULL) {
    fprintf(stderr, "\n*** failed to allocate in StrDup for string [%s]\n", s);
    exit(1);
  }
  return r;
}

// read a line from file 'f' into buffer
// and remove CR/LF and trailing blanks

void Fgets(char* Str, size_t Size, FILE* F)
{
  size_t l;
  memset(Str, 0, Size);
  if (fgets(Str, Size - 1, F)) {
    l = strlen(Str);
    while (l && Str[l - 1] <= ' ')
      Str[--l] = 0;
  }
  if (Str[0] == 0)
    strcpy(Str, " ");
}

static int compar_str(const void* a, const void* b)
{
  return strcmp(*(char* const*)a, *(char* const*)b);
}

void lookup_mode_description(int m, int isQuad)
{
  fprintf(stderr, "Looking for description of '%s', Q=%d\n", modes[m], isQuad);

  // Normalise mode into a safe filename
  char n[256];
  int nlen = 0;

  if (isQuad)
    n[nlen++] = 'Q';

  for (int i = 0; i < 255 && modes[m][i]; i++) {
    switch (modes[m][i]) {
    case '(':
    case '#':
    case '$':
    case ',':
    case ')':
      n[nlen++] = '_';
      break;
    case '[':
    case ']':
      n[nlen++] = 'S';
      break;
    default:
      n[nlen++] = modes[m][i];
    }
  }
  n[nlen] = 0;
  fprintf(stderr, "Normalised to '%s'\n", n);

  char filename[1024];
  if (strlen(n) == 0) // avoiding 'mode.' filename, as it's not windows-friendly
    snprintf(filename, 1024, "instruction_sets/mode");
  else
    snprintf(filename, 1024, "instruction_sets/mode.%s", n);
  FILE* f = fopen(filename, "rb");
  if (f) {
    char line[8192];
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    if (CPU == 6502 && !strncmp(line, "base", 4))
      strncpy(line, "zero", 4);
    modeinfo[m].description = StrDup(line);
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    modeinfo[m].bytes = atoi(line);
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    modeinfo[m].cycles = atoi(line);
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    modeinfo[m].cycle_notes = StrDup(line);
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    modeinfo[m].memory_equation = StrDup(line);
    fgets(line, 1024, f);
    while (line[0] && line[strlen(line) - 1] < ' ')
      line[strlen(line) - 1] = 0;
    modeinfo[m].long_title = StrDup(line);
    int r = fread(line, 1, 8192, f);
    line[r] = 0;
    modeinfo[m].long_description = StrDup(line);

    fclose(f);
  }
  else {
    fprintf(stderr, "WARNING: Could not find mode description file '%s'\n", filename);
    bzero(&modeinfo[m], sizeof(struct modeinfo));
  }

  return;
}

int main(int argc, char** argv)
{
  /* Read a list of opcodes. Gather them by instruction, and
     generate the various tables and things that we need from
     that. We also have one file per instruction that describes
     its function.
  */

  char processor[1024];
  char processor_path[1024];
  int pplen = 0;
  int plen = 0;

  for (int i = 0; i < 256; i++)
    instruction_names[i] = NULL;

  for (int i = 0; argv[1] && argv[1][i] && argv[1][i] != '.'; i++) {
    processor[plen++] = argv[1][i];
    processor[plen] = 0;
    if (argv[1][i] == '/')
      plen = 0;
    processor_path[pplen++] = argv[1][i];
    processor_path[pplen] = 0;
  }
  fprintf(stderr, "Processor name is '%s'\n", processor);
  CPU = atoi(processor);

  char line[1024];

  strcat(processor_path, ".cycles");
  FILE* cf = fopen(processor_path, "rb");
  if (cf) {
    fprintf(stderr, "Reading instruction cycle count information...\n");
    int count = 0;
    line[0] = 0;
    fgets(line, 1024, cf);
    while (line[0]) {
      unsigned int opcode;
      char instr[1024];
      char cycles[1024];
      int n = sscanf(line, "%x %s %[^\n]", &opcode, instr, cycles);
      if (n == 3) {
        if (count >= 0 && count < 1024)
          cycle_count_list[count] = StrDup(cycles);
      }
      else if (n == 2) {
        if (count >= 0 && count < 1024)
          cycle_count_list[count] = StrDup(instr);
      }
      cycle_count_list_bytes[count] = opcode;
      count++;
      line[0] = 0;
      fgets(line, 1024, cf);
    }
    fclose(cf);
    cycle_counts = count;
  }
  fprintf(stderr, "cycle_counts=%d\n", cycle_counts);

  modes[0] = "implied";
  lookup_mode_description(0, 0);

  int opcode = 0;

  FILE* f = fopen(argv[1], "rb");
  line[0] = 0;
  fgets(line, 1024, f);
  while (line[0]) {
    int bytes;
    char name[1024];
    char mode[1024] = "";
    int n = sscanf(line, "%x %s %[^\n]", &bytes, name, mode);
    if (n < 2) {
      fprintf(stderr, "ERROR: Could not parse line: %s\n", line);
      exit(-3);
    }

    // Store full extended byte sequence
    opcodes[opcode].bytes = bytes;

    int isQuad = 0;
    if (strchr(name, 'Q'))
      isQuad = 1;

    if (!mode[0])
      if (isQuad)
        snprintf(mode, 2, "Q");

    if (n >= 2) {
      //      fprintf(stderr,"3<%s><%s>: ",name,mode);
      int i = 99;
      for (i = 0; i < mode_count; i++) {
        if (!strcmp(modes[i], mode)) {
          break;
        }
      }
      if (i < mode_count)
        opcodes[opcode].mode_num = i;
      else {
        opcodes[opcode].mode_num = mode_count;
        modes[mode_count] = StrDup(mode);

        // Try to find better description and data
        lookup_mode_description(mode_count, isQuad);

        mode_count++;
      }
      opcodes[opcode].mode = StrDup(mode);
    }

    {
      int i = 0;
      for (i = 0; i < instruction_count; i++) {
        if (!strcmp(instrs[i], name)) {
          break;
        }
      }
      if (i < instruction_count)
        opcodes[opcode].instr_num = i;
      else {
        opcodes[opcode].instr_num = instruction_count;
        instrs[instruction_count++] = StrDup(name);
      }
    }

    opcodes[opcode].abbrev = StrDup(name);

    opcode++;

    line[0] = 0;
    fgets(line, 1024, f);
  }

  int opcode_count = opcode;

  fprintf(stderr, "%d addressing modes found.\n", mode_count);
  fprintf(stderr, "%d unique instructions found.\n", instruction_count);

  for (int i = 0; i < mode_count; i++) {
    fprintf(stderr, "Modes #%d : %s\n", i, modeinfo[i].description);
  }

  // Sort instruction names alphabetically
  fprintf(stderr, "Sorting instructions alphabetically.\n");
  qsort(&instrs[0], instruction_count, sizeof(char*), compar_str);
  // Now update the instruction numbers in the array
  for (int i = 0; i < opcode_count; i++) {
    for (int j = 0; j < instruction_count; j++) {
      if (!strcmp(opcodes[i].abbrev, instrs[j])) {
        opcodes[i].instr_num = j;
        break;
      }
    }
  }

  // Now generate the instruction tables.

  for (int i = 0; i < instruction_count; i++) {
    instruction = instrs[i];
    extra_cycles = 0;
    for (int ipf = 0; ipf < 7; ++ipf)
      strcpy(pflags[ipf], "$\\cdot$");

    snprintf(insfilename, sizeof(insfilename), "instruction_sets/inst.%s", instrs[i]);
    FILE* f = fopen(insfilename, "rb");
    if (f) {
      fprintf(stderr, "Using %s\n", insfilename);
      Fgets(short_description, sizeof(short_description), f);
      Fgets(action, sizeof(action), f);
      // There may be different lines for 6502 and 4510 CPU
      if (!strncmp(action, "6502:", 5) || !strncmp(action, "4510:", 5)) {
        Fgets(bction, sizeof(bction), f);
        fprintf(stderr, "action:%s\n", action);
        fprintf(stderr, "bction:%s\n", bction);
        if (!strncmp(bction, processor, 4))
          memmove(action, bction + 5, strlen(bction) - 4);
        else
          memmove(action, action + 5, strlen(action) - 4);
        fprintf(stderr, "zction:%s\n", action);
        fprintf(stderr, "short :%s\n", short_description);
      }
      Fgets(flags, sizeof(flags), f);
      for (int i = 0; flags[i]; i += 2) {
        switch (flags[i]) {
        case 'N':
          pflags[0][0] = flags[i + 1];
          pflags[0][1] = 0;
          break;
        case 'Z':
          pflags[1][0] = flags[i + 1];
          pflags[1][1] = 0;
          break;
        case 'I':
          pflags[2][0] = flags[i + 1];
          pflags[2][1] = 0;
          break;
        case 'C':
          pflags[3][0] = flags[i + 1];
          pflags[3][1] = 0;
          break;
        case 'D':
          pflags[4][0] = flags[i + 1];
          pflags[4][1] = 0;
          break;
        case 'V':
          pflags[5][0] = flags[i + 1];
          pflags[5][1] = 0;
          break;
        case 'E':
          pflags[6][0] = flags[i + 1];
          pflags[6][1] = 0;
          break;
        case 'M':
          extra_cycles++;
          break;
        }
      }

      memset(long_description, 0, sizeof(long_description));
      fread(long_description, 1, sizeof(long_description) - 1, f);
      //      fflush(stdout);
      //      fprintf(stderr,"%s Long desc:\n%s\n\n\n\n",instruction,long_description);
      fclose(f);
    }
    else {
      fprintf(stderr, "WARNING: Could not read %s\n", insfilename);
    }
    fprintf(stderr, "%s - %s\n", instrs[i], short_description);

    is_unintended = strstr(short_description, "(unintended instruction)");
    if (is_unintended) {
      *is_unintended = 0;
      printf("\n\n\\subsection*{\\textcolor{red}{%s [unintended]}}\n", instruction);
    }
    else
      printf("\n\n\\subsection*{%s}\n", instruction);
    printf("\\index{%s}%s\n\n\n", instruction, long_description);

    int delmodify65ce02_note_seen = 0;
    int delidle4510_note_seen = 0;
    int branch_note_seen = 0;
    int page_note_seen = 0;
    int read_note_seen = 0;
    int indirect_note_seen = 0;
    int single_cycle_seen = 0;

    printf("\\begin{tabular}{|lp{3.6cm}ll*{7}{p{2.5mm}}p{0.2cm}|}\n\\hline\n"
           " & \\multicolumn{8}{l}{\\bf %s : %s} & \\multicolumn{3}{r|}{\\bf %s} \\\\\n"
           " & \\multicolumn{10}{l}{%s} & \\\\\n"
           " & & & & {\\bf N} & {\\bf Z} & \\multicolumn{1}{c}{\\bf I} & {\\bf C} & {\\bf D} & {\\bf V} & {\\bf E} & \\\\\n "
           "& & &",
        instruction, short_description, processor, action);

    for (int ipf = 0; ipf < 7; ++ipf)
      printf(" & \\multicolumn{1}{c}{\\bf %s}", pflags[ipf]);

    printf(" & \\\\\n"
           " & & & & & & & & & & & \\\\\n"
           " & {\\bf Addressing Mode} & {\\bf Assembly} & {\\bf Code} & \\multicolumn{3}{c}{\\bf Bytes} & "
           "\\multicolumn{3}{c}{\\bf Cycles} & & \\\\ \n\\hline\n");

    for (int j = 0; j < opcode_count; j++) {
      if (opcodes[j].instr_num == i) {
        int m = opcodes[j].mode_num;
        if (m < 0)
          m = 0;
        fprintf(stderr, "  $%02x %d,%d\n", j, m, opcodes[j].mode_num);
        char* addressing_mode = modeinfo[m].description ? modeinfo[m].description : "No description";
        char assembly[1024] = "LDA \\$1234";
        snprintf(assembly, 1024, "%s ", instruction);
        for (int j = 0; modes[m][j]; j++) {
          // Escape tricky chars
          switch (modes[m][j]) {
          case '$':
          case '#':
            assembly[strlen(assembly) + 1] = 0;
            assembly[strlen(assembly)] = '\\';
          }
          assembly[strlen(assembly) + 1] = 0;
          if (m)
            assembly[strlen(assembly)] = modes[m][j];
        }
        char opcode[16] = "A9";
        snprintf(opcode, 16, "%02X", j);
        if (opcodes[j].bytes & 0xff000000) {
          snprintf(opcode, 16, "%02X %02X %02X %02X", opcodes[j].bytes >> 24, (opcodes[j].bytes >> 16) & 0xff,
              (opcodes[j].bytes >> 8) & 0xff, opcodes[j].bytes & 0xff);
          extra_cycles += 3;
        }
        else if (opcodes[j].bytes & 0xff0000) {
          snprintf(opcode, 16, "%02X %02X %02X", (opcodes[j].bytes >> 16) & 0xff, (opcodes[j].bytes >> 8) & 0xff,
              opcodes[j].bytes & 0xff);
          extra_cycles += 2;
        }
        else if (opcodes[j].bytes & 0xff00) {
          snprintf(opcode, 16, "%02X %02X", (opcodes[j].bytes >> 8) & 0xff, opcodes[j].bytes & 0xff);
          extra_cycles += 1;
        }

        char bytes[16] = "3";
        snprintf(bytes, 16, "%d", modeinfo[m].bytes);
        char cycles[16] = "4";
        snprintf(cycles, 16, "%d", modeinfo[m].cycles + extra_cycles);

        char cycle_count[1024];
        char cycle_notes[1024] = "";

        int k = 0;
        for (k = 0; k < cycle_counts; k++) {
          if (cycle_count_list_bytes[k] == opcodes[j].bytes)
            break;
        }

        if (k < cycle_counts) {
          cycle_count[0] = 0;
          for (int i = 0; isdigit(cycle_count_list[k][i]); i++) {
            {
              cycle_count[i] = cycle_count_list[k][i];
              cycle_count[i + 1] = 0;
              strcpy(cycle_notes, &cycle_count_list[k][i + 1]);
            }
          }
        }
        else {
          snprintf(cycle_count, 1024, "??");
          cycle_notes[0] = 0;
        }

        int delmodify65ce02_note = 0;
        int delidle4510_note = 0;
        int branch_note = 0;
        int indirect_note = 0;
        int page_note = 0;
        int read_note = 0;
        int single_cycle = 0;

        if (strstr(cycle_notes, "d")) {
          delmodify65ce02_note = 1;
          delmodify65ce02_note_seen = 1;
        }
        if (strstr(cycle_notes, "m")) {
          delidle4510_note = 1;
          delidle4510_note_seen = 1;
        }
        if (strstr(cycle_notes, "b")) {
          branch_note = 1;
          branch_note_seen = 1;
        }
        if (strstr(cycle_notes, "p")) {
          page_note = 1;
          page_note_seen = 1;
        }
        if (strstr(cycle_notes, "i")) {
          indirect_note = 1;
          indirect_note_seen = 1;
        }
        if (strstr(cycle_notes, "r")) {
          read_note = 1;
          read_note_seen = 1;
        }
        if (!strcmp(cycle_count, "1")) {
          single_cycle = 1;
          single_cycle_seen = 1;
        }

        instruction_names[j] = StrDup(instruction);
        fprintf(stderr, "instruction_names[%d] set\n", j);

        snprintf(cycle_notes, 1024, "$^{%s%s%s%s%s%s%s}$", branch_note ? "b" : "", delmodify65ce02_note ? "d" : "",
            indirect_note ? "i" : "", delidle4510_note ? "m" : "", page_note ? "p" : "", read_note ? "r" : "",
            single_cycle ? "s" : "");

        printf(" & %s        & %s       & \\multicolumn{1}{c}{%s}     & \\multicolumn{3}{c}{%s} & \\multicolumn{3}{c}{%s} & "
               "\\multicolumn{2}{l|}{%s} \\\\\n",
            addressing_mode, assembly, opcode, bytes, cycle_count, cycle_notes);
      }
    }
    printf("\\hline\n");
    // d = 65CE02 delete idle cycles when CPU >2MHz
    // m = 4510 delete non-bus cycles
    if (branch_note_seen) {
      printf(" \\multicolumn{1}{r}{$b$} & \\multicolumn{11}{l}{Add one cycle if branch is taken.} \\\\\n");
      printf(" \\multicolumn{1}{r}{   } & \\multicolumn{11}{l}{Add one more cycle if branch taken crosses a page boundary.} "
             "\\\\\n");
    }
    if (delmodify65ce02_note_seen)
      printf(" \\multicolumn{1}{r}{$d$} & \\multicolumn{11}{l}{Subtract one cycle when CPU is at 3.5MHz. } \\\\\n");
    if (indirect_note_seen)
      printf(" \\multicolumn{1}{r}{$i$} & \\multicolumn{11}{l}{Add one cycle if clock speed is at 40 MHz.} \\\\\n");
    if (delidle4510_note_seen)
      printf(" \\multicolumn{1}{r}{$m$} & \\multicolumn{11}{l}{Subtract non-bus cycles when at 40MHz. } \\\\\n");
    if (page_note_seen)
      printf(" \\multicolumn{1}{r}{$p$} & \\multicolumn{11}{l}{Add one cycle if indexing crosses a page boundary.} \\\\\n");
    if (read_note_seen)
      printf(" \\multicolumn{1}{r}{$r$} & \\multicolumn{11}{l}{Add one cycle if clock speed is at 40 MHz.} \\\\\n");
    if (single_cycle_seen)
      printf(" \\multicolumn{1}{r}{$s$} & \\multicolumn{11}{l}{Instruction requires 2 cycles when CPU is run at 1MHz or "
             "2MHz.} \\\\\n");

    printf("\\end{tabular}\n");
    fflush(stdout);

    snprintf(insfilename, 1024, "%s-opcodes.tex", processor);
    FILE* tf = fopen(insfilename, "wb");
    if (tf) {
      fprintf(tf, "\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|}\n");
      fprintf(tf, "\\cline{2-17}\n");
      fprintf(tf, "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 & \\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC "
                  "& \\$xD & \\$xE & \\$xF \\\\ \\hline\n");
      for (int i = 0; i < 16; i++) {
        fprintf(tf, "\\multicolumn{1}{|l|}{\\$%Xx} ", i);
        for (int j = 0; j < 16; j++)
          if (i * 16 + j < opcode_count)
            fprintf(tf, "& %s     ", instruction_names[i * 16 + j] ? instruction_names[i * 16 + j] : "??");
          else
            fprintf(tf, "& ");
        fprintf(tf, "     \\\\ \\hline\n");
      }
      fprintf(tf, "\\end{tabular}\n");
      fclose(tf);
    }

    snprintf(insfilename, 1024, "%s-cycles.tex", processor);
    tf = fopen(insfilename, "wb");
    if (tf) {
      fprintf(tf, "\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|}\n");
      fprintf(tf, "\\cline{2-17}\n");
      fprintf(tf, "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 & \\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC "
                  "& \\$xD & \\$xE & \\$xF \\\\ \\hline\n");
      for (int i = 0; i < 16; i++) {
        fprintf(tf, "\\multicolumn{1}{|l|}{\\$%Xx} ", i);
        for (int j = 0; j < 16; j++) {
          char cycle_count[1024];
          char cycle_notes[1024] = "";

          if (cycle_count_list[i * 16 + j]) {
            cycle_count[0] = 0;
            for (int k = 0; isdigit(cycle_count_list[i * 16 + j][k]); k++) {
              {
                cycle_count[k] = cycle_count_list[i * 16 + j][k];
                cycle_count[k + 1] = 0;
                strcpy(cycle_notes, &cycle_count_list[i * 16 + j][k + 1]);
              }
            }
          }
          else {
            snprintf(cycle_count, 1024, "??");
            cycle_notes[0] = 0;
          }

          int delmodify65ce02_note = 0;
          int delidle4510_note = 0;
          int branch_note = 0;
          int page_note = 0;
          int read_note = 0;
          int single_cycle = 0;

          if (strstr(cycle_notes, "d")) {
            delmodify65ce02_note = 1;
            delmodify65ce02_note_seen = 1;
          }
          if (strstr(cycle_notes, "m")) {
            delidle4510_note = 1;
            delidle4510_note_seen = 1;
          }
          if (strstr(cycle_notes, "b")) {
            branch_note = 1;
            branch_note_seen = 1;
          }
          if (strstr(cycle_notes, "p")) {
            page_note = 1;
            page_note_seen = 1;
          }
          if (strstr(cycle_notes, "r")) {
            read_note = 1;
            read_note_seen = 1;
          }
          if (!strcmp(cycle_count, "1")) {
            single_cycle = 1;
            single_cycle_seen = 1;
          }

          snprintf(cycle_notes, 1024, "$^{%s%s%s%s%s%s}$", branch_note ? "b" : "", delmodify65ce02_note ? "d" : "",
              delidle4510_note ? "m" : "", page_note ? "p" : "", read_note ? "r" : "", single_cycle ? "s" : "");

          fprintf(tf, "& %s%s     ", cycle_count, cycle_notes);
        }
        fprintf(tf, "     \\\\ \\hline\n");
      }
      if (branch_note_seen)
        fprintf(
            tf, " \\multicolumn{1}{r}{$b$} & \\multicolumn{16}{l}{Add one cycle if branch crosses a page boundary.} \\\\\n");
      if (delmodify65ce02_note_seen)
        fprintf(tf, " \\multicolumn{1}{r}{$d$} & \\multicolumn{16}{l}{Subtract one cycle when CPU is at 3.5MHz. } \\\\\n");
      if (delidle4510_note_seen)
        fprintf(tf, " \\multicolumn{1}{r}{$m$} & \\multicolumn{16}{l}{Subtract non-bus cycles when at 40MHz. } \\\\\n");
      if (page_note_seen)
        fprintf(tf,
            " \\multicolumn{1}{r}{$p$} & \\multicolumn{16}{l}{Add one cycle if indexing crosses a page boundary.} \\\\\n");
      if (read_note_seen)
        fprintf(tf, " \\multicolumn{1}{r}{$r$} & \\multicolumn{16}{l}{Add one cycle if clock speed is at 40 MHz.} \\\\\n");
      if (single_cycle_seen)
        fprintf(tf, " \\multicolumn{1}{r}{$s$} & \\multicolumn{16}{l}{Instruction requires 2 cycles when CPU is run at 1MHz "
                    "or 2MHz.} \\\\\n");

      fprintf(tf, "\\end{tabular}\n");
      fclose(tf);
    }

    // write matrix of addressing modes

    snprintf(insfilename, 1024, "%s-modes.tex", processor);
    tf = fopen(insfilename, "wb");
    if (tf) {
      fprintf(tf, "\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|}\n");
      fprintf(tf, "\\cline{2-17}\n");
      fprintf(tf, "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 & \\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC "
                  "& \\$xD & \\$xE & \\$xF \\\\ \\hline\n");
      for (int i = 0; i < 16; i++) {
        fprintf(tf, "\\multicolumn{1}{|l|}{\\$%Xx} ", i);
        for (int j = 0; j < 16; j++) {
          int m = opcodes[i * 16 + j].mode_num;
          char safe_name[1024];
          int slen = 0;
          if (m >= 0) {
            // fprintf(stderr,"Escaping mode #%d ",m); fflush(stderr);
            // fprintf(stderr,"= \"%s\"\n",modes[m]); fflush(stderr);
            for (int k = 0; modes[m][k]; k++) {
              switch (modes[m][k]) {
              case '$':
              case '#':
                safe_name[slen++] = '\\';
                // FALL THROUGH
              default:
                safe_name[slen++] = modes[m][k];
              }
            }
            safe_name[slen] = 0;
          }
          else {
            snprintf(safe_name, 1024, "");
          }
          fprintf(tf, "& %s     ", safe_name);
        }
        fprintf(tf, "     \\\\ \\hline\n");
      }

      fprintf(tf, "\\end{tabular}\n");
      fclose(tf);
    }
  }
}
