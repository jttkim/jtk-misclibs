#include "ptlibin.h"


static void set_thickness(PHYLTREE_NODE *node, double thickness)
{
  long i;

  node->thick = thickness;
  for (i = 0; i < node->num_descendants; i++)
    set_thickness(node->descendant[i], thickness);
}


void phyl_set_thickness(PHYLTREE *ptree, double thickness)
{
  set_thickness(ptree->root, thickness);
}

