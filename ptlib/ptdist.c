#include "ptlibin.h"


double phyl_nodedistance(PHYLTREE_NODE *node1, PHYLTREE_NODE *node2)
{
  PHYLTREE_NODE *n;
  double d = 0.0;

  n = node1;
  while (n)
  {
    n->tmpinfo = 1;
    n = n->ancestor;
  }
  n = node2;
  while (n)
  {
    if (n->tmpinfo == 1)
    {
      n->tmpinfo = 2;
      break;
    }
    d += n->length;
    n = n->ancestor;
  }
  if (n == NULL)
  {
    n = node1;
    while (n)
    {
      n->tmpinfo = 0;
      n = n->ancestor;
    }
    return (PHYLERR_NOCOMMONANCESTOR);
  }
  n = node1;
  while (n->tmpinfo != 2)
  {
    n->tmpinfo = 0;
    d += n->length;
    n = n->ancestor;
  }
  while (n)
  {
    n->tmpinfo = 0;
    n = n->ancestor;
  }
  return (d);
}

