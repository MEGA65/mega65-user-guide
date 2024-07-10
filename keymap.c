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
		"\\megakey{3}",
		"\\megakey{5}",
		"\\megakey{7}",
		"\\megakey{9}",
		"\\megakey{+}",
		"\\megakey{\\pounds}",
		"\\megakey{1}",
		"\\specialkey{NO\\\\SCROLL}",
		"\\specialkey{RETURN}",
		"\\megakey{W}",
		"\\megakey{R}",
		"\\megakey{Y}",
		"\\megakey{I}",
		"\\megakey{P}",
		"\\megakey{*}",
		"\\megakey{$\\leftarrow$}", // NOT cursor left!
		"\\specialkey{TAB}",
		"\\megakey{$\\rightarrow$}",
		"\\megakey{A}",
		"\\megakey{D}",
		"\\megakey{G}",
		"\\megakey{J}",
		"\\megakey{L}",
		"\\megakey{;}",
		"\\specialkey{CTRL}",
		"\\specialkey{ALT}",
		"\\megakey{F7}",
		"\\megakey{4}",
		"\\megakey{6}",
		"\\megakey{8}",
		"\\megakey{0}",
		"\\megakey{-}",
		"\\specialkey{CLR\\\\HOME}",
		"\\megakey{2}",
		"\\specialkey{HELP}",
		"\\megakey{F1}",
		"\\megakey{Z}",
		"\\megakey{C}",
		"\\megakey{B}",
		"\\megakey{M}",
		"\\megakey{.}",
		"\\specialkey{SHIFT\\\\right}",
		"\\megakey{SPC}",
		"\\megakey{F9}",
		"\\megakey{F3}",
		"\\megakey{S}",
		"\\megakey{F}",
		"\\megakey{H}",
		"\\megakey{K}",
		"\\megakey{:}",
		"\\megakey{=}",
		"\\megasymbolkey",
		"\\megakey{F11}",
		"\\megakey{F5}",
		"\\megakey{E}",
		"\\megakey{T}",
		"\\megakey{U}",
		"\\megakey{O}",
		"\\megakey{@}",
		"\\megakey{$\\uparrow$}",
		"\\megakey{Q}",
		"\\megakey{F13}",
		"\\megakey{$\\downarrow$}",
		"\\specialkey{SHIFT\\\\left}",
		"\\megakey{X}",
		"\\megakey{V}",
		"\\megakey{N}",
		"\\megakey{,}",
		"\\megakey{/}",
		"\\specialkey{RUN\\\\STOP}",
		"\\specialkey{ESC}"
};

int unimap[256][16];


int keyid(int k)
{
  // Rotate keyboard matrix
  int o=k/8;
  o+=(k%8)*9;
  return o;
}

int main(int argc,char **argv)
{
  FILE *f;

  if (argc != 2) {
    fprintf(stderr, "Usage: keymap MATRIX2ASCIIVHDL\n");
    exit(-1);
  }

  f=fopen(argv[1],"r");

  if (!f) {
    fprintf(stderr,"ERROR: Could not read ../mega65-core/src/vhdl/matrix_to_ascii.vhdl.\n");
    exit(-1);
  }

  int mapping[72];

  // Clear reverse map info
  for(int i=0;i<256;i++) for(int j=0;j<16;j++) unimap[i][j]=-1;

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
		"\\begin{tabular}{|l|c|l||l|c|l||l|c|l|}\n"
		"\\hline\n"
		"\\bf{Key} & \\bf{Code} & \\bf{Unicode} &"
		"\\bf{Key} & \\bf{Code} & \\bf{Unicode} &"
		"\\bf{Key} & \\bf{Code} & \\bf{Unicode}"
		"  \\\\\n");

	for(int k=0;k<72;k++) {
	  if (!(k%3)) fprintf(o,"\\hline\n");
	  fprintf(o,
		  "\\small %s & \\$%02X & \\char\"%04X",
		  keys[keyid(k)],mapping[k],mapping[k]);
	  if ((k%3)==2) {
	    fprintf(o," \\\\\n");
	  } else {
	    fprintf(o," & ");
	  }

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

	// ALT+ combos take priority in the table, as they are used
	// for more logical mapping to international keys, while
	// we also keep the MEGA combos to match PETSCII.
	if ((unimap[unicode][0]==-1)||((unimap[unicode][0]>>8)!=5))
	  unimap[unicode][0]=(table<<8)+key_num;
      }

    line[0]=0; fgets(line,1024,f);
  }

  fclose(f);

  FILE *o=fopen("unicode_mapping.tex","w");

  for(int p=0;p<0x100;p+=0x40) {

    fprintf(o,"{\\ttfamily\n"
	    "{\n"
	    "\\setlength{\\def\\arraystretch{1.5}\\tabcolsep}{1mm}\n"
	    "\\begin{center}\n"
	    "\\begin{tabular}{|r|r|r|r|r|r|r|r|r|}\n"
	    "\\hline\n"
	    " & & \\bf{+\\$%02X} & & \\bf{+\\$%02X} & & \\bf{+\\$%02X} & & \\bf{+\\$%02X}"
	    "  \\\\\n",
	    p,p+0x10,p+0x20,p+0x30);

    for(int k=0;k<16;k++) {
      fprintf(o,"\\hline\n\\small \\$0%x & ",k);
      for(int x=0;x<4;x++) {
	int c=k+x*0x10+p;
	fprintf(o," \\char\"%04X & ",c);
	if (unimap[c][0]!=-1) {
	  switch(unimap[c][0]>>8) {
	  case 1: // unmodified
	    break;
	  case 2: // shift
	    fprintf(o,"\\specialkey{SHIFT} +");
	    break;
	  case 3: // CTRL
	    fprintf(o,"\\specialkey{CTRL} +");
	    break;
	  case 4: // MEGA
	    fprintf(o,"\\megasymbolkey +");
	    break;
	  case 5: // ALT
	    fprintf(o,"\\specialkey{ALT} +");
	    break;
	  }
	  fprintf(o," %s ",keys[keyid(unimap[c][0]&0xff)]);
	}
	if (x!=3) fprintf(o," & ");
      }
      fprintf(o," \\\\\n");
    }

    fprintf(o,"\\hline\n"
	    "\\end{tabular}\n"
	    "\\end{center}\n"
	    "}}\n\n");
  }

  fclose(o);
}
