#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct opcode {
  char *abbrev;
  char *mode;
  int mode_num;
  int instr_num;
};

#define MAX_MODES 32
char *mode_descriptions[MAX_MODES];
char *modes[MAX_MODES];
int mode_count=0;

char *instrs[256];
int instruction_count=0;

struct opcode opcodes[256];

static int compar_str(const void *a, const void *b)
{
  return strcmp(* (char * const *) a, * (char * const *) b);
}

char *lookup_mode_description(char *m)
{
  fprintf(stderr,"Looking for description of '%s'\n",m);

  // Normalise mode into a safe filename
  char n[256];
  int nlen=0;
  for(int i=0;i<255&&m[i];i++) {
    switch(m[i]) {
    case '(':
    case '#':
    case '$':
    case ',':
    case ')':
      n[nlen++]='_'; break;
    default:
      n[nlen++]=m[i];
    }
  }
  n[nlen]=0;
  fprintf(stderr,"Normalised to '%s'\n",n);

  char filename[1024];
  snprintf(filename,1024,"instruction_sets/mode.%s",n);
  FILE *f=fopen(filename,"rb");
  if (f) {
    char line[1024];
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    fclose(f);
    fprintf(stderr,"%s -> %s\n",m,line);
    return strdup(line);
  } else {
    fprintf(stderr,"Could not find mode description file '%s'\n",filename);
  }
  
  return NULL;
}

int main(int argc,char **argv)
{
  /* Read a list of opcodes. Gather them by instruction, and
     generate the various tables and things that we need from
     that. We also have one file per instruction that describes
     its function.
  */

  FILE *f=fopen(argv[1],"rb");
  char line[1024];
  line[0]=0; fgets(line,1024,f);
  while(line[0]) {
    int opcode;
    char name[1024];
    char mode[1024];
    int n=sscanf(line,"%x %s %[^\n]",&opcode,name,mode);
    if (n<2) {
      fprintf(stderr,"ERROR: Could not parse line: %s\n",line);
      exit(-3);
    }

    if (n==3) {
      int i;
      for(i=0;i<mode_count;i++) {
	if (!strcmp(modes[i],mode)) {
	  break;
	}
      }
      if (i<mode_count) opcodes[opcode].mode_num=i;
      else {
	mode_descriptions[mode_count]="No description";
	modes[mode_count]=strdup(mode);

	// Try to find better description
	char *d=lookup_mode_description(mode);
	if (d) {
	  mode_descriptions[mode_count]=d;
	  fprintf(stderr,"Setting mode description.\n");
	} else {
	  fprintf(stderr,"No mode description.\n");
	}

	mode_count++;
      }
      opcodes[opcode].mode=strdup(mode);      
    } else {
      opcodes[opcode].mode_num=-1;
      opcodes[opcode].mode="";
    }

    {
      int i=0;
      for(i=0;i<instruction_count;i++) {
	if (!strcmp(instrs[i],name)) {
	  break;
	}
      }
      if (i<instruction_count) opcodes[opcode].instr_num=i;
      else {
	instrs[instruction_count++]=strdup(name);
	opcodes[opcode].instr_num=instruction_count-1;
      }
    }
    
    opcodes[opcode].abbrev=strdup(name);
    
    line[0]=0; fgets(line,1024,f);
  }

  fprintf(stderr,"%d addressing modes found.\n",mode_count);
  fprintf(stderr,"%d unique instructions found.\n",instruction_count);  

  // Sort instruction names alphabetically
  fprintf(stderr,"Sorting instructions alphabetically.\n");
  qsort(&instrs[0],instruction_count,sizeof(char *),compar_str);
  // Now update the instruction numbers in the array
  for(int i=0;i<256;i++) {
    for(int j=0;j<instruction_count;j++) {
      if (!strcmp(opcodes[i].abbrev,instrs[j])) {
	opcodes[i].instr_num=j;
	break;
      }
    }
  }
  
  /* Now generate the instruction tables. 
   */
  for(int i=0;i<instruction_count;i++) {
    fprintf(stderr,"%s - No description\n",instrs[i]);

    char *instruction=instrs[i];
    char *description="No description";
    char *action="";
    char *nflag=".";
    char *zflag=".";
    char *iflag=".";
    char *cflag=".";
    char *dflag=".";
    char *vflag=".";
    char *eflag=".";

    printf("\n\n\\subsection*{%s}\n",instruction);
    fflush(stdout);
    printf("\\begin{table}[ht!]\n"
	   "\\begin{tabular}{|llllllllllll|}\n\\hline\n"
	   "%s &  & \\multicolumn{9}{l}{%s} & \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & \\multicolumn{2}{l}{%s}  &                             & N       & Z      & I      & C       & D       & V      & E      \\\\\n"
	   "&  &                 &           &                             & %s   & %s  & %s  & %s   & %s   & %s  & %s  \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & Addressing Mode & Assembler & \\multicolumn{1}{c}{Op-Code} & \\multicolumn{3}{c}{Bytes} & \\multicolumn{3}{c}{Cycles}       &   \\\\\n",
	   instruction,description,action,
	   nflag,zflag,iflag,cflag,dflag,vflag,eflag
	   );
    
    for(int j=0;j<256;j++) {
      if (opcodes[j].instr_num==i) {
	fprintf(stderr,"  $%02x %s\n",
		j,opcodes[j].mode);
	char *addressing_mode=mode_descriptions[opcodes[j].mode_num];
	char assembly[1024]="LDA \\$1234";
	char opcode[16]="A9";
	char bytes[16]="3";
	char cycles[16]="4";
	char cycle_notes[16]="*^";
	printf("&  & %s        & %s       & \\multicolumn{1}{c}{%s}     & \\multicolumn{3}{c}{%s} & \\multicolumn{3}{l}{%s} & %s\\\\\n",
	       addressing_mode,assembly,opcode,bytes,cycles,cycle_notes);
      }
    }
    printf("\\hline\n"
	   "\\end{tabular}\n"
	   "\\end{table}\n");
        
  }
  
}
