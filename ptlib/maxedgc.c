#include "ptlibin.h"


static long max_edgc(const PHYLTREE_NODE *node)
{
  long i, ec, m = 0;

  for (i = 0; i < node->num_descendants; i++)
  {
    ec = max_edgc(node->descendant[i]);
    if (m < ec)
      m = ec;
    if (m < node->descendant[i]->edge_counter)
      m = node->descendant[i]->edge_counter;
  }
  return (m);
}


long phyl_max_edgecounter(const PHYLTREE *ptree)
{
  return (max_edgc(ptree->root));
}
