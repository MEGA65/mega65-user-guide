#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMES 1000
#define PAGES 5

char N[NAMES][80];
int Names;
int Lines;

FILE *rp;
FILE *wp;

int main(int argc, char *argv[])
{
   int i;
   size_t l;
   int page,col,row,last,done;

   rp = fopen("supporters.txt","r");
   wp = fopen("supporters.tab","w");

   fgets(&N[0][0],79,rp);
   Names = 1;
   while (!feof(rp))
   {
      fgets(&N[Names][0],79,rp);
      ++Names;
      if (Names >= NAMES)
      {
         printf("\n*** too many names ***\n");
         exit(1);
      }
   }
   fclose(rp);
   --Names;
   Lines = (Names+9) / (PAGES * 3) +1;

/*
   printf("\n%d Names\n",Names);
   for (i=0 ; i < Names ; ++i)
      printf("%3d %s",i,N[i]);
*/

   for (i=0 ; i < Names; ++i)
   {
      l = strlen(N[i]);
      if (l && N[i][l-1] == 10) N[i][--l] = 0;
      if (l && N[i][l-1] == 13) N[i][--l] = 0;
   }

   printf("%3d pages\n",PAGES);
   printf("%3d Lines per page\n",Lines);

   fprintf(wp,"\\begin{small}\n");
   done = 0;
   for (page = 0; page < PAGES ; ++page)
   {
      fprintf(wp,"%% page %d\n",page+1);
      if (page) fprintf(wp,"\\newpage\n");
      fprintf(wp,"\\setlength{\\tabcolsep}{1mm}\n");
      fprintf(wp,"\\begin{tabular}{p{4cm}p{4cm}p{4cm}}\n");

      last = Lines;
      if (page ==       0) last = Lines-3;
      if (page == PAGES-1)
      {
         last = (Names - done)/3;
         if (Names % 3) ++last;
      }
      printf("%d lines on page %d\n",last,page);
      for (row = 0 ; row < last ; ++row)
      {
         i = done + row;
         fprintf(wp,"%s & %s & %s \\\\ \n",
            N[i],N[i+last],N[i+last*2]);
      }
      done += last * 3;
      fprintf(wp,"\\end{tabular}\n");
   }
   fprintf(wp,"\\end{small}\n");
   fprintf(wp,"\\ifdefined\\printmanual\n\\else\n\\setstretch{1}\n\\fi\n");

   printf("%3d Lines on last page\n",last);

   return 0;
}


