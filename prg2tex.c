// *************************************************
// prg2tex - version 1.1 - Bit Shifter - 16-Jun-2020
// *************************************************

// =======
// include
// =======

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int UpperCase = 0; // command line option -u
int Debug = 1;

struct TokenStruct {
  int Token;
  const char* Name;
};

// clang-format off
struct TokenStruct TokenTab[] =
{
   {  0x80,"end"      },
   {  0x81,"for"      },
   {  0x82,"next"     },
   {  0x83,"data"     },
   {  0x84,"input#"   },
   {  0x85,"input"    },
   {  0x86,"dim"      },
   {  0x87,"read"     },
   {  0x88,"let"      },
   {  0x89,"goto"     },
   {  0x8a,"run"      },
   {  0x8b,"if"       },
   {  0x8c,"restore"  },
   {  0x8d,"gosub"    },
   {  0x8e,"return"   },
   {  0x8f,"rem"      },

   {  0x90,"stop"     },
   {  0x91,"on"       },
   {  0x92,"wait"     },
   {  0x93,"load"     },
   {  0x94,"save"     },
   {  0x95,"verify"   },
   {  0x96,"def"      },
   {  0x97,"poke"     },
   {  0x98,"print#"   },
   {  0x99,"print"    },
   {  0x9a,"cont"     },
   {  0x9b,"list"     },
   {  0x9c,"clr"      },
   {  0x9d,"cmd"      },
   {  0x9e,"sys"      },
   {  0x9f,"open"     },

   {  0xa0,"close"    },
   {  0xa1,"get"      },
   {  0xa2,"new"      },
   {  0xa3,"tab("     },
   {  0xa4,"to"       },
   {  0xa5,"fn"       },
   {  0xa6,"spc("     },
   {  0xa7,"then"     },
   {  0xa8,"not"      },
   {  0xa9,"step"     },
   {  0xaa,"+"        },
   {  0xab,"-"        },
   {  0xac,"*"        },
   {  0xad,"/"        },
   {  0xae,"^"        },
   {  0xaf,"and"      },

   {  0xb0,"or"       },
   {  0xb1,">"        },
   {  0xb2,"="        },
   {  0xb3,"<"        },
   {  0xb4,"sgn"      },
   {  0xb5,"int"      },
   {  0xb6,"abs"      },
   {  0xb7,"usr"      },
   {  0xb8,"fre"      },
   {  0xb9,"pos"      },
   {  0xba,"sqr"      },
   {  0xbb,"rnd"      },
   {  0xbc,"log"      },
   {  0xbd,"exp"      },
   {  0xbe,"cos"      },
   {  0xbf,"sin"      },

   {  0xc0,"tan"      },
   {  0xc1,"atn"      },
   {  0xc2,"peek"     },
   {  0xc3,"len"      },
   {  0xc4,"str$"     },
   {  0xc5,"val"      },
   {  0xc6,"asc"      },
   {  0xc7,"chr$"     },
   {  0xc8,"left$"    },
   {  0xc9,"right$"   },
   {  0xca,"mid$"     },
   {  0xcb,"go"       },
   {  0xcc,"rgr"      },
   {  0xcd,"rclr"     },
   {  0xcf,"joy"      },

   {  0xd0,"rdot"     },
   {  0xd1,"dec"      },
   {  0xd2,"hex$"     },
   {  0xd3,"err$"     },
   {  0xd4,"instr"    },
   {  0xd5,"else"     },
   {  0xd6,"resume"   },
   {  0xd7,"trap"     },
   {  0xd8,"tron"     },
   {  0xd9,"troff"    },
   {  0xda,"sound"    },
   {  0xdb,"vol"      },
   {  0xdc,"auto"     },
   {  0xdd,"pudef"    },
   {  0xde,"graphic"  },
   {  0xdf,"paint"    },

   {  0xe0,"char"     },
   {  0xe1,"box"      },
   {  0xe2,"circle"   },
   {  0xe3,"gshape"   },
   {  0xe4,"sshape"   },
   {  0xe5,"draw"     },
   {  0xe6,"locate"   },
   {  0xe7,"color"    },
   {  0xe8,"scnclr"   },
   {  0xe9,"scale"    },
   {  0xea,"help"     },
   {  0xeb,"do"       },
   {  0xec,"loop"     },
   {  0xed,"exit"     },
   {  0xee,"directory"},
   {  0xef,"dsave"    },

   {  0xf0,"dload"    },
   {  0xf1,"header"   },
   {  0xf2,"scratch"  },
   {  0xf3,"collect"  },
   {  0xf4,"copy"     },
   {  0xf5,"rename"   },
   {  0xf6,"backup"   },
   {  0xf7,"delete"   },
   {  0xf8,"renumber" },
   {  0xf9,"key"      },
   {  0xfa,"monitor"  },
   {  0xfb,"using"    },
   {  0xfc,"until"    },
   {  0xfd,"while"    },

