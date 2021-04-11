/*
  Programme for generating simple screen map diagrams for the MEGA65 documentation.

  It is designed to produce maps showing screen addresses in various modes, as well
  as the memory layout corresponding to various actual displays.

  It's basically an ad-hoc tool being developed to save me having to spend hours
  making SVG diagrams with the same content.
*/

#include <stdio.h>
#include <hpdf.h>

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
  
}
