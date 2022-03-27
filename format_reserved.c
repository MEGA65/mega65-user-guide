#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMES 256
#define PAGES 2

struct ntstruct
{
   char name[16];
   int  token;
} All[NAMES];

int Names;
int Items;
int Total;
int Lines;
int prefix;

FILE *rp;
FILE *wp;

int CmpName(const void *arg1, const void *arg2 )
{
   struct ntstruct *n1;
   struct ntstruct *n2;

   n1 = (struct ntstruct *) arg1;
   n2 = (struct ntstruct *) arg2;

   return (strcmp(n1->name,n2->name));
}

int CmpToken(const void *arg1, const void *arg2 )
{
   struct ntstruct *n1;
   struct ntstruct *n2;

   n1 = (struct ntstruct *) arg1;
   n2 = (struct ntstruct *) arg2;

   if (n1->token < n2->token) return -1;
   if (n1->token > n2->token) return  1;
   return 0;
}

int main(int argc, char *argv[])
{
   int i,j,k;
   size_t l;
   int page,col,row,last;
   char Line[80];
   char *dp;

   rp = fopen("reserved.txt","r");
   wp = fopen("reserved.tab","w");

   fgets(Line,sizeof(Line),rp);
   while (Line[0] != '-' || Line[1] != '-')
   {
      if (strlen(Line) > 16)
      {
         if (!strncmp(Line,"prefix",6)) sscanf(Line+14,"%2x",&prefix);
         else
         {
            dp = strchr(Line,'$');
            if (dp && (dp-Line) < 10)
            {
               dp[0] = '\\';
               dp[1] = '$';
            }
            dp = strchr(Line,'#');
            if (dp && (dp-Line) < 10)
            {
               dp[0] = '\\';
               dp[1] = '#';
            }
            dp = strchr(Line,'^');
            if (dp && (dp-Line) < 10)
            {
               strncpy(Line,"\\string^",8);
            }
            strncpy(All[Names].name,Line,10);
            sscanf(Line+14,"%2x",&All[Names].token);
            All[Names].token += (prefix << 8);
            Names++;
         }
      }
      fgets(Line,sizeof(Line),rp);
   }
   fclose(rp);
   Items = Names;
   if ((Names % 3) != 0) Items += 3 - (Names % 3);
   Total = Items / 3;
   Lines = Total / PAGES;
   if ((Total % PAGES) != 0) Lines++;

   printf("%3d names\n",Names);
   printf("%3d items\n",Items);
   printf("%3d total\n",Total);
   printf("%3d pages\n",PAGES);
   printf("%3d Lines per page\n",Lines);

   qsort(All,Names,sizeof(struct ntstruct),CmpName);

   for (page = 0; page < PAGES ; ++page)
   {
      fprintf(wp,"%% page %d\n",page+1);
      fprintf(wp,"\\newpage\n");
      fprintf(wp,"\\subsection{Keywords And Tokens Part %d}\n",page+1);
      fprintf(wp,"{\\ttfamily\n");
      fprintf(wp,"\\setlength{\\tabcolsep}{1mm}\n");
      fprintf(wp,"\\begin{tabular}{|p{2.2cm}r|p{2.2cm}r|p{2.2cm}r|}\n");
      fprintf(wp,"\\hline\n");

      last = Lines;
      if (page == PAGES-1) last = Total - (PAGES-1) * Lines;
      for (row = 0 ; row < last ; ++row)
      {
         i = page * Lines * 3 + row;
         j = i + last;
         k = j + last;
         fprintf(wp,"%s & %4.2X &",All[i].name,All[i].token);
         if (j < Names)
         fprintf(wp,"%s & %4.2X &",All[j].name,All[j].token);
         else
         fprintf(wp,"   &       &");
         if (k < Names)
         fprintf(wp,"%s & %4.2X \\\\ \n",All[k].name,All[k].token);
         else
         fprintf(wp,"   &       \\\\ \n");
      }
      fprintf(wp,"\\hline\n");
      fprintf(wp,"\\end{tabular}}\n");
   }

   qsort(All,Names,sizeof(struct ntstruct),CmpToken);

   for (page = 0; page < PAGES ; ++page)
   {
      fprintf(wp,"%% page %d\n",page+1);
      fprintf(wp,"\\newpage\n");
      fprintf(wp,"\\subsection{Tokens And Keywords Part %d}\n",page+1);
      fprintf(wp,"{\\ttfamily\n");
      fprintf(wp,"\\setlength{\\tabcolsep}{1mm}\n");
      fprintf(wp,"\\begin{tabular}{|rp{2.2cm}|rp{2.2cm}|rp{2.2cm}|}\n");
      fprintf(wp,"\\hline\n");

      last = Lines;
      if (page == PAGES-1) last = Total - (PAGES-1) * Lines;
      for (row = 0 ; row < last ; ++row)
      {
         i = page * Lines * 3 + row;
         j = i + last;
         k = j + last;
         fprintf(wp,"%4.2X & %s &",All[i].token,All[i].name);
         if (j < Names)
         fprintf(wp,"%4.2X & %s &",All[j].token,All[j].name);
         else
         fprintf(wp,"   &       &");
         if (k < Names)
         fprintf(wp,"%4.2X & %s \\\\ \n",All[k].token,All[k].name);
         else
         fprintf(wp,"   &       \\\\ \n");
      }
      fprintf(wp,"\\hline\n");
      fprintf(wp,"\\end{tabular}}\n");
   }
   printf("%3d Lines on last page\n",last);

   return 0;
}


