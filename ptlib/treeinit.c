#include <stdio.h>

#include "ptlibin.h"


void phyl_init_tree(PHYLTREE *ptree)
{
  ptree->root = NULL;
  ptree->num_leaves = 0;
  ptree->leaf = NULL;
  ptree->lengthinfo_complete = 0;
}

