#include <float.h>

#include "ptlibin.h"


double phyl_min_nodelevel(const PHYLTREE_NODE *node, double ancestor_level)
{
  double level, min_l, l;
  long i;

  level = ancestor_level + node->length;
  min_l = level;
  for (i = 0; i < node->num_descendants; i++)
  {
    l = phyl_min_nodelevel(node->descendant[i], level);
    if (l < min_l)
      min_l = l;
  }
  return (min_l);
}


double phyl_max_nodelevel(const PHYLTREE_NODE *node, double ancestor_level)
{
  double level, max_l, l;
  long i;

  level = ancestor_level + node->length;
  max_l = level;
  for (i = 0; i < node->num_descendants; i++)
  {
    l = phyl_max_nodelevel(node->descendant[i], level);
    if (l > max_l)
      max_l = l;
  }
  return (max_l);
}


/*
double phyl_negtreeheight(const PHYLTREE_NODE *node)
{
  double height = 0.0, h;
  long i;

  for (i = 0; i < node->num_descendants; i++)
  {
    h = phyl_negtreeheight(node->descendant[i]);
    if (h < height)
      height = h;
  }
  return (height + node->length);
}


double phyl_postreeheight(const PHYLTREE_NODE *node)
{
  double height = 0.0, h;
  long i;

  for (i = 0; i < node->num_descendants; i++)
  {
    h = phyl_treeheight(node->descendant[i]);
    if (h > height)
      height = h;
  }
  return (height + node->length);
}
*/


double phyl_treeheight(const PHYLTREE_NODE *node)
{
  return (phyl_max_nodelevel(node, 0.0) - phyl_min_nodelevel(node, 0.0));
}

