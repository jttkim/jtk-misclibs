#include "ptlibin.h"


static void set_edges_subtree(PHYLTREE_NODE *pnode, long edge_info, long edge_counter)
{
  long i;

  if (edge_info >= 0)
    pnode->edge_info = edge_info;
  if (edge_counter >= 0)
    pnode->edge_counter = edge_counter;
  for (i = 0; i < pnode->num_descendants; i++)
    set_edges_subtree(pnode->descendant[i], edge_info, edge_counter);
}


void phyl_set_edges(PHYLTREE *ptree, long edge_info, long edge_counter)
{
  set_edges_subtree(ptree->root, edge_info, edge_counter);
}

