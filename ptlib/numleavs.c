#include "ptlibin.h"


long phyl_num_leaves(const PHYLTREE_NODE *node)
{
  long n = 0, i;

  if (node->num_descendants)
  {
    for (i = 0; i < node->num_descendants; i++)
    {
      n += phyl_num_leaves(node->descendant[i]);
    }
    return (n);
  }
  else
  {
    return (1);
  }
}
