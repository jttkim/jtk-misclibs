#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Produce a Felsenstein style string representation of the
 * tree rooting in root. If curr_generation is > -1, branch
 * length information is included. *tree base is used to
 * return the generation of the first split. Because this
 * variable is also used for recursive calls to jf_subtreespec,
 * it must be specified even if this information is not
 * needed (i.e. don't pass invalid or NULL pointers here).
 */

static char *jf_subtreespec(GN_NODE *root, long curr_generation, long *tree_base)
{
  static char branch1_length[20], branch2_length[20];

  char *branch1, *branch2, *tree;
  long  i, branch1_base, branch2_base, tree_strlen;

  tree = NULL;
  if (root->lifespan == GN_STILL_ALIVE)
  {
    if ((tree = (char *) malloc((GN_MAX_NODENAME_LENGTH + 21) * sizeof(char))) == NULL)
    {
      fprintf(stderr, "jf_subtreespec: malloc() failed\n");
      return (NULL);
    }
    gn_sprint_nodename(tree, root);
    *tree_base = curr_generation;
  }
  for (i = root->offspring_arrsize - 1; i >= 0; i--)
  {
    if ((root->offspring[i]) && (root->offspring[i]->num_alive > 0))
    {
      branch1 = tree;
      branch1_base = *tree_base;
      if ((branch2 = jf_subtreespec(root->offspring[i], curr_generation, &branch2_base)) == NULL)
      {
	/* fprintf(stderr, "jf_subtreespec: branch2 is NULL (offspring %ld)\n", i); */
        if (branch1)
          free(branch1);
        return (NULL);
      }
      if (branch1)
      {
        *tree_base = root->offspring[i]->branch_generation;
        if (curr_generation == -1)
        {
          if ((tree = (char *) malloc((strlen(branch1) + strlen(branch2) + 5) * sizeof(char))) == NULL)
          {
	    fprintf(stderr, "jf_subtreespec: malloc() for tree failed\n");
            if (branch1)
              free(branch1);
            if (branch2)
              free(branch2);
            return (NULL);
          }
          sprintf(tree, "(%s,%s)", branch1, branch2);
        }
        else
        {
          sprintf(branch1_length, "%ld", branch1_base - *tree_base);
          sprintf(branch2_length, "%ld", branch2_base - *tree_base);
          tree_strlen = strlen(branch1) + strlen(branch2) + strlen(branch1_length) + strlen(branch2_length) + 6;
          if ((tree = (char *) malloc((tree_strlen + 1) * sizeof(char))) == NULL)
          {
	    fprintf(stderr, "jf_subtreespec: malloc() for tree failed\n");
            if (branch1)
              free(branch1);
            if (branch2)
              free(branch2);
            return (NULL);
          }
          sprintf(tree, "(%s:%s,%s:%s)", branch1, branch1_length, branch2, branch2_length);
        }
        free(branch1);
        free(branch2);
      }
      else
      {
        tree = branch2;
        *tree_base = branch2_base;
      }
    }
  }
  if (tree == NULL)
    fprintf(stderr, "returning tree = NULL\n");
  return (tree);
}


char *gn_jf_treespec(GN_NODE *gn, long generation)
{
  long base_split;
  char *treespec, *t;

  treespec = NULL;
  t = jf_subtreespec(gn, generation, &base_split);
  if (t)
  {
    if (*t == '(')
      treespec = t;
    else
    {
      treespec = (char *) malloc(strlen(t) + 4);
      if (treespec)
        sprintf(treespec, "(%s)", t);
      free(t);
    }
  }
  return (treespec);
}

