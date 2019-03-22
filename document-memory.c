/*
  Automatically generate documentation about memory addresses of the MEGA65.

  The idea is that it scans the VHDL source for the @IO: magic comments that
  we already have everywhere, and generates register tables from those.
  
  To do this in a really nice way, it needs to not only do the C64/C65/M65
  differentiation, but also have some kind of convention for indicating which
  table each should appear in, names for the signals for showing in the 
  compact register table, and then a mechanism for allowing longer blocks
  of documentation to follow, that can be put in the signal explanations
  below the table.

  The overall goal is to produce the sections of the User's Guide that 
  mirror the chip information chapters of the C64 Programmer's Reference Guide,
  in a way that pulls directly from the source code, so that it never goes out
  of date, and removes the need to separately maintain the two, which is of
  course simply a recipe for neglect.  It's "creative laziness" or "work smarter,
  not harder" thinking at work.

  So, the key changes we need are to have a more structured approach to the @IO:
  comment lines to indicate which table they should appear in. To appear in multiple
  tables, just have multiple comments.

  The new format is:

  @IO:XX <address spec> TABLE:SIGNAL Description

  <address spec> can be $NNNN or $NNNN-$NNNN or $NNNN.B or $NNNN.B-B
  (or $NNNNNNN in place of $NNNN)

  All signals with the same TABLE will appear in the same table file, called
  reginfo_TABLE.tex
  The SIGNAL tags will be used for populating the relevant bits in the table
  rows.
  The description will then appear in a column on the right side of the tables.

  Extended comments for SIGNALs will take the form:

  @INFO:TABLE:SIGNAL

  And all following lines of the same comment block in the VHDL will produce a
  \subsubsection{SIGNAL - Description}
  following the table, that has more in-depth information about the signal,
  possibly including examples etc. It will be treated as raw input for the 
  latex.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define MODE_C64 1
#define MODE_C65 2
#define MODE_MEGA65 3

struct reg_line {
  unsigned int low_address,high_address;
  unsigned int low_bit,high_bit;
  char *signal;
  char *description;

};

struct info_block {
  char *signal;
  char *description;
  int line_count;
#define MAX_LINES 1024
  char *lines[MAX_LINES];
};

struct reg_table {
  char *name;
  int mode;
  #define MAX_ENTRIES 1024
  struct reg_line regs[MAX_ENTRIES];
  int reg_count;
  struct info_block info_blocks[MAX_ENTRIES];
  int info_count;
};

#define MAX_TABLES 1024
struct reg_table *reg_tables[MAX_TABLES];
int table_count=0;

int parse_io_line(char *line)
{
  int low_addr=0,high_addr=0,low_bit=0,high_bit=7;
  char mode[8192];
  char table[8192];
  char signal[8192];
  char *description;
  int n;
  int mode_num;

  // Get fields
  int ok=0;

  if (sscanf(line,"-- @IO:%[^ ] $%x.%d-%d %[^:]:%[^ ] %n",mode,&low_addr,&low_bit,&high_bit,table,signal,&n)==6) {
    ok=1; high_addr=low_addr;
  }
  else if (sscanf(line,"-- @IO:%[^ ] $%x.%d %[^:]:%[^ ] %n",mode,&low_addr,&low_bit,table,signal,&n)==5) {
    ok=1; high_addr=low_addr; high_bit=low_bit;
  }
  else if (sscanf(line,"-- @IO:%[^ ] $%x-$%x %[^:]:%[^ ] %n",mode,&low_addr,&high_addr,table,signal,&n)==5) {
    ok=1;
  }
  else if (sscanf(line,"-- @IO:%[^ ] $%x - $%x %[^:]:%[^ ] %n",mode,&low_addr,&high_addr,table,signal,&n)==5) {
    ok=1;
  }
  else if (sscanf(line,"-- @IO:%[^ ] $%x %[^:]:%[^ ] %n",mode,&low_addr,table,signal,&n)==4) {
    ok=1; high_addr=low_addr;
  }
  
  if (!ok) {
    fprintf(stderr,"ERROR: @IO line missing TABLE:SIGNAL descriptor or otherwise mal-formed\n");
    return -1;
  }
  
  description=&line[n];

  // Make sure TABLE has no spaces, which would indicate a mal-formed entry
  for(int i=0;table[i];i++)
    if (table[i]==' ') {
      fprintf(stderr,"ERROR: @IO line missing TABLE:SIGNAL descriptor\n");
      return -1;
    }

  mode_num=-1;
  if (!strcmp(mode,"C64")) mode_num=MODE_C64;
  if (!strcmp(mode,"C65")) mode_num=MODE_C65;
  if (!strcmp(mode,"GS")) mode_num=MODE_MEGA65;
  if (mode_num==-1) {
    fprintf(stderr,"ERROR: Could not parse machine mode '%s'\n",mode);
    return -1;
  }

  // Search for existing table
  int table_num=0;
  for(;table_num<table_count;table_num++)
    {
      if ((!strcmp(reg_tables[table_num]->name,table))
	  &&(mode_num==reg_tables[table_num]->mode)) {
	break;
      }
    }
  if (table_num>=table_count) {
    if (table_num>=MAX_TABLES) {
      fprintf(stderr,"ERROR: Too many tables. Fix or increase MAX_TABLES.\n");
      return -1;
    }
    reg_tables[table_num]=calloc(sizeof(struct reg_table),1);
    if (!reg_tables[table_num]) {
      fprintf(stderr,"ERROR: Could not allocate new register table.\n");
      return -1;
    } 
    reg_tables[table_num]->name=strdup(table);
    reg_tables[table_num]->mode=mode_num;
    table_count++;
  }

  // Add entry to table
  if (reg_tables[table_num]->reg_count>=MAX_ENTRIES) {
    fprintf(stderr,"ERROR: Too many entries in register table '%s' for mode '%s'\n",
	    table,mode);
    return -1;
  }
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].low_address=low_addr;
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].high_address=high_addr;
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].low_bit=low_addr;
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].high_bit=high_bit;
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].signal=strdup(signal);
  reg_tables[table_num]->regs[reg_tables[table_num]->reg_count].description=strdup(description);

  reg_tables[table_num]->reg_count++;    
  
  return 0;
}

int scan_vhdl_file(char *file)
{
  int retVal=0;
  int error_count=0;
  
  struct info_block *current_info_block=NULL;
  
  do {
    char line[8192];
    int line_num=0;
    FILE *f=fopen(file,"r");
    if (!f) {
      fprintf(stderr,"Could not open input file '%s' for reading.\n",file);
      perror("fopen()");
      retVal=-1; break;
    }
    line[0]=0; fgets(line,8192,f);
    while(line[0]) {
      int offset=0;

      line_num++;
      
      // Skip over leading white space
      while(line[offset]==' '||line[offset]=='\t') offset++;
      // Trim CRLF
      while((line[strlen(line)-1]=='\r')||(line[strlen(line)-1]=='\n'))
	line[strlen(line)-1]=0;

      // Look for comments beginning anywhere in the lin
      int is_comment=0;
      for (;line[offset]&&line[offset+1];offset++) {
	if (line[offset]=='-'&&line[offset+1]=='-') {
	  // It's a comment, so do something!
	  
	  is_comment=1;
	  if (!strncmp(&line[offset],"-- @IO:",7))  {
	    // Register short description
	    if (parse_io_line(&line[offset])) {
	      fprintf(stderr,"%s:%d: ",file,line_num);
	      fprintf(stderr,"Error parsing @IO comment: '%s'\n",&line[offset]);
	      error_count++;
	    }
	  } else if (!strncmp(&line[offset],"-- @INFO:",9))  {
	    // Beginning of an info block
	    char table[8192],signal[8192];
	    int n;
	    if (sscanf(&line[offset],"-- @INFO:%[^:]:%[^: ]%n",table,signal,&n)==2) {
	      if (line[offset+n]) {
		fprintf(stderr,"%s:%d: ",file,line_num);
		fprintf(stderr,"Malformed @INFO: line: '%s'\n",line);
		retVal=-1; break;
	      }
	      // We have a valid info block.
	      // Check if it is a duplicate
	      int info_block_num=0;
	      struct reg_table *rt=NULL;
	      int table_num=0;
	      for(table_num=0;table_num<table_count;table_num++) {
		if (!strcmp(reg_tables[table_num]->name,table)) break;
	      }
	      if (table_num>=table_count) {
		// Need to allocate new table
		if (table_count>=MAX_TABLES) {
		  fprintf(stderr,"%s:%d: ",file,line_num);
		  fprintf(stderr,"Too many register tables defined.  Fix or increase MAX_TABLES.\n");
		  retVal=-1; break;
		}
		else {
		  reg_tables[table_num]=calloc(sizeof(struct reg_table),1);
		  if (!reg_tables[table_num]) {
		    fprintf(stderr,"%s:%d: ",file,line_num);
		    fprintf(stderr,"Failed to allocate new reg_table structure\n");
		    retVal=-1; break;
		  }
		  reg_tables[table_num]->name=strdup(table);
		  reg_tables[table_num]->reg_count=0;
		  reg_tables[table_num]->info_count=0;
		  table_count++;
		}
	      }
	      rt=reg_tables[table_num];
 
	      for(info_block_num=0;info_block_num<rt->info_count;info_block_num++) {
		if (!strcmp(signal,rt->info_blocks[info_block_num].signal)) {
		  // Found the block
		  break;
		}
	      }
	      if (info_block_num>=rt->info_count) {
		// New info block, so allocate it
		if (info_block_num>=MAX_ENTRIES) {
		  fprintf(stderr,"%s:%d: ",file,line_num);
		  fprintf(stderr,"Too many information blocks defined in table '%s'.  Fix or increase MAX_ENTRIES.\n",
			  table);
		  retVal=-1; break;
		}
		rt->info_blocks[info_block_num].signal=strdup(signal);
		rt->info_blocks[info_block_num].line_count=0;
		rt->info_count++;
	      }
	      // Set current info block to which we record comments
	      current_info_block=&rt->info_blocks[info_block_num];
	    }
	  } else {
	    // NOT an @IO: comment block
	    if (current_info_block) {
	      // Append comment line to info block
	      if (current_info_block->line_count>=MAX_LINES) {
		fprintf(stderr,"%s:%d: ",file,line_num);
		fprintf(stderr,"Too many lines in info block '%s'.  Fix or increase MAX_LINES.\n",
			current_info_block->signal);
		retVal=-1; break;
	      }
	      current_info_block->lines[current_info_block->line_count++]=strdup(&line[offset+3]);
	    }
	  }
	  break;
	}
      }
      if (!is_comment) {
	// It's not a comment. So do nothing, except for mark the end of
	// a multi-line info block, if required.
	current_info_block=NULL;	
      }

      if (retVal) break;
      
      line[0]=0; fgets(line,8192,f);
    }

    fclose(f);

  } while(0);

  if (error_count) {
    fprintf(stderr,"WARNING: Encountered %d errors while parsing '%s'\n",error_count,file);
  }    
  
  return retVal;  
}

int main(int argc,char **argv)
{
  for(int i=1;i<argc;i++)
    scan_vhdl_file(argv[i]);

  printf("%d tables defined.\n",table_count);
}
  
