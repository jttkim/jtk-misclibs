#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


int gn_print_jftrees(const GN_TREE *gtree, long generation, FILE *f)
{
  GN_NODELIST_ELEMENT *gl;
  char *treestring;
  long num_rootnodes;

  num_rootnodes = 0;
  gl = gtree->root_list;
  while (gl)
  {
	 num_rootnodes++;
	 gl = gl->next;
  }
  fprintf(f, "%ld\n", num_rootnodes);
  gl = gtree->root_list;
  while (gl)
  {
	 treestring = gn_jf_treespec(gl->gnode, generation);
	 if (treestring)
	 {
		fprintf(f, "%s;\n", treestring);
		free(treestring);
	 }
	 else
	 {
		return (GNERR_MEM);
	 }
	 gl = gl->next;
  }
  return (0);
}

