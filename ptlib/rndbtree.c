#include <stdlib.h>

#ifdef __atarist__      /* declare random() and srandom() */
#  include <unistd.h>
#else
#  include <math.h>
#endif

#include "ptlibin.h"


int phyl_rndbin_tree(long num_leaves, PHYLTREE *tree, int seed)
{
  PHYLTREE_NODE **node, *new_leaf;
  long num_nodes, n, k;

  srandom(seed);
  if ((node = (PHYLTREE_NODE **) malloc((num_leaves - 1) * sizeof(PHYLTREE_NODE *))) == NULL)
    return (PHYLERR_MEM);
  if ((tree->leaf = (PHYLTREE_NODE **) malloc(num_leaves * sizeof(PHYLTREE_NODE *))) == NULL)
  {
    free(node);
    return (PHYLERR_MEM);
  }
  if ((tree->root = add_node(NULL)) == NULL)
  {
    free(node);
    free0(tree->leaf);
    return (PHYLERR_MEM);
  }
  node[0] = tree->root;
  tree->num_leaves = 0;
  tree->lengthinfo_complete = 0;
  if ((new_leaf = add_node(tree->root)) == NULL)
  {
    free(node);
    phyl_free_tree(tree);
    return (PHYLERR_MEM);
  }
  new_leaf->length = 1.0;
  sprintf(new_leaf->name, "rndleaf #%ld", tree->num_leaves);
  new_leaf->leaf_index = tree->num_leaves;
  tree->leaf[tree->num_leaves++] = new_leaf;
  if ((new_leaf = add_node(tree->root)) == NULL)
  {
    free(node);
    phyl_free_tree(tree);
    return (PHYLERR_MEM);
  }
  new_leaf->length = 1.0;
  sprintf(new_leaf->name, "rndleaf #%ld", tree->num_leaves);
  new_leaf->leaf_index = tree->num_leaves;
  tree->leaf[tree->num_leaves++] = new_leaf;
  for (num_nodes = 1; num_nodes < num_leaves - 1; num_nodes++)
  {
    if ((node[num_nodes] = add_node(NULL)) == NULL)
    {
      free(node);
      phyl_free_tree(tree);
      return (PHYLERR_MEM);
    }
    if ((node[num_nodes]->descendant = (PHYLTREE_NODE **) malloc(sizeof(PHYLTREE_NODE *))) == NULL)
    {
      free(node[num_nodes]);
      free(node);
      phyl_free_tree(tree);
      return (PHYLERR_MEM);
    }
    node[num_nodes]->length = 1.0;
    n = random() % num_nodes;
    k = random() % 2;
    node[num_nodes]->ancestor = node[n];
    node[num_nodes]->descendant[0] = node[n]->descendant[k];
    node[n]->descendant[k] = node[num_nodes];
    node[num_nodes]->num_descendants = 1;
    if ((new_leaf = add_node(node[num_nodes])) == NULL)
    {
      free(node);
      phyl_free_tree(tree);
      return (PHYLERR_MEM);
    }
    new_leaf->length = 1.0;
    sprintf(new_leaf->name, "rndleaf #%ld", tree->num_leaves);
    new_leaf->leaf_index = tree->num_leaves;
    tree->leaf[tree->num_leaves++] = new_leaf;
  }
  free(node);
  tree->lengthinfo_complete = 1;
  return (0);
}

