#include <stdio.h>

#include "ptlibin.h"


void print_descendants(PHYLTREE_NODE *node)
{
  int i;

  if (node->num_descendants)
  {
    for (i = 0; i < node->num_descendants; i++)
      print_descendants(node->descendant[i]);
  }
  else
    printf("%s (#%ld)\n", node->name, node->leaf_index);

}


void print_subtree(PHYLTREE_NODE *node)
{
  int i;

  if (node->num_descendants)
  {
    printf("HTU is predecessor of\n");
    if (node->leaf_index >= 0)
      printf("***** HTU \"%s\" has non-negative leaf index *****\n", node->name);
    print_descendants(node);
    printf("\n");
    for (i = 0; i < node->num_descendants; i++)
      print_subtree(node->descendant[i]);
  }
  else if (node->leaf_index < 0)
    printf("***** OTU \"%s\" has negative leaf index *****\n", node->name);
}


void print_tree(PHYLTREE *tree)
{
  long i;

  printf("tree has %ld leaves:\n", tree->num_leaves);
  for (i = 0; i < tree->num_leaves; i++)
    printf("  %5ld: %s\n", i, tree->leaf[i]->name);
  print_subtree(tree->root);
}


void print_set(PHYL_LEAFSET *set, PHYLTREE_NODE **leaf)
{
  long i;
  char c;

  if (leaf == NULL)
  {
    for (i = 0; i < set->size; i++)
      printf("%d", set->flag[i]);
  }
  else
  {
    c = '{';
    for (i = 0; i < set->size; i++)
    {
      if (set->flag[i])
      {
        printf("%c %s", c, leaf[i]->name);
        c = ',';
      }
    }
    if (c == '{')
      printf("{");
    printf(" }");
  }
  printf("\n");
}

