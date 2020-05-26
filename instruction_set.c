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
char *modes[MAX_MODES];
int mode_count=0;

char *instrs[256];
int instruction_count=0;

struct opcode opcodes[256];

static int compar_str(const void *a, const void *b)
{
  return strcmp(* (char * const *) a, * (char * const *) b);
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
	modes[mode_count++]=strdup(mode);
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
    for(int j=0;j<256;j++) {
      if (opcodes[j].instr_num==i) {
	fprintf(stderr,"  $%02x %s\n",
		j,opcodes[j].mode);
      }
    }
  }
  
}
