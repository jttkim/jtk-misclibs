#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


int gn_fprint_history(const GN_TREE *gtree, FILE *f, const GN_NODE_ID *id)
{
  GN_NODE *gn;
  char buf[256];

  gn = gn_find_node(gtree, id);
  if (gn == NULL)
    return (GNERR_UNKNODE);
  while (gn)
  {
    if (gn->genome)
      fprintf(f, "%ld (ID: %s): %s\n",
	      gn->branch_generation, gn_node_idstring(&(gn->node_id), buf), gn->genome);
    else
      fprintf(f, "%ld (ID: %s)\n", gn->branch_generation, gn_node_idstring(&(gn->node_id), buf));
    gn = gn->ancestor;
  }
  return (0);
}

