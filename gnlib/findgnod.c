#include <stdio.h>

#include "gnlibin.h"


GN_NODE *find_in_subtree(const GN_NODE *gn, const GN_NODE_ID *gn_nid)
{
  long i;
  GN_NODE *p;

  if ((gn->node_id.generation == gn_nid->generation) && (gn->node_id.id == gn_nid->id))
    return ((GN_NODE *) gn);
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
    {
      p = find_in_subtree(gn->offspring[i], gn_nid);
      if (p)
        return (p);
    }
  }
  return (NULL);
}


/*
 * Find the pointer to a genealogy node in a tree.
 * Return: Pointer to node if node is found, NULL otherwise.
 */

GN_NODE *gn_find_node(const GN_TREE *gtree, const GN_NODE_ID *gn_nid)
{
  GN_NODE *p;
  GN_NODELIST_ELEMENT *gl;

  gl = gtree->root_list;
  while (gl)
  {
    p = find_in_subtree(gl->gnode, gn_nid);
    if (p)
      return (p);
    gl = gl->next;
  }
  return (NULL);
}

