#include "ptlibin.h"

void phyl_vshape_lengths(PHYLTREE_NODE *node, long ancestor_level)
{
  long i, level;

  level = phyl_num_leaves(node);
  node->length = ancestor_level - level;
  for (i = 0; i < node->num_descendants; i++)
    phyl_vshape_lengths(node->descendant[i], level);
}

