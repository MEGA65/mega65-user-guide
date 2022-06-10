#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

struct opcode {
  unsigned char defined;
  char *abbrev;
  char *mode;
  int modeId;
  int instr_num;
  unsigned int cycle_count;
  char cycle_notes[10];
  unsigned int bytes;
  unsigned char isUnintended;
  unsigned char isFar, isQuad, isFarQuad;
};

#define MAX_MODES 32
struct modeinfo {
  char *mode;
  char *nmode;
  char *texmode;
  char *description;
  char *shortDesc;
  int bytes;
  int cycles;
  int extra_cycles;
  char *cycle_notes;
  char *memory_equation;
  char *long_description;
  char *long_title;
  unsigned char isQuad;
};

int mode_count = 0;
struct modeinfo modeinfo[MAX_MODES];

char *instrs[256];
int instruction_count = 0;

#define MAX_OPCODES 256*4
struct opcode opcodes[MAX_OPCODES];

// Collected information for opcode table generation
int cycle_counts = 0;
char *cycle_count_list[1024];
unsigned int cycle_count_list_bytes[1024];

char *instruction_names[256];

// static variables and arrays

//   0     1     2     3     4     5     6
//   nflag,zflag,iflag,cflag,dflag,vflag,eflag;
char pflags[7][16];

char *instruction;
int CPU;
int extra_cycles;
size_t len;
char *is_unintended;
char short_description[256];
char long_description[8192];
char action[256];
char bction[256];
char flags[256];
char DEBUG = 0;

// strdup, bug panic and exit if no memory was available
char *StrDup(const char *s) {
  char *r;
  if (s == NULL)
    r = strdup("");
  else
    r = strdup(s);
  if (r == NULL) {
    fprintf(stderr, "\n*** failed to allocate in StrDup for string [%s]\n", s);
    exit(1);
  }
  return r;
}

// escape string by putting backslash before $ and #
char *texEscape(const char *s) {
  char t[255];
  int i = 0;
  for (int j = 0; s[j]; j++) {
    // Escape tricky chars
    switch (s[j]) {
      case '$':
      case '#':
        t[i++] = '\\';
    }
    t[i++] = s[j];
  }
  t[i] = 0;
  return StrDup(t);
}

// read a line from file 'f' into buffer
// and remove CR/LF and trailing blanks
char *Fgets(char *Str, size_t Size, FILE* F) {
  size_t l;
  if (fgets(Str, Size, F)) {
    l = strlen(Str);
    while (l && Str[l - 1] <= ' ')
      Str[--l] = 0;
    return Str;
  }
  return NULL;
}

static int compar_str(const void* a, const void* b) {
  return strcmp(*(char *const*)a, *(char *const*)b);
}

void error_exit(char *errstr) {
  fprintf(stderr, "\nERROR %s\n", errstr);
  exit(-1);
}

#define Fgets_err(a, b, c, d) if (!Fgets(a,b,c)) error_exit(d)