   {0xce02,"pot"      },
   {0xce03,"bump"     },
   {0xce04,"pen"      },
   {0xce05,"rsppos"   },
   {0xce06,"rsprite"  },
   {0xce07,"rspcolor" },
   {0xce08,"xor"      },
   {0xce09,"rwindow"  },
   {0xce0a,"pointer"  },

   {0xfe02,"bank"     },
   {0xfe03,"filter"   },
   {0xfe04,"play"     },
   {0xfe05,"tempo"    },
   {0xfe06,"movspr"   },
   {0xfe07,"sprite"   },
   {0xfe08,"sprcolor" },
   {0xfe09,"rreg"     },
   {0xfe0a,"envelope" },
   {0xfe0b,"sleep"    },
   {0xfe0c,"catalog"  },
   {0xfe0d,"dopen"    },
   {0xfe0e,"append"   },
   {0xfe0f,"dclose"   },

   {0xfe10,"bsave"    },
   {0xfe11,"bload"    },
   {0xfe12,"record"   },
   {0xfe13,"concat"   },
   {0xfe14,"dverify"  },
   {0xfe15,"dclear"   },
   {0xfe16,"sprsav"   },
   {0xfe17,"collision"},
   {0xfe18,"begin"    },
   {0xfe19,"bend"     },
   {0xfe1a,"window"   },
   {0xfe1b,"boot"     },
   {0xfe1c,"width"    },
   {0xfe1d,"sprdef"   },
   {0xfe1e,"quit"     },
   {0xfe1f,"stash"    },

   {0xfe21,"fetch"    },
   {0xfe23,"swap"     },
   {0xfe24,"off"      },
   {0xfe25,"fast"     },
   {0xfe26,"slow"     }

};
// clang-format on

#define TOKENS (sizeof(TokenTab) / sizeof(struct TokenStruct))

// clang-format off
struct TokenStruct ControlTab[] =
{
   {0x02,"und_on"   }, // ctrl b  c5a2
   {0x05,"white"    }, // ctrl e  c5a5
   {0x07,"bell"     }, // ctrl g  c5a7
   {0x09,"tab"      },
   {0x0a,"newline"  },
   {0x0b,"lock"     },
   {0x0c,"unlock"   },
   {0x0d,"cr"       },
   {0x0e,"text"     },
   {0x0f,"blink_on" },

   {0x11,"down"     },
   {0x12,"rvs_on"   },
   {0x13,"home"     },
   {0x14,"del"      },
   {0x18,"settab"   },
   {0x1b,"esc"      },
   {0x1d,"right"    },
   {0x1f,"blue"     },

   {0x5c,"pound"    },

   {0x81,"orange"   },
   {0x82,"und_off"  },
   {0x84,"help"     },
   {0x85,"f1"       },
   {0x86,"f3"       },
   {0x87,"f5"       },
   {0x88,"f7"       },
   {0x89,"f2"       },
   {0x8a,"f4"       },
   {0x8b,"f6"       },
   {0x8c,"f8"       },
   {0x8d,"shift_cr" },
   {0x8e,"graphics" },
   {0x8f,"blink_off"},

   {0x90,"black"    },
   {0x91,"up"       },
   {0x92,"rvs_off"  },
   {0x93,"clear"    },
   {0x94,"ins"      },
   {0x95,"brown"    },
   {0x96,"lt_red"   },
   {0x97,"grey_1"   },
   {0x98,"grey_2"   },
   {0x99,"lt_green" },
   {0x9a,"lt_blue"  },
   {0x9b,"grey_3"   },
   {0x9c,"purple"   },
   {0x9d,"left"     },
   {0x9e,"yellow"   },
   {0x9f,"cyan"     },

   {0xad,"cbl"      },  // corner bottom left
   {0xae,"ctr"      },  // corner top    right
   {0xb0,"ctl"      },  // corner top    left
   {0xbd,"cbr"      },  // corner bottom right
   {0xc0,"hor"      },  // horizontal
   {0xdd,"ver"      },  // vertical
   {0xff,"pi"       }   // pi
};
// clang-format on

#define CONTROLS (sizeof(ControlTab) / sizeof(struct TokenStruct))

FILE* ap;
FILE* bp;
FILE* tp;

char A_file[256]; // ASCII file ".txt"
char B_file[256]; // BASIC file ".prg"
char T_file[256]; // LATEX file ".tex"

unsigned char Buf[256];     // BASIC line
unsigned char Det[512];     // BASIC line detokenized
unsigned short LoadAddress; // CBM load address
unsigned short Link;        // line link (unused)
unsigned short Numb;        // line #

int BASIC_Version;

