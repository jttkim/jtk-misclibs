#include "ptlibin.h"


/*
 * Sed edge thickness according to edge counter
 */

static void counter2thick(PHYLTREE_NODE *node, double thick)
{
  long i;

  node->thick = node->edge_counter * thick;
  for (i = 0; i < node->num_descendants; i++)
    counter2thick(node->descendant[i], thick);
}


void phyl_counter2thick(PHYLTREE *ptree, double thick)
{
  counter2thick(ptree->root, thick);
}

