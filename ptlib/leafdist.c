#include "ptlibin.h"


double phyl_leafdistance(PHYLTREE *ptree, const char *name1, const char *name2)
{
  long i;
  PHYLTREE_NODE *node1 = NULL, *node2 = NULL;

  for (i = 0; i < ptree->num_leaves; i++)
  {
    if (!strcmp(ptree->leaf[i]->name, name1))
    {
      node1 = ptree->leaf[i];
      break;
    }
  }
  if (node1 == NULL)
    return (PHYLERR_NOTINTREE);
  for (i = 0; i < ptree->num_leaves; i++)
  {
    if (!strcmp(ptree->leaf[i]->name, name2))
    {
      node2 = ptree->leaf[i];
      break;
    }
  }
  if (node2 == NULL)
    return (PHYLERR_NOTINTREE);
  return (phyl_nodedistance(node1, node2));
}