int lookup_mode_description(char *mode, int isQuad) {
  if (DEBUG) fprintf(stderr, "Looking for description of '%s', Q=%d", mode, isQuad);

  // Normalise mode into a safe filename extension
  char nmode[256], errstr[1256];
  int nlen = 0;

  if (isQuad)
    nmode[nlen++] = 'Q';

  for (int i = 0; i < 255 && mode[i]; i++) {
    switch (mode[i]) {
      case '(':
      case '#':
      case '$':
      case ',':
      case ')':
        nmode[nlen++] = '_';
        break;
      case '[':
      case ']':
        nmode[nlen++] = 'S';
        break;
      default:
        nmode[nlen++] = mode[i];
    }
  }
  nmode[nlen] = 0;
  if (DEBUG) fprintf(stderr, ", normalised to '%s'\n", nmode);

  // search if we already loaded this mode
  int modeId = 0;
  for (modeId = 0; modeId < mode_count; modeId++)
    if (!strcmp(nmode, modeinfo[modeId].nmode))
      return modeId;

  // new mode
  mode_count++;
  modeinfo[modeId].mode = StrDup(mode);
  modeinfo[modeId].nmode = StrDup(nmode);
  modeinfo[modeId].texmode = texEscape(mode);
  modeinfo[modeId].isQuad = isQuad;
  char filename[1024];
  if (strlen(nmode) == 0) // avoiding 'mode.' filename, as it's not windows-friendly
    snprintf(filename, 1024, "instruction_sets/mode.implied");
  else
    snprintf(filename, 1024, "instruction_sets/mode.%s", nmode);
  if (DEBUG) fprintf(stderr, "MODE-FILE %s\n", filename);
  FILE* f = fopen(filename, "rb");
  if (f) {
    char line[8192];
    snprintf(errstr, 1255, "reading mode info %s", filename);
    Fgets_err(line, 1024, f, errstr);
    if (CPU == 6502 && !strncmp(line, "base", 4))
      memcpy(line, "zero", 4);
    modeinfo[modeId].description = StrDup(line);
    if (CPU == 6502) {
      char *find = strstr(line, "bp");
      if (find != NULL) memcpy(find, "zp", 2);
    }
    Fgets_err(line, 1024, f, errstr);
    modeinfo[modeId].shortDesc = texEscape(line);
    Fgets_err(line, 1024, f, errstr);
    modeinfo[modeId].bytes = atoi(line);
    Fgets_err(line, 1024, f, errstr);
    modeinfo[modeId].cycles = atoi(line);
    Fgets_err(line, 1024, f, errstr);
    modeinfo[modeId].cycle_notes = StrDup(line);
    Fgets_err(line, 1024, f, errstr);
    modeinfo[modeId].long_title = StrDup(line);
    int r = fread(line, 1, 8192, f);
    line[r] = 0;
    modeinfo[modeId].long_description = StrDup(line);

    fclose(f);
    return modeId;
  }
  else {
    fprintf(stderr, "WARNING: Could not find mode description file '%s'\n", filename);
    bzero(&modeinfo[modeId], sizeof(struct modeinfo));
  }

  return -1;
}

void usage(char *prg) {
  fprintf(stderr, "Usage: %s [--debug|-d] OPCODEFILE [EXTOPCODEFILE]\n", prg);
  exit(-1);
}

