#include <stdlib.h>

#include "gnlibin.h"


/*
 * Trim off a branch that does not have any living
 * descendants anymore.
 * Return: 0 upon success, negative error code otherwise.
 */

int gn_trim_branch(GN_TREE *gtree, GN_NODE *gn)
{
  int ret_code;
  GN_NODE *gn_offspring;
  GN_NODELIST_ELEMENT *gl;
  long i;

  if (gtree->pruning_strategy == GN_PRUNE_NEVER)
    return (0);
  while ((gn) && (gn->num_alive == 0))
  {
    gn_offspring = gn;
    gn = gn->ancestor;
    /* printf("trimming off node %05ld-%04ld\n", gn->node_id.generation, gn->node_id.id); */
    if ((ret_code = gn_remove_treenode(gtree, gn_offspring)) < 0)
      return (ret_code);
  }
  if (gtree->pruning_strategy == GN_PRUNE_DEAD)
    return (0);
  /* do this only if gtree->pruning_strategy == GN_PRUNE_MONOLINKS */
  if ((gn) && (gn->lifespan != GN_STILL_ALIVE) && (gn->num_offspring == 1))
  {
    /* search the link to the only remaining offspring */
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if ((gn_offspring = gn->offspring[i]) != NULL)
        break;
    }
    if (gn->ancestor)
    {
      /* search link from monolink node's ancestor to monolink */
      for (i = 0; i < gn->ancestor->offspring_arrsize; i++)
      {
        if (gn->ancestor->offspring[i] == gn)
          break;
      }
      if (i == gn->ancestor->offspring_arrsize)
        return (GNERR_CORRUPTTREE);
      /* link monolink's ancestor to monolink's offspring */
      gn->ancestor->offspring[i] = gn_offspring;
    }
    else
    {
      /* if monolink is a root node, search its entry in root list */
      gl = gtree->root_list;
      while (gl)
      {
        if (gl->gnode == gn)
          break;
        gl = gl->next;
      }
      if (gl == NULL)
        return (GNERR_CORRUPTTREE);
      /* link monolink's rootnode entry to monolink's offspring */
      gl->gnode = gn_offspring;
    }
    /* link monolink's offspring to monolink's ancestor (ok if ancestor NULL) */
    gn_offspring->ancestor = gn->ancestor;
    gn_offspring->branch_generation = gn->branch_generation;
    /* remove monolink from tree */
    gtree->num_nodes--;
    if (gn->offspring_arrsize)
      free(gn->offspring);
    gn_free_node(gn);
  }
  return (0);
}

