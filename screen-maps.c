/*
  Programme for generating simple screen map diagrams for the MEGA65 documentation.

  It is designed to produce maps showing screen addresses in various modes, as well
  as the memory layout corresponding to various actual displays.

  It's basically an ad-hoc tool being developed to save me having to spend hours
  making SVG diagrams with the same content.
*/

#include <stdio.h>
#include <hpdf.h>

int logic_columns=40;
int columns=40;
int rows=25;
int screen_address=0x400;
int show_addrs=1;
int show_chars=0;
int sixteenbit_mode=0;

HPDF_Doc pdf;

void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%d\n",
      (unsigned int) error_no, (int) detail_no);
    exit(-1);
}

int main(int argc,char **argv)
{
  pdf = HPDF_New (error_handler, NULL);
  if (!pdf) {
    printf ("ERROR: cannot create pdf object.\n");
    return 1;
  }
    
  /* set compression mode */
  HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);

  HPDF_Page page_1;
  
  page_1 = HPDF_AddPage (pdf);

  HPDF_Page_SetWidth (page_1, 660);
  HPDF_Page_SetHeight (page_1, 420);



  for(int x=0;x<=columns;x++) {  
      HPDF_Page_MoveTo (page_1,10+(x*640/columns),10+0);
      HPDF_Page_LineTo (page_1,10+(x*640/columns),10+400);
      HPDF_Page_Stroke (page_1);
    }

  for(int y=0;y<=rows;y++) {
    HPDF_Page_MoveTo (page_1,10+0,10+(y*400/rows));
    HPDF_Page_LineTo (page_1,10+640,10+(y*400/rows));
    HPDF_Page_Stroke (page_1);
  }

  //  const char *font_name = HPDF_LoadTTFontFromFile (pdf, "./fonts/mega40-Regular.ttf", HPDF_TRUE);
  HPDF_Font font = HPDF_GetFont (pdf, "Helvetica", "CP1250");
  
  HPDF_Page_SetFontAndSize(page_1,font,640/columns/2);

  HPDF_Page_BeginText(page_1);
  
  for(int y=0;y<rows;y++) {
    for(int x=0;x<columns;x++) {
      HPDF_Page_TextOut(page_1,10+1+(x*640/columns),400-(y*400/rows),"1");
      
    }
  }

  HPDF_Page_EndText(page_1);
  
  HPDF_SaveToFile (pdf, "test.pdf");
  
}
