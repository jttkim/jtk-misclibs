#include <stdio.h>
#include <string.h>

#include "gnlibin.h"


/*
 * Save the subtree attached to a node to file f.
 * Note: Only offspring nodes that are still in memory are
 *   saved. Since the number of remaining offspring is not
 *   updated upon trimming tree branches (pointers to
 *   nodes being deleted are just set to NULL), the
 *   offspring_arrsize fields will likely change after saving
 *   and restoring a genealogy tree.
 */

static int save_gn_subtree(const GN_NODE *gn, FILE *f)
{
  long i;
  int return_code;

  if ((return_code = gn_save_id(&(gn->node_id), f)) < 0)
    return (return_code);
  if (gn->ancestor)
  {
    if ((return_code = gn_save_id(&(gn->ancestor->node_id), f)) < 0)
      return (return_code);
  }
  else
    fprintf(f, "!\n");
  fprintf(f, "%ld\n", gn->branch_generation);
  fprintf(f, "%ld\n", gn->num_offspring);
  fprintf(f, "%ld\n", gn->lifespan);
  fprintf(f, "%ld\n", gn->num_alive);
  fprintf(f, "%s\n", gn->name);
  if (gn->genome)
  {
    fprintf(f, "%lu\n", (unsigned long) strlen(gn->genome));
    fprintf(f, "%s\n", gn->genome);
  }
  else
    fprintf(f, "0\n");
  if (ferror(f))
    return (GNERR_FILE);
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
      save_gn_subtree(gn->offspring[i], f);
  }
  return (0);
}


int gn_save_tree(const GN_TREE *gtree, FILE *f)
{
  GN_NODELIST_ELEMENT *gl;
  long num_rootnodes;
  int return_code;

  fprintf(f, "BEGIN STORED PHYLO TREE\n");
  fprintf(f, "%d\n", gtree->pruning_strategy);
  fprintf(f, "%ld\n", gtree->num_nodes);
  fprintf(f, "%ld\n", gtree->num_alive);
  num_rootnodes = 0;
  gl = gtree->root_list;
  while (gl)
  {
    num_rootnodes++;
    gl = gl->next;
  }
  fprintf(f, "%ld\n", num_rootnodes);
  if ((return_code = gn_save_id(&(gtree->last_id), f)) < 0)
    return (return_code);
  gl = gtree->root_list;
  while (gl)
  {
    if ((return_code = save_gn_subtree(gl->gnode, f)) < 0)
      return (return_code);
    gl = gl->next;
  }
  return (0);
}

