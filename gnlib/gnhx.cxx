#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"
#include "gnlibin.h"


int main(int argc, char *argv[])
{
  const char *treefile, *basename = "gnhx";
  char fname[FILENAME_MAX], buf[256];
  GN_TREE gn_tree;
  GN_NODELIST_ELEMENT *gl;
  ifstream s;
  FILE *f;
  int i;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <treefile> [<outfile base name>]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  treefile = argv[1];
  if (argc > 2)
    basename = argv[2];
  {
    ifstream s(treefile);

    s >> gn_tree;
  }
  i = 0;
  for (gl = gn_tree.alive_list; gl; gl = gl->next)
  {
    sprintf(fname, "%s_%04d.gls", basename, i);
    i++;
    f = fopen(fname, "w");
    if (f == NULL)
    {
      fprintf(stderr, "error opening \"%s\" for writing\n", fname);
      continue;
    }
    gn_sprint_nodename(buf, gl->gnode);
    fprintf(f, "# genealogy list of individual \"%s\" from file %s\n", buf, treefile);
    gn_fprint_history(&gn_tree, f, &(gl->gnode->node_id));
    fclose(f);
  }
  return (EXIT_SUCCESS);
}

