#include <limits.h>

#include "gnlibin.h"


/*
 * Determine height of a tree, i.e. the difference between
 * the generation (or time step) of the deepest branching
 * and the current generation which is passed as the second
 * argument.
 * Written for analysis in TRAGIC, Bayreuth, June 1997
 */

long gn_tree_height(const GN_TREE *gtree, long generation)
{
  GN_NODELIST_ELEMENT *gl;
  GN_NODE *gn;
  long i, lowlevel;

  /* fprintf(stderr, "gn_tree_height: starting\n"); */
  lowlevel = LONG_MAX;
  gl = gtree->root_list;
  for (gl = gtree->root_list; gl; gl = gl->next)
  {
    gn = gl->gnode;
    while ((gn->num_offspring == 1) && (gn->lifespan != GN_STILL_ALIVE))
    {
      for (i = 0; i < gn->offspring_arrsize; i++)
      {
        if (gn->offspring[i])
          break;
      }
      if (i == gn->offspring_arrsize)
        return (GNERR_CORRUPTTREE);
      gn = gn->offspring[i];
    }
    if (gn->num_offspring == 0)
      continue;
    /*
    fprintf(stderr, "*** found node %05ld-%04ld with %ld offspring\n",
            gn->node_id.generation, gn->node_id.id, gn->num_offspring);
    */
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if (gn->offspring[i])
      {
        if (lowlevel > gn->offspring[i]->branch_generation)
          lowlevel = gn->offspring[i]->branch_generation;
      }
    }
  }
  if (lowlevel == LONG_MAX)
    return (0);
  if (lowlevel > generation)
    return (GNERR_BADGENERATION);
  return (generation - lowlevel);
}


static long subtree_length(const GN_NODE *gn, long generation)
{
  long length, i, l_high;

  length = 0;
  l_high = gn->branch_generation;
  if (gn->num_offspring > 0)
  {
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if (gn->offspring[i])
      {
        if (l_high < gn->offspring[i]->branch_generation)
          l_high = gn->offspring[i]->branch_generation;
        length += subtree_length(gn->offspring[i], generation);
      }
    }
    /* fprintf(stderr, "subtree_length: (%05ld-%04ld)\n", gn->node_id.generation, gn->node_id.id); */
    /* fprintf(stderr, "subtree_length: adding (b) %ld, (bg = %ld, high = %ld)\n", l_high - gn->branch_generation, gn->branch_generation, l_high); */
    length += l_high - gn->branch_generation;
  }
  if (gn->lifespan == GN_STILL_ALIVE)
  {
    /* fprintf(stderr, "subtree_length: (%05ld-%04ld)\n", gn->node_id.generation, gn->node_id.id); */
    /* fprintf(stderr, "subtree_length: adding (l) %ld (generation = %ld, l_high = %ld)\n", generation - l_high, generation, l_high); */
    length += generation - l_high;
  }
  return (length);
}


/*
 * Determine the length of a tree, i.e. the sum of all branch
 * lengths. Edges connecting the root with the oldest ancestor
 * from which different lineages branch off are not counted.
 */

long gn_tree_length(const GN_TREE *gtree, long generation)
{
  GN_NODELIST_ELEMENT *gl;
  GN_NODE *gn;
  long length, l_low, i;

  length = 0;
  gl = gtree->root_list;
  for (gl = gtree->root_list; gl; gl = gl->next)
  {
    gn = gl->gnode;
    while ((gn->num_offspring == 1) && (gn->lifespan != GN_STILL_ALIVE))
    {
      for (i = 0; i < gn->offspring_arrsize; i++)
      {
        if (gn->offspring[i])
          break;
      }
      if (i == gn->offspring_arrsize)
        return (GNERR_CORRUPTTREE);
      gn = gn->offspring[i];
    }
    if (gn->num_offspring == 0)
      continue;
    l_low = generation;
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if (gn->offspring[i] && (l_low > gn->offspring[i]->branch_generation))
	l_low = gn->offspring[i]->branch_generation;
    }
    /* fprintf(stderr, "gn_tree_length: gn->branch_generatio = %ld, l_low = %ld\n", gn->branch_generation, l_low); */
    length += subtree_length(gn, generation) + gn->branch_generation - l_low;
  }
  return (length);
}

