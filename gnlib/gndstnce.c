#include <math.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Compute the genealogical distance between two nodes at a given
 * generation.
 * Return: Positive distance value or -1 if nodes belong to
 * disjunct subtrees.
 * Note: This function really should check if both nodes are alive
 * at the generation for which the distance is requested, but it
 * doesn't do that in the current form.
 */

long gn_distance(const GN_TREE *gtree, GN_NODE *p1, GN_NODE *p2, long g)
{
  long g1, g2;

  if (p1 == p2)
    return (0);
  g1 = g;
  g2 = g;
  while ((p1->ancestor != NULL) && (p2->ancestor != NULL))
  {
    if (p1->node_id.generation > p2->node_id.generation)
    {
      g1 = p1->node_id.generation;
      p1 = p1->ancestor;
    }
    else
    {
      g2 = p2->node_id.generation;
      p2 = p2->ancestor;
    }
    if (p1 == p2)
    {
      return (g - ((g1 < g2) ? g1 : g2));
    }
  }
  return (-1);
}


double gn_ddc(const GN_TREE *gtree, long generation)
{
  long res, d, *dd;
  double d_sum, ddc, r;
  GN_NODELIST_ELEMENT *gl, *gl1;

  res = gn_tree_height(gtree, generation);
  if (res <= 0)
    return (0.0);
  res++;
  dd = (long *) malloc(res * sizeof(long));
  if (dd == NULL)
    return (0.0);
  for (d = 0; d < res; d++)
    dd[d] = 0;
  for (gl = gtree->alive_list; gl; gl = gl->next)
  {
    for (gl1 = gl->next; gl1; gl1 = gl1->next)
    {
      d = gn_distance(gtree, gl->gnode, gl1->gnode, generation);
      if (d < 0)
        continue;
      dd[d]++;
    }
  }
  d_sum = 0.0;
  for (d = 0; d < res; d++)
    d_sum += dd[d];
  ddc = 0.0;
  for (d = 0; d < res; d++)
  {
    if (dd[d])
    {
      r = ((double) dd[d]) / d_sum;
      ddc -= r * log(r);
    }
  }
  free(dd);
  return (ddc);
}