// clang-format off
struct TokenStruct BasicVersion[] =
{
   {0x0401,"BASIC 4.0 [PET/CBM]"},
   {0x0801,"BASIC 2 [C64/VIC-20]"},
   {0x1001,"BASIC 3.5 [+4/C16]"},
   {0x1c01,"BASIC 7.0 [C128]"}
};
// clang-format on

#define BVERSIONS (sizeof(BasicVersion) / sizeof(struct TokenStruct))

unsigned char Ascii(unsigned char c)
{
  if (c >= 'A' && c <= 'Z')
    return tolower(c);
  if (c >= 'A' + 0x80 && c <= 'Z' + 0x80)
    return c & 0x7f;

  return c;
}

// **********
// DeTokenize
// **********

void DeTokenize(void)
{
  int i;
  int Token;
  int bi = 0; // Buf index
  int di = 0; // Det index

  memset(Det, 0, sizeof(Det));

  while (bi < sizeof(Buf) && Buf[bi]) {
    if (Buf[bi] == 0x8f) // rem token
    {
      strcpy((char*)Det + di, "rem");
      bi++;
      di = strlen((char*)Det);
      do
        Det[di++] = Ascii(Buf[bi++]);
      while (Buf[bi] && bi < sizeof(Buf) - 1 && di < sizeof(Det) - 1);
    }
    else if (Buf[bi] == '\"') // start of string
    {
      do
        Det[di++] = Ascii(Buf[bi++]);
      while (Buf[bi] && Buf[bi] != '\"' && bi < sizeof(Buf) - 1 && di < sizeof(Det) - 1);
      if (Buf[bi] == '\"')
        Det[di++] = Buf[bi++];
    }
    else if (Buf[bi] & 0x80) // token
    {
      Token = Buf[bi++];
      if (Token == 0xce || Token == 0xfe)
        Token = (Token << 8) | Buf[bi++];
      for (i = 0; i < TOKENS; ++i) {
        if (Token == TokenTab[i].Token) {
          strcat((char*)Det, TokenTab[i].Name);
          break;
        }
        if (i == TOKENS) {
          printf("*** unknown token %x ***\n", Token);
          exit(1);
        }
      }
      di = strlen((char*)Det);
    }
    else // normal text
    {
      Det[di++] = Ascii(Buf[bi++]);
    }
  }
}

// *************
// ReadBasicLine
// *************

int ReadBasicLine(void)
{
  unsigned char c;
  int l = 0; // length

  memset(Buf, 0, sizeof(Buf));

  fread(&Link, sizeof(Link), 1, bp);
  if (!Link)
    return 0; // end of program
  fread(&Numb, sizeof(Numb), 1, bp);

  do {
    fread(&c, 1, 1, bp);
    Buf[l++] = c;
  } while (c && l < sizeof(Buf) - 1 && !feof(bp));

  DeTokenize();
  return !feof(bp);
}

// ******
// PutSeq
// ******

// m : multiplier
// c : character

void PutSeq(int m, int c)
{
  int i;

  for (i = 0; i < CONTROLS; ++i) {
    if (c == ControlTab[i].Token) {
      if (m > 1)
        fprintf(ap, "{%d %s}", m, ControlTab[i].Name);
      else
        fprintf(ap, "{%s}", ControlTab[i].Name);
      return;
    }
  }
  if (m > 1)
    fprintf(ap, "{%d $%2.2x}", m, c);
  else
    fprintf(ap, "{$%2.2x}", c);
}

// **************
// PrintAsciiLine
// **************

void PrintAsciiLine(void)
{
  unsigned int i;
  int c, m;

  fprintf(ap, "%5d ", Numb);
  for (i = 0; i < strlen((char*)Det); ++i) {
    c = Det[i];
    if (c < 0x20 || c > 'z' || c == 0x5c) {
      m = 1;
      while (Det[i] == Det[i + 1]) {
        ++m;
        ++i;
      }
      PutSeq(m, c);
    }
    else
      fputc(c, ap);
  }
  fputc('\n', ap);
}

// ************
// PrintLatexLC
// ************

void PrintLatexLC(void)
{
  int c, i;

  fprintf(tp, "%d ", Numb);
  for (i = 0; i < strlen((char*)Det); ++i) {
    c = Det[i];
    if (c == 0xff)
      fputc('~', tp);   // pi
    else if (c == 0x60) // horizontal bar
    {
      fputc(0xc4, tp);
      fputc(0x80, tp);
    }
    else if (c >= 0x7b && c < 0x80) // mapped to c49b -> c49f
    {                               // pi, triangle
      fputc(0xc4, tp);
      fputc(0x20 + c, tp);
    }
    else if (c < 0x20) // mapped to 6a0 -> c5bf
    {                  // reverse graphics set 1
      fputc(0xc5, tp);
      fputc(0xa0 + c, tp);
    }
    else if (c >= 0x80 && c < 0xa0) // mapped to c6a0 -> c6bf
    {
      fputc(0xc6, tp);
      fputc(0x20 + c, tp);
    }
    else if (c >= 0xa0 && c < 0xc0) // mapped to c4a0 -> c4bf
    {
      fputc(0xc4, tp);
      fputc(c, tp);
    }
    else if (c >= 0xc0 && c < 0xe0) // mapped to c4a0 -> c4bf
    {
      fputc(0xc4, tp);
      fputc(c - 0x40, tp);
    }
    else
      fputc(c, tp);
  }
  fputc('\n', tp);
}