int main(int argc, char** argv) {
  /* 
   * Read a list of opcodes. Gather them by instruction, and
   * generate the various tables and things that we need from
   * that. We also have one file per instruction that describes
   * its function.
   */
  char processor[256];
  char processor_path[1024];
  char filename[1024];
  char *find;
  int argi = 1;
  char line[1024], errstr[256];

  if (argc < 2) usage(argv[0]);

  // parse cmdline for debug flag
  if (!strcmp(argv[argi], "--debug") || !strcmp(argv[argi], "-d")) {
    argi++;
    DEBUG = 1;
  }

  if (argi >= argc) usage(argv[0]);

  for (int i = 0; i < 256; i++)
    instruction_names[i] = NULL;

  find = strrchr(argv[argi], '/');
  if (find == NULL)
    find = argv[argi];
  else
    find++; // skip over /
  strncpy(processor, find, 256);
  find = strrchr(processor, '.');
  if (find != NULL) *find = 0;

  strncpy(processor_path, argv[argi], 1024);
  find = strrchr(processor_path, '.');
  if (find != NULL) *find = 0;
  CPU = atoi(processor);
  fprintf(stderr, "Processor name is '%s' (%d)\n", processor, CPU);

  // read opcodes
  lookup_mode_description("", 0);
  int opcode_count = 0;
  FILE* f = fopen(argv[argi], "rb");
  if (!f) {
    fprintf(stderr, "opc file \"%s\" not found!\n\n", argv[argi]);
    exit(1);
  }

  memset(opcodes, 0, sizeof(struct opcode)*MAX_OPCODES);
  for (int i=0; i<MAX_OPCODES; i++) // initialise for empty instructions
    opcodes[i].instr_num = -1;

  while (Fgets(line, 1024, f)) {
    unsigned int bytes, i = 0, n, opc_offset;
    char name[1024];
    char mode[1024] = "";

    n = sscanf(line, "%x %s %[^\n]", &bytes, name, mode);
    if (n < 2) {
      fprintf(stderr, "ERROR: Could not parse line: %s\n", line);
      exit(-3);
    }

    opc_offset = bytes&0xff;
    if (bytes > 0xff)
      opc_offset += 256;
    if (bytes > 0xffff)
      opc_offset += 256;
    if (bytes > 0xffffff)
      opc_offset += 256;

    if (opcodes[opc_offset].defined) {
      fprintf(stderr, "WARNING: Duplicate definition for opcode %08X - %s IGNORED\n", bytes, name);
      continue;
    }
    // Store full extended byte sequence
    opcodes[opc_offset].bytes = bytes;
    opcodes[opc_offset].defined = 1;

    int isQuad = 0;
    if (strchr(name, 'Q') && strcmp(name, "BEQ")) // BEQ is no Q opcode!
      isQuad = 1;

    if (!mode[0] && isQuad) // replace empty Q with "Q"
      strcpy(mode, "Q");

    if (DEBUG) fprintf(stderr, "parsing opc #%02x: %d / $%08X / %s / %s / Q=%d\n", bytes&0xff, n, bytes, name, mode, isQuad);

    if (n >= 2) {
      //      fprintf(stderr,"3<%s><%s>: ",name,mode);
      int modeId = lookup_mode_description(mode, isQuad);
      if (modeId < 0) {
        fprintf(stderr, "ERROR: mode not found (%s, %d)\n", mode, isQuad);
        exit(-3);
      } else
        opcodes[opc_offset].modeId = modeId;
    }

    for (i = 0; i < instruction_count; i++) {
      if (!strcmp(instrs[i], name)) {
        break;
      }
    }
    if (i < instruction_count)
      opcodes[opc_offset].instr_num = i;
    else {
      opcodes[opc_offset].instr_num = instruction_count;
      instrs[instruction_count++] = StrDup(name);
    }
    opcodes[opc_offset].abbrev = StrDup(name);
    opcode_count++;
  }

  argi++;
  // check for extended opcode file
  if (argi < argc) {
    FILE* f = fopen(argv[argi], "rb");
    if (!f) {
      fprintf(stderr, "ERROR: extended opcode file \"%s\" not found!\n\n", argv[argi]);
      exit(1);
    }
    unsigned int bytes, opc_offset, n;
    char name[1024];
    char mode[1024] = "";
    while (Fgets(line, 1024, f)) {
      n = sscanf(line, "%x %s %[^\n]", &bytes, name, mode);
      if (n < 2) {
        fprintf(stderr, "ERROR: Could not parse line: %s\n", line);
        exit(-3);
      }

      if (!strcmp(name, "RESQ") || !strcmp(name, "RESV")) continue;

      opc_offset = bytes&0xff;

      if (!opcodes[opc_offset].defined) continue;

      if ((bytes&0xffffff00) == 0x0000ea00)
        opcodes[opc_offset].isFar = 1;
      else if ((bytes&0xffffff00) == 0x00424200)
        opcodes[opc_offset].isQuad = 1;
      else if ((bytes&0xffffff00) == 0x4242ea00)
        opcodes[opc_offset].isFarQuad = 1;
    }
  }

  // read cycles
  strcat(processor_path, ".cycles");
  FILE* cf = fopen(processor_path, "rb");
  if (!cf) {
    fprintf(stderr, "ERROR: cycle file \"%s\" not found!\n\n", processor_path);
    exit(1);
  }
  fprintf(stderr, "Reading instruction cycle count information...\n");
  cycle_counts = 0;
  while (Fgets(line, 1024, cf) && cycle_counts < 1024) {
    unsigned int bytes, opc_offset;
    char instr[1024], *notes, cycles[1024];

    int n = sscanf(line, "%x %s %s", &bytes, instr, cycles);
    if (n == 3)
      cycle_count_list[cycle_counts] = StrDup(cycles);
    else if (n == 2)
      cycle_count_list[cycle_counts] = StrDup(instr);
    else
      fprintf(stderr, "WARNING: invalid number of fields for %X\n", bytes);
    cycle_count_list_bytes[cycle_counts] = bytes;

    opc_offset = bytes&0xff;
    if (bytes > 0xff)
      opc_offset += 256;
    if (bytes > 0xffff)
      opc_offset += 256;
    if (bytes > 0xffffff)
      opc_offset += 256;

    opcodes[opc_offset].cycle_count = strtol(cycle_count_list[cycle_counts], &notes, 10);
    strncpy(opcodes[opc_offset].cycle_notes, notes, 9);
    if (DEBUG) fprintf(stderr, "%08X %d '%s'\n", bytes, opcodes[opc_offset].cycle_count, opcodes[opc_offset].cycle_notes);

    cycle_counts++;
  }
  fclose(cf);

  fprintf(stderr, "%d opcodes found.\n", opcode_count);
  fprintf(stderr, "%d cycle_counts added.\n", cycle_counts);
  fprintf(stderr, "%d addressing modes found.\n", mode_count);
  fprintf(stderr, "%d unique instructions found.\n", instruction_count);

  for (int i = 0; i < mode_count; i++) {
    fprintf(stderr, "Mode #%02d: %s\n", i, modeinfo[i].description);
  }

  // Sort instruction names alphabetically
  fprintf(stderr, "Sorting instructions alphabetically.\n");
  qsort(&instrs[0], instruction_count, sizeof(char*), compar_str);
  // Now update the instruction numbers in the array
  for (int i = 0; i < MAX_OPCODES; i++) {
    for (int j = 0; j < instruction_count; j++) {
      if (opcodes[i].defined && opcodes[i].abbrev != NULL && !strcmp(opcodes[i].abbrev, instrs[j])) {
        opcodes[i].instr_num = j;
        break;
      }
    }
  }

  /*
   * Now generate the instruction tables.
   */
  int delmodify65ce02_note = 0;
  int delidle4510_note = 0;
  int branch_note = 0;
  int indirect_note = 0;
  int page_note = 0;
  int read_note = 0;
  int single_cycle = 0;
  int delmodify65ce02_note_seen = 0;
  int delidle4510_note_seen = 0;
  int branch_note_seen = 0;
  int page_note_seen = 0;
  int read_note_seen = 0;
  int indirect_note_seen = 0;
  int single_cycle_seen = 0;
  char opcode[16], assembly[256], bytes[16], cycles[16], cycle_count[256], cycle_notes[1024];
  char *addressing_mode;

  snprintf(filename, 1024, "instructionset-%s.tex", processor);
  FILE* tf1 = fopen(filename, "wb");
  if (!tf1) {
    fprintf(stderr, "ERROR: output file \"%s\" could not be created!\n", filename);
    exit(1);
  }

  for (int i = 0; i < instruction_count; i++) {
    // skip reserved 45GS02 instructions
    // if (!strcmp(instrs[i], "RESQ") || !strcmp(instrs[i], "RSVQ")) continue;
    instruction = instrs[i];
    extra_cycles = 0;
    for (int ipf = 0; ipf < 7; ++ipf)
      strcpy(pflags[ipf], "$\\cdot$");

    snprintf(filename, sizeof(filename), "instruction_sets/inst.%s", instrs[i]);
    FILE* f = fopen(filename, "rb");
    if (!f) {
      fprintf(stderr, "WARNING: Could not read %s\n", filename);
    } else {
      snprintf(errstr, 1255, "reading instruction %s", filename);
      fprintf(stderr, "Using %s\n", filename);
      Fgets_err(short_description, sizeof(short_description), f, errstr);
      Fgets_err(action, sizeof(action), f, errstr);
      // There may be different lines for 6502 and 4510 CPU
      if (!strncmp(action, "6502:", 5) || !strncmp(action, "4510:", 5)) {
        Fgets_err(bction, sizeof(bction), f, errstr);
        if (DEBUG) fprintf(stderr, "action:%s\n", action);
        if (DEBUG) fprintf(stderr, "bction:%s\n", bction);
        if (!strncmp(bction, processor, 4))
          memmove(action, bction + 5, strlen(bction) - 4);
        else
          memmove(action, action + 5, strlen(action) - 4);
        if (DEBUG) fprintf(stderr, "zction:%s\n", action);
        if (DEBUG) fprintf(stderr, "short :%s\n", short_description);
      }
      Fgets_err(flags, sizeof(flags), f, errstr);
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
    fprintf(stderr, "%s - %s:\n", instrs[i], short_description);

    is_unintended = strstr(short_description, "(unintended instruction)");

    if (is_unintended) {
      *is_unintended = 0;
      fprintf(tf1, "\n\n\\subsection{\\textcolor{red}{%s [unintended]}}\n", instruction);
    }
    else
      fprintf(tf1, "\n\n\\subsection{%s}\n", instruction);
    fprintf(tf1, "\\index{%s}%s\n\n\n", instruction, long_description);

    delmodify65ce02_note_seen = 0;
    delidle4510_note_seen = 0;
    branch_note_seen = 0;
    page_note_seen = 0;
    read_note_seen = 0;
    indirect_note_seen = 0;
    single_cycle_seen = 0;

    fprintf(tf1, "\\begin{center}\n\\begin{tabular}{|>{\\raggedright\\arraybackslash}p{0.2cm}p{9em}p{7.5em}p{6em}*{7}{>{\\centering\\arraybackslash}p{1em}}p{0.2cm}|}\n"
           "\\hline\n"
           " & \\multicolumn{8}{l}{\\bf %s : %s} & \\multicolumn{3}{r|}{\\bf %s} \\\\\n"
           " & \\multicolumn{10}{l}{%s} & \\\\\n"
           " & & & & {\\bf N} & {\\bf Z} & {\\bf I} & {\\bf C} & {\\bf D} & {\\bf V} & {\\bf E} & \\\\\n"
           " & & &",
        instruction, short_description, processor, action);

    for (int ipf = 0; ipf < 7; ipf++)
      fprintf(tf1, " & {\\bf %s}", pflags[ipf]);

    fprintf(tf1, " & \\\\\n"
           " & & & & & & & & & & & \\\\[-6pt]\n"
           " & {\\bf Addressing Mode} & {\\bf Assembly} & {\\bf Code} & \\multicolumn{3}{c}{\\bf Bytes} & "
           "\\multicolumn{3}{c}{\\bf Cycles} & & \\\\ \n\\hline\n");

    int j;
    for (j = 0; j < MAX_OPCODES; j++) {
      if (opcodes[j].instr_num == i) {
        int m = opcodes[j].modeId;
        if (m < 0)
          m = 0;
        if (DEBUG) fprintf(stderr,"  %d %08X %d%d%d\n", j, opcodes[j].bytes, opcodes[j].isFar, opcodes[j].isQuad, opcodes[j].isFarQuad);
        opcodes[j].isUnintended = (is_unintended!=NULL) || (!strncmp(opcodes[j].abbrev, "NOP", 3) && j != 0xEA); // only EA is the real NOP
        addressing_mode = modeinfo[m].description ? modeinfo[m].description : "No description";
        snprintf(assembly, 256, "%s %s", instruction, modeinfo[m].texmode);

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
        } else
          snprintf(opcode, 16, "%02X", opcodes[j].bytes & 0xff);
        fprintf(stderr, "  %s,%s; ", opcode, m?modeinfo[m].nmode:"NULL");

        snprintf(bytes, 16, "%d", modeinfo[m].bytes);
        snprintf(cycles, 16, "%d", modeinfo[m].cycles + extra_cycles);
        modeinfo[m].extra_cycles = extra_cycles;

        snprintf(cycle_count, 255, "%d", opcodes[j].cycle_count);
        strncpy(cycle_notes, opcodes[j].cycle_notes, 1023);
        if (DEBUG) fprintf(stderr, "%s %s\n", cycle_count, cycle_notes);

        if (strstr(cycle_notes, "d")) {
          delmodify65ce02_note = 1;
          delmodify65ce02_note_seen = 1;
        } else
          delmodify65ce02_note = 0;
        if (strstr(cycle_notes, "m")) {
          delidle4510_note = 1;
          delidle4510_note_seen = 1;
        } else
          delidle4510_note = 0;
        if (strstr(cycle_notes, "b")) {
          branch_note = 1;
          branch_note_seen = 1;
        } else
          branch_note = 0;
        if (strstr(cycle_notes, "p")) {
          page_note = 1;
          page_note_seen = 1;
        } else
          page_note = 0;
        if (strstr(cycle_notes, "i")) {
          indirect_note = 1;
          indirect_note_seen = 1;
        } else
          indirect_note = 0;
        if (strstr(cycle_notes, "r")) {
          read_note = 1;
          read_note_seen = 1;
        } else
          read_note = 0;
        if (!strcmp(cycle_count, "1")) {
          single_cycle = 1;
          single_cycle_seen = 1;
        } else
          single_cycle = 0;

        snprintf(cycle_notes, 1024, "$^{%s%s%s%s%s%s%s}$", branch_note ? "b" : "", delmodify65ce02_note ? "d" : "",
            indirect_note ? "i" : "", delidle4510_note ? "m" : "", page_note ? "p" : "", read_note ? "r" : "",
            single_cycle ? "s" : "");

        fprintf(tf1, " & %s        & %s       & %s     & \\multicolumn{3}{c}{%s} & \\multicolumn{3}{c}{%s} & "
               "\\multicolumn{2}{l|}{%s} \\\\\n",
            addressing_mode, assembly, opcode, bytes, cycle_count, cycle_notes);
      }
    }

    fprintf(tf1, "\\hline\n");
    // d = 65CE02 delete idle cycles when CPU >2MHz
    // m = 4510 delete non-bus cycles
    if (branch_note_seen) {
      fprintf(tf1, " \\multicolumn{1}{r}{$b$} & \\multicolumn{11}{l}{Add one cycle if branch is taken.} \\\\\n");
      fprintf(tf1, " \\multicolumn{1}{r}{   } & \\multicolumn{11}{l}{Add one more cycle if branch taken crosses a page boundary.} "
             "\\\\\n");
    }
    if (delmodify65ce02_note_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$d$} & \\multicolumn{11}{l}{Subtract one cycle when CPU is at 3.5MHz. } \\\\\n");
    if (indirect_note_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$i$} & \\multicolumn{11}{l}{Add one cycle if clock speed is at 40 MHz.} \\\\\n");
    if (delidle4510_note_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$m$} & \\multicolumn{11}{l}{Subtract non-bus cycles when at 40MHz. } \\\\\n");
    if (page_note_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$p$} & \\multicolumn{11}{l}{Add one cycle if indexing crosses a page boundary.} \\\\\n");
    if (read_note_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$r$} & \\multicolumn{11}{l}{Add one cycle if clock speed is at 40 MHz.} \\\\\n");
    if (single_cycle_seen)
      fprintf(tf1, " \\multicolumn{1}{r}{$s$} & \\multicolumn{11}{l}{Instruction requires 2 cycles when CPU is run at 1MHz or "
             "2MHz.} \\\\\n");

    fprintf(tf1, "\\end{tabular}\n\\end{center}\n");
  }
  fclose(tf1);

  // generate new combined two-page opcode table, but not for 45GS02
  if (CPU != 45) {
    snprintf(filename, 1024, "opcodetable-%s.tex", processor);
    FILE* tf1 = fopen(filename, "wb");
    if (!tf1) {
      fprintf(stderr, "ERROR: cycle file \"%s\" not found!\n\n", filename);
      exit(1);
    }
    fprintf(tf1, "\\cleartoleftpage\n\\label{sec:opctable%s}\n", processor);
    for (int side = 0; side < 2; side++) {
      if (side == 0)
        fprintf(tf1, "\\begin{center}\n"
                    "{\\bf Opcode Table %s}\n"
                    "\\begin{tabular}{c|c|c|c|c|c|c|c|c|}\n"
                    "\\cline{2-9}\n"
                    "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 \\\\ \\hline\n", CPU==4510 ? "4510/45GS02" : processor);
      else
        fprintf(tf1, "\\begin{center}\n"
                    "{\\bf Opcode Table %s}\n"
                    "\\begin{tabular}{|c|c|c|c|c|c|c|c|c}\n"
                    "\\cline{1-8}\n"
                    "\\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC & \\$xD & \\$xE & \\$xF & \\\\ \\hline\n", CPU==4510 ? "4510/45GS02" : processor);
      for (int i = 0; i < 16; i++) {
        if (side == 0)
          fprintf(tf1, "\\multicolumn{1}{|c|}{\\$%Xx} & ", i);
        for (int j = side*8; j < (side + 1)*8; j++) {
          int opc_offset = i * 16 + j;
          int m = opcodes[opc_offset].modeId;
          fprintf(tf1, "%s\\OPC%s{%s}{%s}{%d}{%d%s} %s",
            opcodes[opc_offset].isUnintended ? "\\OPill" : 
              ((opcodes[opc_offset].isFar && opcodes[opc_offset].isFarQuad && opcodes[opc_offset].isQuad) ? "\\OPfarq" : 
                (opcodes[opc_offset].isQuad ? "\\OPquad" : 
                  (opcodes[opc_offset].isFar ? "\\OPfar" : ""))),
            (opcodes[opc_offset].isFarQuad || opcodes[opc_offset].isQuad) ? "Q" : "",
            opcodes[opc_offset].defined ? opcodes[opc_offset].abbrev : "???", // opcode
            modeinfo[m].shortDesc,
            modeinfo[m].bytes, //bytes
            opcodes[opc_offset].cycle_count, opcodes[opc_offset].cycle_notes,
            j!=7 && j!=15 ? "& ": "");
        }
        if (side == 1)
          fprintf(tf1, " & \\multicolumn{1}{c|}{\\$%Xx} \\\\ \\hline\n", i);
        else
          fprintf(tf1, " \\\\ \\hline\n");
      }
      fprintf(tf1, "\\end{tabular}\n\\end{center}\n\n");
    }
    fclose(tf1);
  }

  // skip old tables
  if (0) {
    snprintf(filename, 1024, "%s-opcodes.tex", processor);
    FILE* tf = fopen(filename, "wb");
    if (tf) {
      fprintf(tf, "\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|}\n");
      fprintf(tf, "\\cline{2-17}\n");
      fprintf(tf, "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 & \\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC "
                  "& \\$xD & \\$xE & \\$xF \\\\ \\hline\n");
      for (int i = 0; i < 16; i++) {
        fprintf(tf, "\\multicolumn{1}{|l|}{\\$%Xx} ", i);
        for (int j = 0; j < 16; j++)
          if (i * 16 + j < 256)
            fprintf(tf, "& %s     ", instruction_names[i * 16 + j] ? instruction_names[i * 16 + j] : "??");
          else
            fprintf(tf, "& ");
        fprintf(tf, "     \\\\ \\hline\n");
      }
      fprintf(tf, "\\end{tabular}\n");
      fclose(tf);
    }

    snprintf(filename, 1024, "%s-cycles.tex", processor);
    tf = fopen(filename, "wb");
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

    snprintf(filename, 1024, "%s-modes.tex", processor);
    tf = fopen(filename, "wb");
    if (tf) {
      fprintf(tf, "\\begin{tabular}{l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|l|}\n");
      fprintf(tf, "\\cline{2-17}\n");
      fprintf(tf, "& \\$x0 & \\$x1 & \\$x2 & \\$x3 & \\$x4 & \\$x5 & \\$x6 & \\$x7 & \\$x8 & \\$x9 & \\$xA & \\$xB & \\$xC "
                  "& \\$xD & \\$xE & \\$xF \\\\ \\hline\n");
      for (int i = 0; i < 16; i++) {
        fprintf(tf, "\\multicolumn{1}{|l|}{\\$%Xx} ", i);
        for (int j = 0; j < 16; j++) {
          int m = opcodes[i * 16 + j].modeId;
          fprintf(tf, "& %s     ", m>=0?modeinfo[m].texmode:"???");
        }
        fprintf(tf, "     \\\\ \\hline\n");
      }

      fprintf(tf, "\\end{tabular}\n");
      fclose(tf);
    }
  }

  fprintf(stderr, "\n");
}
