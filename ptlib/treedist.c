/* $Id: treedist.c,v 1.2 2000/01/20 01:26:11 kim Exp $ */
/*
 * $Log: treedist.c,v $
 * Revision 1.2  2000/01/20 01:26:11  kim
 * Created kludge: In phyl_topotreedist, edge lengths are written into
 * thickness of corresponding edges as a side effect.
 *
 */

#include <stdlib.h>
#include <string.h>

#include "ptlibin.h"


long phyl_topotreedist(PHYLTREE *tree1, PHYLTREE *tree2)
{
  PHYL_LEAFSET *set1, *set2;
  char     *set1_flags, *set2_flags;
  long i, num_edges1, num_edges2, edge1, edge2, e, d, similarity;
  int ec;
  double edge1_length, edge2_length;

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
/*
  printf("tree #1 has %ld edges\n", num_edges1);
  for (i = 0; i < num_edges1; i++)
    print_set(set1 + i, tree1->leaf);
*/
  num_edges2 = 0;
  phyl_get_leafsets(tree2->root, &num_edges2, set2);
  qsort(set2, num_edges2, sizeof(PHYL_LEAFSET), phyl_cmp_leafset);
/* 
  printf("tree #2 has %ld edges\n", num_edges2);
  for (i = 0; i < num_edges2; i++)
    print_set(set2 + i, tree2->leaf);
*/
  d = 0;
  similarity = 0;
  edge1 = 0;
  edge2 = 0;
  while ((edge1 < num_edges1) && (edge2 < num_edges2))
  {
    ec = memcmp(set1[edge1].flag, set2[edge2].flag, tree1->num_leaves);
    /* printf("edge1 = %ld, edge2 = %ld, memcmp result: %d\n", edge1, edge2, ec); */
    if (ec)
    {
      d++;
/*
      printf("differing leaf sets:\n");
      print_set(set1 + edge1, tree1->leaf);
      print_set(set2 + edge2, tree2->leaf);
*/
      set1[edge1].node->edge_info = PHYLEDGINF_DIFFEDGE;
      set2[edge2].node->edge_info = PHYLEDGINF_DIFFEDGE;
      if (ec < 0)
	edge1++;
      if (ec > 0)
	edge2++;
    }
    else
    {
      similarity++;
/*
      printf("identical leaf set:\n");
      print_set(set1 + edge1, tree1->leaf);
*/
      /*
       * Kludge: set thickness values for all matching edges in tree #1 to
       * length of first matching edge in tree2 and vice versa.
       */
      edge1_length = set1[edge1].node->length;
      edge2_length = set2[edge2].node->length;
      e = edge1;
      while ((edge1 < num_edges1) && !memcmp(set1[edge1].flag, set1[e].flag, tree1->num_leaves))
      {
	set1[edge1].node->edge_info = PHYLEDGINF_IDEDGE;
	set1[edge1].node->thick = edge2_length;
	(set1[edge1++].node->edge_counter)++;
      }
      e = edge2;
      while ((edge2 < num_edges2) && !memcmp(set2[edge2].flag, set2[e].flag, tree2->num_leaves))
      {
	set2[edge2].node->edge_info = PHYLEDGINF_IDEDGE;
	set2[edge2].node->thick = edge1_length;
	(set2[edge2++].node->edge_counter)++;
      }
    }
  }
  /* printf("\nsimilarity (# identical edges): %ld\n", similarity); */
  free(set1);
  free(set1_flags);
  free(set2);
  free(set2_flags);
  return (d);
}
