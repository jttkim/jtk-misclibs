#include "ptlibin.h"

static double max_edgelength(const PHYLTREE_NODE *node)
{
  long i;
  double m = node->length, l;

  for (i = 0; i < node->num_descendants; i++)
  {
    if ((l = max_edgelength(node->descendant[i])) > m)
      m = l;
  }
  return m;
}


double phyl_max_edgelength(const PHYLTREE *ptree)
{
  return (max_edgelength(ptree->root));
}

    
