#include <string.h>

#include "gnlibin.h"


static int copy_subtree(GN_TREE *gtree, const GN_NODE *source, GN_NODE *p_ancestor)
{
  int ret_code;
  long i;
  GN_NODE *new_node;

  if (p_ancestor)
    ret_code = gn_add_treenode(gtree, &(source->node_id), &(p_ancestor->node_id), &new_node);
  else
    ret_code = gn_add_treenode(gtree, &(source->node_id), NULL, &new_node);
  if (ret_code < 0)
    return (ret_code);
  new_node->branch_generation = source->branch_generation;
  new_node->lifespan = source->lifespan;
  new_node->num_alive = source->num_alive;
  strncpy(new_node->name, source->name, GN_MAX_NODENAME_LENGTH);
  if (new_node->lifespan == GN_STILL_ALIVE)
  {
    ret_code = gn_addtonlist(&(gtree->alive_list), new_node);
    if (ret_code < 0)
      return (ret_code);
  }
  for (i = 0; i < source->offspring_arrsize; i++)
  {
    if (source->offspring[i])
    {
      ret_code = copy_subtree(gtree, source->offspring[i], new_node);
      if (ret_code < 0)
	return (ret_code);
    }
  }
  return (0);
}


int gn_copy_tree(GN_TREE *target, const GN_TREE *source)
{
  GN_NODELIST_ELEMENT *gl;
  int ret_code;

  if (target->num_nodes > 0)
    return (GNERR_NOTEMPTY);
  gn_init_tree(target);
  gl = source->root_list;
  while (gl)
  {
    ret_code = copy_subtree(target, gl->gnode, NULL);
    if (ret_code < 0)
      return (ret_code);
    gl = gl->next;
  }
  target->num_alive = source->num_alive;
  return (0);
}

