#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


void gn_free_node(GN_NODE *gn)
{
  if (gn->genome)
    free(gn->genome);
  free(gn);
}


void gn_free_subtree(GN_NODE *gn)
{
  long i;

  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
      gn_free_subtree(gn->offspring[i]);
  }
  if (gn->offspring_arrsize)
    free(gn->offspring);
  gn_free_node(gn);
}


/*
 * Free an entire genealogy tree.
 */

int gn_free_tree(GN_TREE *gtree)
{
  GN_NODELIST_ELEMENT *gl;

  gl = gtree->root_list;
  while (gl)
  {
    gn_free_subtree(gl->gnode);
    gl = gl->next;
  }
  gn_free_nodelist(&gtree->alive_list);
  gn_free_nodelist(&gtree->root_list);
  gtree->num_nodes = 0;
  gtree->last_id.generation = LONG_MIN;
  gtree->last_id.id = 0;
  gtree->num_alive = 0;
  return (0);
}

