#include <stdio.h>

#include "gnlibin.h"


int gn_print_subtree(const GN_NODE *gn)
{
  long i;

  gn_print_node(gn);
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
      gn_print_subtree(gn->offspring[i]);
  }
  return (0);
}


/*
 * Print a dump of all nodes in a genealogy tree to stdout.
 * Use for debugging and testing purposes.
 */

int gn_print_tree(const GN_TREE *gtree)
{
  GN_NODELIST_ELEMENT *gl;
  long n;

  n = 0;
  printf("tree has %ld nodes, last node has ID: ", gtree->num_nodes);
  gn_save_id(&(gtree->last_id), stdout);
  printf("%ld nodes are living\n", gtree->num_alive);
  gl = gtree->root_list;
  while (gl)
  {
    gn_print_subtree(gl->gnode);
    gl = gl->next;
  }
  printf("\n");
  return (0);
}

