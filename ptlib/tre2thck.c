#include <stdlib.h>
#include <string.h>

#include "ptlibin.h"


int phyl_tree2thick(PHYLTREE *tree1, const PHYLTREE *tree2, double max_thickness)
{
  PHYL_LEAFSET *set1, *set2;
  char     *set1_flags, *set2_flags;
  long i, num_edges1, num_edges2, edge1, edge2, d, similarity;
  int ec;
  double tf;

  tf = max_thickness / phyl_max_edgelength(tree2);
  if ((tree1->num_leaves == 0) || (tree2->num_leaves == 0))
    return (PHYLERR_NOLEAVES);
  if (tree1->num_leaves != tree2->num_leaves)
    return (PHYLERR_DIFFNUMLEAVES);
  phyl_sort_leaves(tree1);
  phyl_sort_leaves(tree2);
  for (i = 0; i < tree1->num_leaves; i++)
  {
    if (strcmp(tree1->leaf[i]->name, tree2->leaf[i]->name))
      return (PHYLERR_INCOMPATLEAVES);
  }
  if ((set1 = (PHYL_LEAFSET *) malloc(2 * tree1->num_leaves * sizeof(PHYL_LEAFSET))) == NULL)
    return (PHYLERR_MEM);
  if ((set1_flags = (char *) malloc(2 * tree1->num_leaves * tree1->num_leaves * sizeof(char))) == NULL)
  {
    free(set1);
    return (PHYLERR_MEM);
  }
  memset(set1_flags, 0, 2 * tree1->num_leaves * tree1->num_leaves);
  for (i = 0; i < 2 * tree1->num_leaves; i++)
  {
    set1[i].size = tree1->num_leaves;
    set1[i].flag = set1_flags + tree1->num_leaves * i;
  }
  if ((set2 = (PHYL_LEAFSET *) malloc(2 * tree2->num_leaves * sizeof(PHYL_LEAFSET))) == NULL)
  {
    free(set1);
    free(set1_flags);
    return (PHYLERR_MEM);
  }
  if ((set2_flags = (char *) malloc(2 * tree2->num_leaves * tree2->num_leaves * sizeof(char))) == NULL)
  {
    free(set1);
    free(set1_flags);
    free(set2);
    return (PHYLERR_MEM);
  }
  memset(set2_flags, 0, 2 * tree2->num_leaves * tree2->num_leaves);
  for (i = 0; i < 2 * tree2->num_leaves; i++)
  {
    set2[i].size = tree2->num_leaves;
    set2[i].flag = set2_flags + tree2->num_leaves * i;
  }
  num_edges1 = 0;
  phyl_get_leafsets(tree1->root, &num_edges1, set1);
  qsort(set1, num_edges1, sizeof(PHYL_LEAFSET), phyl_cmp_leafset);
  num_edges2 = 0;
  phyl_get_leafsets(tree2->root, &num_edges2, set2);
  qsort(set2, num_edges2, sizeof(PHYL_LEAFSET), phyl_cmp_leafset);
  d = 0;
  similarity = 0;
  edge1 = 0;
  edge2 = 0;
  while ((edge1 < num_edges1) && (edge2 < num_edges2))
  {
    ec = memcmp(set1[edge1].flag, set2[edge2].flag, tree1->num_leaves);
    if (ec == 0)
      set1[edge1].node->thick = set2[edge2].node->length * tf;
    if (ec <= 0)
      edge1++;
    if (ec >= 0)
      edge2++;
  }
  free(set1);
  free(set1_flags);
  free(set2);
  free(set2_flags);
  return (0);
}

