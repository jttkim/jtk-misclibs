#include <stdlib.h>
#include <string.h>

#include "ptlibin.h"


static void get_leafset(const PHYLTREE_NODE *node, char *flag)
{
  long i;

  if (node->leaf_index >= 0)
    flag[node->leaf_index] = 1;
  else
  {
    for (i = 0; i < node->num_descendants; i++)
      get_leafset(node->descendant[i], flag);
  }
}


void phyl_get_leafsets(const PHYLTREE_NODE *node, long *num_edges, PHYL_LEAFSET *set)
{
  long i;

  get_leafset(node, set[*num_edges].flag);
  if (set[*num_edges].flag[0])
  {
    /* fprintf(stderr, "phyl_get_leafsets: inverting\n"); */
    for (i = 0; i < set[*num_edges].size; i++)
      set[*num_edges].flag[i] = !set[*num_edges].flag[i];
  }
/*
  fprintf(stderr, "leafset, (size=%lu): ", (unsigned long) set[*num_edges].size);
  for (i = 0; i < set[*num_edges].size; i++)
  {
    if (set[*num_edges].flag[i])
      fprintf(stderr, "1");
    else
      fprintf(stderr, "0");
  }
  fprintf(stderr, "\n");
*/
  set[*num_edges].node = (PHYLTREE_NODE *) node;
  /* print_set(set + *num_edges, NULL); */
  (*num_edges)++;
  for (i = 0; i < node->num_descendants; i++)
    phyl_get_leafsets(node->descendant[i], num_edges, set);
}


int phyl_cmp_leafset(const void *s1, const void *s2)
{
  PHYL_LEAFSET *set1 = (PHYL_LEAFSET *) s1, *set2 = (PHYL_LEAFSET *) s2;
  return (memcmp(set1->flag, set2->flag, set1->size));
}


int phyl_alloc_set(const PHYLTREE *ptree, PHYL_LEAFSET *set)
{
  if ((set->flag = (char *) malloc(ptree->num_leaves * sizeof(char))) == NULL)
  {
    set->size = 0;
    set->node = NULL;
    return (PHYLERR_MEM);
  }
  memset(set->flag, 0, ptree->num_leaves);
  set->size = ptree->num_leaves;
  return (0);
}


void phyl_free_set(PHYL_LEAFSET *set)
{
  if (set->flag)
    free(set->flag);
  set->flag = NULL;
  set->size = 0;
  set->node = NULL;
}


int phyl_strtoset(const PHYLTREE *ptree, char **name, PHYL_LEAFSET *set)
{
  long i, j;

  memset(set->flag, 0, set->size);
  i = 0;
  while (name[i])
  {
    for (j = 0; j < ptree->num_leaves; j++)
    {
      if (!strcmp(name[i], ptree->leaf[j]->name))
      {
        set->flag[j] = 1;
        break;
      }
    }
    if (j == ptree->num_leaves)
      return (PHYLERR_NOTINTREE);
    i++;
  }
  return (0);
}

