/* $Id */
/*
 * $Log: multhick.c,v $
 * Revision 1.1  2000/01/20 01:27:33  kim
 * Added phyl_multiply_thickness() and phyl_max_thickness()
 *
 */

#include "ptlibin.h"


/*
 * Multiply edge thickness values with a constant
 */

static void multiply_thick(PHYLTREE_NODE *node, double factor)
{
  long i;

  node->thick *= factor;
  for (i = 0; i < node->num_descendants; i++)
    multiply_thick(node->descendant[i], factor);
}

void phyl_multiply_thick(PHYLTREE *ptree, double factor)
{
  multiply_thick(ptree->root, factor);
}

