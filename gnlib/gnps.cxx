#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"
#include "gnlibin.h"


int main(int argc, char *argv[])
{
  const char *treefile, *fname = "gntree.ps", *label = NULL;
  GN_TREE gn_tree;
  ifstream s;
  FILE *f;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <treefile> [<label>] [<outfile name>]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  treefile = argv[1];
  if (argc > 2)
    label = argv[2];
  if (argc > 3)
    fname = argv[3];
  {
    ifstream s(treefile);

    s >> gn_tree;
  }
  f = fopen(fname, "w");
  if (f == NULL)
  {
    fprintf(stderr, "error opening \"%s\" for writing\n", fname);
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(f, "%%%%Creator: gnps\n");
  fprintf(f, "%%%%Title: %s\n", fname);
  fprintf(f, "%%%%BoundingBox: 50 50 500 700\n");
  fprintf(f, "%%%%Pages 1\n");
  fprintf(f, "%%%%EndComments\n");
  fprintf(f, "%%%%Page 1 1\n");
  fprintf(f, "save\n");
  fprintf(f, "50 50 translate\n");
  fprintf(f, "/Courier-Bold findfont 12 scalefont setfont\n");
  gn_fprint_postscript(&gn_tree, f, gn_tree.last_id.generation + 1, 500.0, 700.0, 12.0, label);
  fprintf(f, "restore\n");
  fprintf(f, "showpage\n");
  fprintf(f, "%%%%EOF\n");
  fclose(f);
  return (EXIT_SUCCESS);
}

