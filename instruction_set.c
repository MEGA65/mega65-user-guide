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

// Collected information for opcode table generation
char *cycle_count_list[256];
char *instructions[256];

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

  char processor[1024];
  char processor_path[1024];
  int pplen=0;
  int plen=0;
  for(int i=0;argv[1]&&argv[1][i]&&argv[1][i]!='.';i++)
    {
      processor[plen++]=argv[1][i];
      processor[plen]=0;
      if (argv[1][i]=='/') plen=0;
      processor_path[pplen++]=argv[1][i];
      processor_path[pplen]=0;
    }
  fprintf(stderr,"Processor name is '%s'\n",processor);

  char line[1024];
  
  strcat(processor_path,".cycles");
  FILE *cf=fopen(processor_path,"rb");
  if (cf)
  {
    fprintf(stderr,"Reading instruction cycle count information...\n");
    line[0]=0; fgets(line,1024,cf);
    while(line[0]) {
      int opcode;
      char instr[1024];
      char cycles[1024];
      int n=sscanf(line,"%x %s %[^\n]",&opcode,instr,cycles);
      if (n==3) {
	if (opcode>=0&&opcode<256)
	  cycle_count_list[opcode]=strdup(cycles);
      }
      line[0]=0; fgets(line,1024,cf);
    }
    fclose(cf);
  }
  
  FILE *f=fopen(argv[1],"rb");
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
    char *instruction=instrs[i];
    char *short_description="No description";
    char *long_description="No description";
    char *action="";
    char nflag[2]=".";
    char zflag[2]=".";
    char iflag[2]=".";
    char cflag[2]=".";
    char dflag[2]=".";
    char vflag[2]=".";
    char eflag[2]=".";
    int extra_cycles=0;

    char filename[1024];
    snprintf(filename,1024,"instruction_sets/inst.%s",instrs[i]);
    FILE *f=fopen(filename,"rb");
    char line[8192];
    if (f) {
      fprintf(stderr,"Using %s\n",filename);
      line[0]=0; fgets(line,1024,f);
      while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
      short_description=strdup(line);
      
      line[0]=0; fgets(line,1024,f);
      while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
      action=strdup(line);
      line[0]=0; fgets(line,1024,f);
      while(line[0]&&line[strlen(line)-1]<' ') line[strlen(line)-1]=0;
      for(int i=0;line[i];i+=2) {
	switch(line[i]) {
	case 'N': nflag[0]=line[i+1]; break;
	case 'V': vflag[0]=line[i+1]; break;
	case 'C': cflag[0]=line[i+1]; break;
	case 'D': dflag[0]=line[i+1]; break;
	case 'I': iflag[0]=line[i+1]; break;
	case 'E': eflag[0]=line[i+1]; break;
	case 'Z': zflag[0]=line[i+1]; break;
	case 'M': extra_cycles++; break;
	}
      }
      int r=fread(line,1,8192,f);
      line[r]=0;
      long_description=line;
      //      fflush(stdout);
      //      fprintf(stderr,"%s Long desc:\n%s\n\n\n\n",instruction,long_description);
      fclose(f);
    } else {
      fprintf(stderr,"WARNING: Could not read %s\n",filename);
    }
    fprintf(stderr,"%s - %s\n",instrs[i],short_description);	  

    printf("\n\n\\subsection*{%s}\n",instruction);
    printf("%s\n\n\n",long_description);

    int branch_note=0;
    int page_note=0;
    int read_note=0;
    
    printf("\\begin{tabular}{|llp{4cm}lllllllll|}\n\\hline\n"
	   "{\\bf %s} &  & \\multicolumn{6}{l}{\\bf %s} & \\multicolumn{4}{r|}{\\bf %s}    \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & \\multicolumn{3}{l}{%s}  & {\\bf N}       & {\\bf Z}      & {\\bf I}      & {\\bf C}       & {\\bf D}       & {\\bf V}      & {\\bf E}      \\\\\n"
	   "&  &                 &           &                             & %s   & %s  & %s  & %s   & %s   & %s  & %s  \\\\\n"
	   "&  &                 &           &                             &         &        &        &         &         &        &        \\\\\n"
	   "&  & {\\underline{\\bf Addressing Mode}} & {\\bf \\underline{Assembly}} & \\multicolumn{1}{c}{\\bf \\underline{Op-Code}} & \\multicolumn{3}{c}{\\bf \\underline{Bytes}} & \\multicolumn{3}{c}{\\bf \\underline{Cycles}}       &   \\\\\n",
	   instruction,short_description,processor,
	   action,
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
	snprintf(cycles,16,"%d",modeinfo[m].cycles+extra_cycles);
	instructions[m]=strdup(instruction);
	
	if (opcodes[j].mode_num==-1) {
	  // Implied mode is handled separately
	  addressing_mode="implied";
	  snprintf(bytes,16,"1");
	  snprintf(cycles,16,"1");
	  assembly[0]=0;

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

	char cycle_count[1024];
	char *cycle_notes;
	
	if (cycle_count_list[j]) {
	  cycle_count[0]=0;
	  for(int i=0;isdigit(cycle_count_list[j][i]);i++) {
	    {
	      cycle_count[i]=cycle_count_list[j][i];
	      cycle_count[i+1]=0;
	      cycle_notes=&cycle_count_list[j][i+1];
	    }
	  }
	} else {
	  snprintf(cycle_count,1024,"??"); cycle_notes="";
	}
	if (strstr(cycle_notes,"b")) branch_note=1;
	if (strstr(cycle_notes,"p")) page_note=1;
	if (strstr(cycle_notes,"r")) read_note=1;
	printf("&  & %s        & %s       & \\multicolumn{1}{c}{%s}     & \\multicolumn{3}{c}{%s} & \\multicolumn{3}{r}{%s} & %s \\\\\n",
	       addressing_mode,assembly,opcode,bytes,cycle_count,cycle_notes);

      }
    }    
    printf("\\hline\n");
    if (read_note) printf(" \\multicolumn{1}{r}{$r$} & \\multicolumn{11}{l}{Add one cycle if clock speed is at 40 MHz} \\\\\n");
    if (page_note) printf(" \\multicolumn{1}{r}{$p$} & \\multicolumn{11}{l}{Add one cycle if indexing crosses a page boundary.} \\\\\n");
    if (branch_note) printf(" \\multicolumn{1}{r}{$b$} & \\multicolumn{11}{l}{Add one cycle if branch crosses a page boundary.} \\\\\n");
    
    printf("\\end{tabular}\n");
    fflush(stdout);
        
  }
  
}
