#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gnlibin.h"


static int read_long(long *n, FILE *f)
{
  char buf[GN_MAX_LINELENGTH], *cp;

  if (fgets(buf, 30, f) == NULL)
    return (GNERR_FILE);
  *n = strtol(buf, &cp, 10);
  if (cp == buf)
    return (GNERR_PARSE);
  return (0);
}


static int read_gn_subtree(GN_TREE *gtree, FILE *f)
{
  GN_NODE_ID id, ancestor_id;
  GN_NODE *gn;
  long i, num_offspring, num_alive;
  int return_code;
  long glen;
  char *gbuf;

  if ((return_code = gn_read_id(&(id), f)) < 0)
    return (return_code);
  if ((return_code = gn_read_id(&(ancestor_id), f)) < 0)
    return (return_code);
  if (return_code == GN_UNSPEC_ID)
  {
    if ((return_code = gn_add_treenode(gtree, &id, NULL, &gn)) < 0)
      return (return_code);
  }
  else
  {
    if ((return_code = gn_add_treenode(gtree, &id, &ancestor_id, &gn)) < 0)
      return (return_code);
  }
#ifdef GNLIB_DEBUG
  fprintf(stderr, "read_gn_subtree: node ID is (%ld, %ld)\n", gn->node_id.generation, gn->node_id.id);
#endif
  if ((return_code = read_long(&(gn->branch_generation), f)) < 0)
    return (return_code);
  if ((return_code = read_long(&(num_offspring), f)) < 0)
    return (return_code);
  if ((return_code = read_long(&(gn->lifespan), f)) < 0)
    return (return_code);
  if ((return_code = read_long(&num_alive, f)) < 0)
    return (return_code);
  if (gn->lifespan == GN_STILL_ALIVE)
  {
    if ((return_code = gn_addtonlist(&(gtree->alive_list), gn)) < 0)
    {
      gn_remove_treenode(gtree, gn);
      return (return_code);
    }
    gtree->num_alive++;
  }
  if (fgets(gn->name, GN_MAX_NODENAME_LENGTH, f) == NULL)
    return (GNERR_FILE);
  gn->name[GN_MAX_NODENAME_LENGTH] = '\0';
  for (i = strlen(gn->name) - 1; (i >= 0) && iscntrl(gn->name[i]); i--)
    gn->name[i] = '\0';
#ifdef GNLIB_DEBUG
  fprintf(stderr, "read_gn_subtree: read name \"%s\"\n", gn->name);
#endif
  if ((return_code = read_long(&glen, f)) < 0)
    return (return_code);
  if (glen)
  {
    gbuf = (char *) malloc(glen + 1);
    if (gbuf == NULL)
      return (GNERR_MEM);
    fgets(gbuf, glen + 1, f);
    gbuf[glen] = '\0';
    gn->genome = gbuf;
#ifdef GNLIB_DEBUG
    fprintf(stderr, "read_gn_subtree: read genome \"%s\"\n", gn->genome);
#endif
  }
  else
  {
#ifdef GNLIB_DEBUG
    fprintf(stderr, "read_gn_subtree: no genome\n");
#endif
    gn->genome = NULL;
  }
  if (gn->lifespan == GN_STILL_ALIVE)
  {
    while (gn)
    {
      gn->num_alive++;
      gn = gn->ancestor;
    }
  }
  for (i = 0; i < num_offspring; i++)
  {
    if ((return_code = read_gn_subtree(gtree, f)) < 0)
      return (return_code);
  }
  return (0);
}


int gn_read_tree(GN_TREE *gtree, FILE *f)
{
  char buf[GN_MAX_LINELENGTH];
  GN_NODELIST_ELEMENT *gl;
  long expect_num_nodes, expect_num_alive, num_rootnodes, i;
  int return_code;

  do
  {
    if (fgets(buf, GN_MAX_LINELENGTH, f) == NULL)
      return (GNERR_FILE);
  }
  while (strcmp(buf, "BEGIN STORED PHYLO TREE\n"));
  gn_init_tree(gtree);
  if ((return_code = read_long(&i, f)) < 0)
    return (return_code);
  gtree->pruning_strategy = i;
  if ((return_code = read_long(&expect_num_nodes, f)) < 0)
    return (return_code);
  if ((return_code = read_long(&expect_num_alive, f)) < 0)
    return (return_code);
  if ((return_code = read_long(&num_rootnodes, f)) < 0)
    return (return_code);
  if ((return_code = gn_read_id(&(gtree->last_id), f)) < 0)
  {
    gn_init_tree(gtree);
    return (return_code);
  }
  for (i = 0; i < num_rootnodes; i++)
  {
    if ((return_code = read_gn_subtree(gtree, f)) < 0)
    {
      gn_free_tree(gtree);
      return (return_code);
    }
    if (gtree->root_list->next)
    {
      gl = gtree->root_list->next;
      while (gl->next)
	gl = gl->next;
      gl->next = gtree->root_list;
      gtree->root_list = gtree->root_list->next;
      gl->next->next = NULL;
    }
  }
  if ((expect_num_nodes != gtree->num_nodes) || (expect_num_alive != gtree->num_alive))
  {
    gn_free_tree(gtree);
    return (GNERR_CORRUPTFILE);
  }
  return (0);
}

