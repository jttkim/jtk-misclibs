#include <stdio.h>

#include "gnlibin.h"


/*
 * Update phylogenetic tree upon death of a node. The lifespan
 * of the node is set and the number of links of the node and
 * its ancestors is updated.
 * Return: 0 upon success, negative error code otherwise.
 */

int gn_node_death(GN_TREE *gtree, const GN_NODE_ID *gn_nid, long death_generation)
{
  GN_NODE *gn, *gn1;

  if ((gn = gn_find_node(gtree, gn_nid)) == NULL)
    return (GNERR_UNKNODE);
  if (death_generation < gn->node_id.generation)
    return (GNERR_NEGLSPAN);
  if (gn->lifespan != GN_STILL_ALIVE)
    return (GNERR_DUPLICDEATH);
  gn->lifespan = death_generation - gn->node_id.generation;
  gn_rmfromnlist(&(gtree->alive_list), gn);
  gtree->num_alive--;
  gn1 = gn;
  while (gn1)
  {
    gn1->num_alive--;
    gn1 = gn1->ancestor;
  }
  return (gn_trim_branch(gtree, gn));
}

