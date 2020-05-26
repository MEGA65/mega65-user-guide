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
struct modeinfo {
  char *description;
  int bytes;
  int cycles;
  char *cycle_notes;
  char *memory_equation;
  char *long_description;
  char *long_title;
};

struct modeinfo modeinfo[MAX_MODES];
char *modes[MAX_MODES];
int mode_count=0;

char *instrs[256];
int instruction_count=0;

struct opcode opcodes[256];

static int compar_str(const void *a, const void *b)
{
  return strcmp(* (char * const *) a, * (char * const *) b);
}

void lookup_mode_description(int m)
{
  fprintf(stderr,"Looking for description of '%s'\n",modes[m]);

  // Normalise mode into a safe filename
  char n[256];
  int nlen=0;
  for(int i=0;i<255&&modes[m][i];i++) {
    switch(modes[m][i]) {
    case '(':
    case '#':
    case '$':
    case ',':
    case ')':
      n[nlen++]='_'; break;
    default:
      n[nlen++]=modes[m][i];
    }
  }
  n[nlen]=0;
  fprintf(stderr,"Normalised to '%s'\n",n);

  char filename[1024];
  snprintf(filename,1024,"instruction_sets/mode.%s",n);
  FILE *f=fopen(filename,"rb");
  if (f) {
    char line[8192];
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].description=strdup(line);
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].bytes=atoi(line);
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].cycles=atoi(line);
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].cycle_notes=strdup(line);
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].memory_equation=strdup(line);
    fgets(line,1024,f);
    while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
    modeinfo[m].long_title=strdup(line);
    int r=fread(line,1,8192,f);
    line[r]=0;
    modeinfo[m].long_description=strdup(line);
    
    fclose(f);
  } else {
    fprintf(stderr,"WARNING: Could not find mode description file '%s'\n",filename);
    bzero(&modeinfo[m],sizeof(struct modeinfo));
  }
  
  return;
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
      int i=99;
      for(i=0;i<mode_count;i++) {
	if (!strcmp(modes[i],mode)) {
	  break;
	}
      }
      if (i<mode_count) opcodes[opcode].mode_num=i;
      else {
	opcodes[opcode].mode_num=mode_count;	
	modes[mode_count]=strdup(mode);

	// Try to find better description and data
	lookup_mode_description(mode_count);

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
	opcodes[opcode].instr_num=instruction_count;
	instrs[instruction_count++]=strdup(name);
      }
    }
    
    opcodes[opcode].abbrev=strdup(name);
    
    line[0]=0; fgets(line,1024,f);
  }

  fprintf(stderr,"%d addressing modes found.\n",mode_count);
  fprintf(stderr,"%d unique instructions found.\n",instruction_count);  

  for(int i=0;i<mode_count;i++) {
    fprintf(stderr,"Modes #%d : %s\n",i,modeinfo[i].description);
  }
  
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
    printf("\\begin{tabular}{|llllllllllll|}\n\\hline\n"
	   "%s &  & \\multicolumn{9}{l}{%s} & \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & \\multicolumn{2}{l}{%s}  &                             & N       & Z      & I      & C       & D       & V      & E      \\\\\n"
	   "&  &                 &           &                             & %s   & %s  & %s  & %s   & %s   & %s  & %s  \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & {\\underline{Addressing Mode}} & {\\underline{Assembly}} & \\multicolumn{1}{c}{\\underline{Op-Code}} & \\multicolumn{3}{c}{\\underline{Bytes}} & \\multicolumn{3}{c}{\\underline{Cycles}}       &   \\\\\n",
	   instruction,description,action,
	   nflag,zflag,iflag,cflag,dflag,vflag,eflag
	   );
    
    for(int j=0;j<256;j++) {
      if (opcodes[j].instr_num==i) {
	int m=opcodes[j].mode_num;
	if (m<0) m=0;
	fprintf(stderr,"  $%02x %d,%d\n",
		j,m,opcodes[j].mode_num);
	char *addressing_mode=modeinfo[m].description?modeinfo[m].description:"No description";
	char assembly[1024]="LDA \\$1234";
	snprintf(assembly,1024,"%s ",instruction);
	for(int j=0;modes[m][j];j++) {
	  // Escape tricky chars
	  switch (modes[m][j]) {
	  case '$': case '#': 
	    assembly[strlen(assembly)+1]=0;
	    assembly[strlen(assembly)]='\\';
	  }
	  assembly[strlen(assembly)+1]=0;
	  assembly[strlen(assembly)]=modes[m][j];
	}
	char opcode[16]="A9";
	snprintf(opcode,16,"%02X",j);
	char bytes[16]="3";
	snprintf(bytes,16,"%d",modeinfo[m].bytes);
	char cycles[16]="4";
	snprintf(cycles,16,"%d",modeinfo[m].cycles);
	char *cycle_notes=modeinfo[m].cycle_notes?modeinfo[m].cycle_notes:"";
	if (opcodes[j].mode_num==-1) {
	  // Implied mode is handled separately
	  addressing_mode="implied";
	  snprintf(bytes,16,"1");
	  snprintf(cycles,16,"1");
	  assembly[0]=0;
	  cycle_notes="";

	  // XXX Replace this with reading data from instruction description files
	  if (instruction[0]=='P') {
	    // Push/Pop = 2 cycles
	    snprintf(cycles,16,"2");
	  } else if (!strcmp("RTS",instruction)) {
	    snprintf(cycles,16,"3");
	  } else if (!strcmp("RTI",instruction)) {
	    snprintf(cycles,16,"4");
	  }
	}
	printf("&  & %s        & %s       & \\multicolumn{1}{c}{%s}     & \\multicolumn{3}{c}{%s} & \\multicolumn{3}{r}{%s} & %s\\\\\n",
	       addressing_mode,assembly,opcode,bytes,cycles,cycle_notes);
      }
    }
    printf("\\hline\n"
	   "\\end{tabular}\n");
    fflush(stdout);
        
  }
  
}
