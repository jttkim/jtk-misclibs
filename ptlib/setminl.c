#include "ptlibin.h"


static void set_minlength(PHYLTREE_NODE *node, double min_length)
{
  long i;

  if (node->length < min_length)
    node->length = min_length;
  for (i = 0; i < node->num_descendants; i++)
    set_minlength(node->descendant[i], min_length);
}


void phyl_set_minlength(PHYLTREE *ptree, double min_length)
{
  set_minlength(ptree->root, min_length);
}

