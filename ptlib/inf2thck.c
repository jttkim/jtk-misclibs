#include "ptlibin.h"


/*
 * Make edges with a certain info value thick
 */

static void inf2thick(PHYLTREE_NODE *node, int thickinfo, double thick)
{
  long i;

  if (node->edge_info == thickinfo)
    node->thick = thick;
  for (i = 0; i < node->num_descendants; i++)
    inf2thick(node->descendant[i], thickinfo, thick);
}

void phyl_inf2thick(PHYLTREE *ptree, int thickinfo, double thick)
{
  inf2thick(ptree->root, thickinfo, thick);
}

