#include <stdio.h>

#include "jklib.h"


void ps_pageheader(FILE *f, const char *text, long page_no, double page_width, double page_height, double left_margin)
{
  char buf[250], ps_str[520];

  fprintf(f, "gsave /Helvetica-Bold findfont 12 scalefont setfont\n");
  fprintf(f, "%f %f moveto %s show\n", left_margin, page_height - 12, ps_string(text, ps_str));
  if (page_no >= 0)
  {
    sprintf(buf, "%ld", page_no);
    fprintf(f, "%s dup stringwidth pop %f exch sub %f moveto show\n", ps_string(buf, ps_str), left_margin + page_width, page_height - 12.0);
  }

  fprintf(f, "0 setlinecap 2 setlinewidth newpath %f %f moveto %f %f lineto stroke grestore\n",
          left_margin, page_height - 17, left_margin + page_width, page_height - 17);
}

