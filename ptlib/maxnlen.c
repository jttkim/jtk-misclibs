#include <string.h>

#include "ptlibin.h"


static int max_nlength(const PHYLTREE_NODE *node, const PHYL_LEAFATTRIBUTE *attrlist)
{
  long i;
  int m, l;
  const PHYL_LEAFATTRIBUTE *a;
  const char *name;

  a = phyl_find_leafattr(node->name, attrlist);
  if (a && a->printname[0])
    name = a->printname;
  else
    name = node->name;
  m = strlen(name);
  if (a && a->species[0])
    m += strlen(a->species) + 3;
  for (i = 0; i < node->num_descendants; i++)
  {
    if ((l = max_nlength(node->descendant[i], attrlist)) > m)
      m = l;
  }
  return (m);
}


int phyl_max_nodenamelength(const PHYLTREE *ptree, const PHYL_LEAFATTRIBUTE *attrlist)
{
  return (max_nlength(ptree->root, attrlist));
}

