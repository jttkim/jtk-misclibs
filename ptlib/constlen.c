#include "ptlibin.h"


static void set_constlength(PHYLTREE_NODE *node, double length)
{
  long i;

  node->length = length;
  for (i = 0; i < node->num_descendants; i++)
    set_constlength(node->descendant[i], length);
}


void phyl_set_constlength(PHYLTREE *ptree, double length)
{
  set_constlength(ptree->root, length);
}