// ************
// PrintLatexUC
// ************

void PrintLatexUC(void)
{
  int c, i;

  fprintf(tp, "%d ", Numb);
  for (i = 0; i < strlen((char*)Det); ++i) {
    c = Det[i];
    if (c == 0xff)
      fputc('~', tp);   // checker board
    else if (c == 0x60) // horizontal bar
    {
      fputc(0xc4, tp);
      fputc(0x80, tp);
    }
    else if (c >= 0x7b && c < 0x80) // mapped to c49b -> c49f
    {
      fputc(0xc4, tp);
      fputc(0x20 + c, tp);
    }
    else if (c < 0x20) // mapped to c5a0 -> c5bf
    {                  // A-Z reverse
      fputc(0xc5, tp);
      fputc(0xa0 + c, tp);
    }
    else if (c >= 0x80 && c < 0xa0) // mapped to c6a0 -> c6bf
    {
      fputc(0xc6, tp);
      fputc(0x20 + c, tp);
    }
    else if (c >= 0xa0 && c < 0xc0) // mapped to c4a0 -> c4bf
    {
      fputc(0xc4, tp);
      fputc(c, tp);
    }
    else if (c >= 0xc0 && c < 0xe0) // mapped to c4a0 -> c4bf
    {
      fputc(0xc4, tp);
      fputc(c - 0x40, tp);
    }
    else if (c >= 'a' && c <= 'z') // mapped to A -> Z
    {
      fputc(c - 0x20, tp);
    }
    else if (c >= 'A' && c <= 'Z') // mapped to c481 -> c49a
    {
      fputc(0xc4, tp);
      fputc(c + 0x40, tp);
    }
    else
      fputc(c, tp);
  }
  fputc('\n', tp);
}

// *******
// Convert
// *******

void Convert(void)
{
  int i;
  fread(&LoadAddress, sizeof(LoadAddress), 1, bp);
  printf("load address = $%4.4x  ", LoadAddress);
  for (i = 0; i < BVERSIONS; ++i)
    if (LoadAddress == BasicVersion[i].Token)
      printf("%s", BasicVersion[i].Name);
  printf("\n");

  fprintf(tp, "\\begin{tcolorbox}[colback=black,coltext=white]\n");
  fprintf(tp, "\\verbatimfont{\\codefont}\n");
  fprintf(tp, "\\begin{verbatim}\n");
  while (ReadBasicLine()) {
    PrintAsciiLine();
    if (UpperCase)
      PrintLatexUC();
    else
      PrintLatexLC();
  }
  fprintf(tp, "\\end{verbatim}\n");
  fprintf(tp, "\\end{tcolorbox}\n");
}

// ****
// main
// ****

int main(int argc, char* argv[])
{
  int l;

  if (argc == 3 && !strcmp(argv[1], "-u"))
    UpperCase = 1;
  if (argc < 2) {
    printf("\nUsage: prg2tex [-u] prog\n");
    printf("reads <prog.prg> and writes <prog.tex> and <prog.txt>\n");
    printf("the option '-u' selects upper case/graphics mode\n");
    exit(1);
  }

  // input file must have the extension ".prg"
  // add this extension if it is not specified

  strncpy(B_file, argv[argc - 1], sizeof(B_file));
  l = strlen(B_file);
  if (l < 5 || strcmp(B_file + l - 4, ".prg"))
    strcat(B_file, ".prg");
  l = strlen(B_file);

  // ASCII file gets the extension ".txt"

  strncpy(A_file, B_file, l - 4);
  strcat(A_file, ".txt");

  // LaTeX file gets the extension ".tex"

  strncpy(T_file, B_file, l - 4);
  strcat(T_file, ".tex");

  bp = fopen(B_file, "rb");
  if (!bp) {
    printf("\nCould not open input file <%s>\n", B_file);
    exit(1);
  }

  ap = fopen(A_file, "w");
  if (!ap) {
    printf("\nCould not open output file <%s>\n", A_file);
    exit(1);
  }

  tp = fopen(T_file, "wb");
  if (!tp) {
    printf("\nCould not open output file <%s>\n", T_file);
    exit(1);
  }

  Convert();

  fclose(ap);
  fclose(bp);
  fclose(tp);

  return 0;
}
