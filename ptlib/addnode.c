#include <stdio.h>
#include <stdlib.h>

#include "ptlibin.h"


PHYLTREE_NODE *add_node(PHYLTREE_NODE *root)
{
  PHYLTREE_NODE *node, **dsc;
  if ((node = (PHYLTREE_NODE *) malloc(sizeof(PHYLTREE_NODE))) == NULL)
    return (NULL);
  if (root)
  {
    if (root->num_descendants)
    {
      if ((dsc = (PHYLTREE_NODE **) realloc(root->descendant, (root->num_descendants + 1) * sizeof(PHYLTREE_NODE *))) == NULL)
      {
        free(node);
        return (NULL);
      }
      root->descendant = dsc;
    }
    else
    {
      if ((root->descendant = (PHYLTREE_NODE **) malloc(sizeof(PHYLTREE_NODE *))) == NULL)
      {
        free(node);
        return (NULL);
      }
    }
    root->descendant[root->num_descendants] = node;
    root->num_descendants++;
  }
  node->ancestor = root;
  node->length = 0.0;
  node->tmpinfo = 0;
  node->edge_info = PHYLEDGINF_NONE;
  node->edge_counter = 0;
  node->num_descendants = 0;
  node->descendant = NULL;
  node->leaf_index = -1;
  node->name[0] = '\0';
  return (node);
}

