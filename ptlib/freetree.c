#include <stdio.h>
#include <stdlib.h>

#include "ptlibin.h"


static void phyl_free_treenode(PHYLTREE_NODE *node)
{
  long i;

  if (node)
  {
    for (i = 0; i < node->num_descendants; i++)
      phyl_free_treenode(node->descendant[i]);
    if (node->descendant)
      free(node->descendant);
    free(node);
  }
}


void phyl_free_tree(PHYLTREE *tree)
{
  phyl_free_treenode(tree->root);
  tree->root = NULL;
  free(tree->leaf);
  tree->leaf = NULL;
  tree->num_leaves = 0;
}

