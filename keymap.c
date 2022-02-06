/*
  Generate Unicode key mapping tables from the VHDL source of the keyboard
  mapper, so that people can see how to generate the various keys.

  Also generate reverse diagrams that show the key you will get if you 
  use various combinations.

*/

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

char *keys[72]={
		"\\specialkey{INST\\\\DEL}",
		"3",
		"5",
		"7",
		"9",
		"+",
		"\\pounds",
		"1",
		"\\specialkey{NO\\\\SCROLL}",
		"\\specialkey{RETURN}",
		"W",
		"R",
		"Y",
		"I",
		"P",
		"*",
		"$\\leftarrow$",
		"\\specialkey{TAB}",
		"\\megakey{$\\rightarrow$}",
		"A",
		"D",
		"G",
		"J",
		"L",
		";",
		"\\specialkey{CTRL}",
		"\\specialkey{ALT}",
		"\\megakey{F7}",
		"4",
		"6",
		"8",
		"0",
		"-",
		"\\specialkey{CLR\\\\HOME}",
		"2",
		"\\specialkey{HELP}",
		"\\megakey{F1}",
		"Z",
		"C",
		"B",
		"M",
		".",
		"\\specialkey{SHIFT\\\\right}",
		"\\megakey{SPC}",
		"\\megakey{F9}",
		"\\megakey{F3}",
		"S",
		"F",
		"H",
		"K",
		":",
		"=",
		"\\megasymbolkey",
		"\\megakey{F11}",
		"\\megakey{F5}",
		"E",
		"T",
		"U",
		"O",
		"@",
		"\\megakey{$\\uparrow$}",
		"Q",
		"\\megakey{F13}",
		"\\megakey{$\\downarrow$}",
		"\\specialkey{SHIFT\\\\left}",
		"X",
		"V",
		"N",
		",",
		"/",
		"\\specialkey{RUN STOP}",
		"\\specialkey{ESC}"
};

int keyid(int k)
{
  // Rotate keyboard matrix
  int o=k/8;
  o+=(k%8)*9;
  return o;
}

int main(int argc,char **argv)
{
  FILE *f=fopen("../mega65-core/src/vhdl/matrix_to_ascii.vhdl","r");
  if (!f) {
    fprintf(stderr,"ERROR: Could not read ../mega65-core/src/vhdl/matrix_to_ascii.vhdl.\n");
    exit(-1);
  }

  int mapping[72];
  
  // Clear mapping ready for next table
  for(int i=0;i<72;i++) mapping[i]=0;

  int table=0;
  char line[1024];
  line[0]=0; fgets(line,1024,f);
  while(line[0]) {

    if (strstr(line,"signal matrix_normal : key_matrix_t"))
      table=1;
    if (strstr(line,"signal matrix_shift : key_matrix_t"))
      table=2;
    if (strstr(line,"signal matrix_control : key_matrix_t"))
      table=3;
    if (strstr(line,"signal matrix_cbm : key_matrix_t"))
      table=4;
    if (strstr(line,"signal matrix_alt : key_matrix_t"))
      table=5;
    if (strstr(line,");")) {
      // End of table
      if (table) {
	fprintf(stderr,"End of table %d\n",table);
	
	char filename[1024];
	snprintf(filename,1024,"keymap_table_%d.tex",table);
	FILE *o=fopen(filename,"w");
	fprintf(o,"{\\ttfamily\n"
		"{\n"
		"\\setlength{\\def\\arraystretch{1.5}\\tabcolsep}{1mm}\n"
		"\\begin{center}\n"
		"\\begin{tabular}{|l{3cm}|c{2cm}|l{2cm}|}\n"
		"\\hline\n"
		"& \\bf{Key} & \\bf{Output} & \\bf{Unicode Glyph}  \\\\\n");
	
	for(int k=0;k<72;k++) {
	  fprintf(o,"\\hline\n"
		  "\\small %s & \\$%02X & \\char\"%04X \\\\\n",
		  keys[keyid(k)],mapping[k],mapping[k]);
	  
	}
	
	fprintf(o,"\\hline\n"
		"\\end{tabular}\n"
		"\\end{center}\n"
		"}}\n");
	fclose(o);
	
	// Clear mapping ready for next table
	for(int i=0;i<72;i++) mapping[i]=0;
	table=0;
      }
    }
    int key_num,unicode;
    if (sscanf(line,"%d => x\"%x",&key_num,&unicode)==2)
      {
	//	fprintf(stderr,"Key %d maps to $%02X\n",key_num,unicode);
	mapping[key_num]=unicode;
      }
    
    line[0]=0; fgets(line,1024,f);
  }
  
  fclose(f);
}
