#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAMES 256
#define MAXLEN 80
#define NL 16

char *Type[] =
{
   "byte",
   "word",
   "quad",
   "near",
   "far",
   "low",
   "high",
   "array"
};

#define TYPES (sizeof(Type) / sizeof(char *))

struct memstruct
{
   int  addr    ;
   char name[NL];
   char type[NL];
   char grup[NL];
   char comm[80];
} All[NAMES];

char *ifn = "/Users/home/MEGA65/C65/b65.src";
char *ofn = "appendix-rommap.tex";

char *LaTeX = "* ### LaTeX ### *\n";

char Line[256];

int lc;
int Names;
int Items;
int Total;
int Lines;
int prefix;

FILE *rp;
FILE *wp;

int CmpName(const void *arg1, const void *arg2 )
{
   struct memstruct *n1;
   struct memstruct *n2;

   n1 = (struct memstruct *) arg1;
   n2 = (struct memstruct *) arg2;

   return (strcmp(n1->name,n2->name));
}

int CmpAddr(const void *arg1, const void *arg2 )
{
   struct memstruct *n1;
   struct memstruct *n2;

   n1 = (struct memstruct *) arg1;
   n2 = (struct memstruct *) arg2;

   if (n1->addr < n2->addr) return -1;
   if (n1->addr > n2->addr) return  1;
   return 0;
}

//   int  addr    ;
//   char name[16];
//   char type[16];
//   char grup[16];
//   char comm[64];

void AddItem(void)
{
   char *tr;
   char *eq;
   char *bl;
   char *dl;
   char *tt;
   char *tc;
   char *gr;
   char *co;
   char  ty;
   char  c ;
   int   nl;
   int   in;
   int   i,j;


   if (Names >= NAMES)
   {
      fprintf(stderr,"\n*** name table overflow ***\n");
      exit(1);
   }

   tr = strstr(Line,";&&"); // trigger
   if (tr)
   {
      All[Names].addr = -1;
      strncpy(All[Names].comm,tr+3,strlen(tr)-4);
      ++Names;
      return;
   }

   tr = strstr(Line,";& "); // trigger
   eq = strchr(Line,'=');   // assignment
   bl = strchr(Line,' ');   // blanks
   dl = strchr(Line,'$');   // start of hex address
   if (tr && eq && dl)
   {
      nl = eq - Line;
      if (bl && bl < eq) nl = bl - Line;
      if (nl >= NL-1) nl = NL-1;
      for (i=0,j=0 ; i < nl ; ++i)
      {
         c = Line[i];
         if (c == '_') All[Names].name[j++] = '\\';
         All[Names].name[j++] = c;
      }
      sscanf(dl+1,"%x",&All[Names].addr);

      strcpy(All[Names].type," ");
      tt = strchr(tr+2,'&');
      if (tt)
      {
         ty = 0;
         for (tc = tr+2 ; tc < tt ; ++tc)
         {
            if (*tc != ' ')
            {
               ty = tolower(*tc);
               break;
            }
         }
         if (ty)
         for (in=0 ; in < TYPES ; ++in)
         {
            if (ty == Type[in][0])
            {
               strcpy(All[Names].type,Type[in]);
            }
            gr = strchr(tc,'&');
            if (gr)
            {
               j=0;
               ++gr;
               while (*gr == ' ') ++gr;
               while (j < NL-1 && *gr != 0 && *gr !='&' && *gr != ' ')
                  All[Names].grup[j++] = *gr++;
               co = gr;
               if (*co)
               {
                  ++co;
                  while (*co == ' ' || *co == '&') ++co;
                  j=0;
                  while (*co && j < 78 && *co >= ' ')
                  {
                     if (*co == '$')
                        All[Names].comm[j++] = '\\';
                     All[Names].comm[j++] = *co++;
                  }
               }
            }
         }
      }

      *tr = 0;
      printf("%5d [%2d] %s --> %s",lc,nl,Line,tr+1);
      ++Names;
   }
}

void PrintPage(int alpha, int omega)
{
   int i;

   fprintf(wp,"\\texttt{\n");
   fprintf(wp,"\\small\n");
   fprintf(wp,"\\setlength{\\tabcolsep}{1mm}\n");
   fprintf(wp,"\\begin{tabular}{|l|l|l|l|l|}\n");
   fprintf(wp,"\\hline\n");
   fprintf(wp,"Name & Ad & Type & Group & Comment \\\\\n");
   fprintf(wp,"\\hline\n");

   for (i=alpha ; i < Names && i < omega; ++i)
   {
      if (All[i].addr < 0)
      {
         fprintf(wp," & & & & %s \\\\\n",All[i].comm);
      }
      else
      {
         fprintf(wp,"%-16s & ",All[i].name);
         if (All[i].addr < 256) fprintf(wp,"  %02x",All[i].addr);
         else                   fprintf(wp,  "%04x",All[i].addr);
         fprintf(wp," & %-5s ",All[i].type);
         fprintf(wp," & %-5s ",All[i].grup);
         fprintf(wp," & %s",All[i].comm);
         fprintf(wp," \\\\\n");
      }
   }
   fprintf(wp,"\\hline\n");
   fprintf(wp,"\\end{tabular}\n");
   fprintf(wp,"}\n");
}

int main(int argc, char *argv[])
{
   int i,j,k,alpha,omega;
   size_t l;
   int page,col,row,last;
   char *dp;

   rp = fopen(ifn,"r");
   wp = fopen(ofn,"w");

   while (!feof(rp))
   {
      fgets(Line,sizeof(Line),rp);
      ++lc;
      if (!strcmp(Line,LaTeX)) break; // found start
   }

   if (feof(rp))
   {
      fprintf(stderr,"\n*** found no [* ### LaTeX ### *] marker ***\n");
      exit(1);
   }

   printf("start marker [* ### LaTeX ### *] found at line %d\n",lc);

   while (!feof(rp))
   {
      fgets(Line,sizeof(Line),rp);
      ++lc;
      if (!strcmp(Line,LaTeX)) break; // found stop
      AddItem();
   }

   printf("stop  marker [* ### LaTeX ### *] found at line %d\n",lc);
   fclose(rp);

   fprintf(wp,"\\newpage\n");
   fprintf(wp,"\\subsection{direct (zero) page}\n");
   PrintPage(0,48);

   for (i=48 ; i < Names; i+= 48)
   {
      alpha = All[i].addr;
      if (i+47 < Names) omega = All[i+47].addr;
      else              omega = All[Names-1].addr;
      fprintf(wp,"\\newpage\n");
      fprintf(wp,"\\subsubsection{\\$%2X - \\$%2X}\n",alpha,omega);
      PrintPage(i,i+50);
   }

   fclose(wp);

   return 0;
}


