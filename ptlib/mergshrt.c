#include <stdlib.h>

#include "ptlibin.h"


static int merge_shortbranches(PHYLTREE_NODE *node, double threshold)
{
  int ret_code;
  long i, j;
  PHYLTREE_NODE **d, *n;

  i = 0;
  while (i < node->num_descendants)
  {
    if ((node->descendant[i]->length < threshold)
            && (node->descendant[i]->num_descendants > 0))
    {
      if ((d = realloc(node->descendant,
              (node->num_descendants + node->descendant[i]->num_descendants - 1)
              * sizeof(PHYLTREE_NODE *))) == NULL)
        return (PHYLERR_MEM);
      node->descendant = d;
      n = node->descendant[i];
      for (j = i + 1; j < node->num_descendants; j++)
        node->descendant[j - 1] = node->descendant[j];
      node->num_descendants--;
      for (j = 0; j < n->num_descendants; j++)
      {
        node->descendant[node->num_descendants + j] = n->descendant[j];
        n->descendant[j]->ancestor = node;
      }
      node->num_descendants += n->num_descendants;
      free(n->descendant);
      free(n);
    }
    else
    {
      if ((ret_code = merge_shortbranches(node->descendant[i], threshold)) < 0)
        return (ret_code);
      i++;
    }
  }
  if (node->num_descendants == 0)
    node->length = threshold;
  return (0);
}


int phyl_merge_shortbranches(PHYLTREE *ptree, double threshold)
{
  return (merge_shortbranches(ptree->root, threshold));
}

