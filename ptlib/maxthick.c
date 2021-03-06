#include <math.h>

#include "ptlibin.h"

static double max_thickness(const PHYLTREE_NODE *node)
{
  long i;
  double m = node->thick, l;

  for (i = 0; i < node->num_descendants; i++)
  {
    if ((l = max_thickness(node->descendant[i])) > m)
      m = l;
  }
  return m;
}


double phyl_max_thickness(const PHYLTREE *ptree)
{
  return (max_thickness(ptree->root));
}


static double max_abs_thickness(const PHYLTREE_NODE *node)
{
  long i;
  double m = fabs(node->thick), l;

  for (i = 0; i < node->num_descendants; i++)
  {
    if ((l = max_abs_thickness(node->descendant[i])) > m)
      m = l;
  }
  return m;
}


double phyl_max_abs_thickness(const PHYLTREE *ptree)
{
  return (max_abs_thickness(ptree->root));
}
