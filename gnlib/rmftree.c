#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Remove a node from a phylogenetic tree. Three steps are necessary:
 * 1. Unlink node from its ancestor.
 * 2. Unlink node from its offspring nodes.
 * 3. Remove node from list of root nodes if it is in that list.
 * Unlinked offspring nodes don't have an ancestor any longer, they
 * are added to the root_list of the genealogy tree lest they
 * and their descendants become forgotten.
 * Return: 0 upon success, negative error code otherwise.
 */

int gn_remove_treenode(GN_TREE *gtree, GN_NODE *gn)
{
  GN_NODE *p_ancestor;
  long   i;
  int return_code;

  /* Unlink node from its ancestor */

  p_ancestor = gn->ancestor;
  if (p_ancestor)
  {
    for (i = 0; i < p_ancestor->offspring_arrsize; i++)
    {
      if (p_ancestor->offspring[i] == gn)
      {
	p_ancestor->num_offspring--;
        p_ancestor->offspring[i] = NULL;
        break;
      }
    }
  }

  /* Unlink node from its offsprings */

  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
    {
      return_code = gn_addtonlist(&(gtree->root_list), gn->offspring[i]);
      if (return_code < 0)
        return (return_code);
      gn->offspring[i]->ancestor = NULL;
    }
  }

  /* Remove node from list of root nodes. If node is no root node, this
   * call has no effect */

  gn_rmfromnlist(&(gtree->root_list), gn);
  if (gn->offspring_arrsize)
    free(gn->offspring);
  gn_free_node(gn);
  gtree->num_nodes--;
  return (0);
}

