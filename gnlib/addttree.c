#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gnlibin.h"


/*
 * Add a node to a phylogenetic tree. The ID of the node to be
 * added and the ID of its ancestors are passed as arguments.
 * Parameters:
 *   gtree: The tree to which a node is added
 *   gn_nid: The ID of the node to be added.
 *   p_ancestor_id: ID of the ancestor where new node should be rooted.
 *   gn_return: The address of newly created node is passed back through
 *       this pointer.
 * Return: 0 upon success, negative error code otherwise.
 *   Pointer to newly created node is returned via gn_return if that
 *   pointer is not NULL.
 * Note: The fields lifespan and num_alive are not initialized
 *   by gn_add_treenode. The caller is responsible for placing
 *   appropriate values in them.
 */

int gn_add_treenode(GN_TREE *gtree, const GN_NODE_ID *gn_nid, const GN_NODE_ID *p_ancestor_id, GN_NODE **gn_return)
{
  GN_NODE *gn, *p_ancestor, **pgn;
  int return_code;

  if (gn_find_node(gtree, gn_nid) != NULL)
    return (GNERR_DUPLICID);
  if (p_ancestor_id)
  {
    if ((p_ancestor = gn_find_node(gtree, p_ancestor_id)) == NULL)
      return (GNERR_UNKNODE);
  }
  else
    p_ancestor = NULL;
  if ((gn = (GN_NODE *) malloc(sizeof(GN_NODE))) == NULL)
    return (GNERR_MEM);
  gn->node_id = *gn_nid;
  gn->ancestor = p_ancestor;
  gn->num_offspring = 0;
  gn->offspring_arrsize = 0;
  gn->offspring = NULL;
  gn->genome = NULL;
  gn->name[0] = '\0';
  if (p_ancestor == NULL)
  {
    if ((return_code = gn_addtonlist(&(gtree->root_list), gn)) < 0)
    {
      free(gn);
      return (return_code);
    }
  }
  else
  {
    if (p_ancestor->offspring_arrsize)
      pgn = (GN_NODE **) realloc(p_ancestor->offspring, (p_ancestor->offspring_arrsize + 1) * sizeof(GN_NODE *));
    else
      pgn = (GN_NODE **) malloc(sizeof(GN_NODE *));
    if (pgn == NULL)
    {
      free(gn);
      return (GNERR_MEM);
    }
    p_ancestor->offspring = pgn;
    p_ancestor->offspring[p_ancestor->offspring_arrsize++] = gn;
    p_ancestor->num_offspring++;
  }
  gtree->num_nodes++;
  if (gn_return)
    *gn_return = gn;
  return (0);
}

