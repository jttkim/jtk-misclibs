#include <stdlib.h>
#include <string.h>

#include "ptlibin.h"


static int cmp_node_lexical(const void *n1, const void *n2)
{
  const char *name1 = (*((PHYLTREE_NODE **) n1))->name;
  const char *name2 = (*((PHYLTREE_NODE **) n2))->name;

  return (strncmp(name1, name2, PHYL_MAX_NODENAMELENGTH));
}


void phyl_sort_leaves(PHYLTREE *tree)
{
  long i;

  qsort(tree->leaf, tree->num_leaves, sizeof(PHYLTREE_NODE *), &cmp_node_lexical);
  for (i = 0; i < tree->num_leaves; i++)
    tree->leaf[i]->leaf_index = i;
}

