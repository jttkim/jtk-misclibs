#include "ptlibin.h"


void phyl_set_lengths(PHYLTREE_NODE *node, double length)
{
  long i;

  node->length = length;
  for (i = 0; i < node->num_descendants; i++)
    phyl_set_lengths(node->descendant[i], length);
}

