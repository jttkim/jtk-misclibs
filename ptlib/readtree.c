#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ptlibin.h"


typedef struct
{
  FILE *f;
  int pos;
  char s[PHYL_MAX_SLEN];
} TREE_SOURCE;


static int read_treestring(TREE_SOURCE *ts)
{
  int i;

  if (fgets(ts->s, PHYL_MAX_SLEN, ts->f) == NULL)
    return (PHYLERR_FILE);
  for (i = strlen(ts->s); i && iscntrl(ts->s[i]); i--)
    ts->s[i] = '\0';
  ts->pos = 0;
  /* fprintf(stderr, "got string: \"%s\"\n", ts->s); */
  return (0);
}


static void fprint_treestring(FILE *f, const TREE_SOURCE *ts)
{
  int i;

  fprintf(f, "%s\n", ts->s);
  for (i = 0; i < ts->pos; i++)
    fprintf(f, " ");
  fprintf(f, "^\n");
}


static int get_nodename(TREE_SOURCE *ts, PHYLTREE_NODE *node)
{
  int n;

  n = 0;
  while ((ts->s[ts->pos] != ')') && (ts->s[ts->pos] != ',') && (ts->s[ts->pos] != ':'))
  {
    if (!ts->s[ts->pos])
    {
      if (read_treestring(ts) < 0)
      {
        /* fprintf(stderr, "file error or unexpected end of file\n"); */
        return (PHYLERR_FILE);
      }
    }
    if ((isprint(ts->s[ts->pos])) && (n < PHYL_MAX_NODENAMELENGTH))
      node->name[n++] = ts->s[ts->pos++];
  }
  node->name[n] = '\0';
  /* fprintf(stderr, "got node name \"%s\"\n", node->name); */
  return (0);
}


static int get_branchlength(TREE_SOURCE *ts, PHYLTREE_NODE *node)
{
  int n;
  char *p, buf[PHYL_MAX_SLEN];

  for (n = 0; n < PHYL_MAX_SLEN - 1; n++)
  {
    if (!ts->s[ts->pos])
    {
      if (read_treestring(ts) < 0)
      {
        /* fprintf(stderr, "file error or unexpected end of file\n"); */
        return (PHYLERR_FILE);
      }
    }
    if ((ts->s[ts->pos] == ')') || (ts->s[ts->pos] == ',') || (ts->s[ts->pos] == ':') || isspace(ts->s[ts->pos]))
      break;
    buf[n] = ts->s[ts->pos];
    ts->pos++;
  }
  buf[n] = '\0';
  /* printf("length specifier: %s\n", buf); */
  node->length = strtod(buf, &p);
  if (p == buf)
  {
    node->length = 0.0;
    return (PHYLERR_PARSE);
  }
  /* fprintf(stderr, "got branch length %f\n", node->length); */
  return (0);
}


static int read_node(TREE_SOURCE *ts, PHYLTREE_NODE *root, PHYLTREE *tree)
{
  PHYLTREE_NODE *node, **leaf;
  int lengthinfo_present, ret_code;

  while (ts->s[ts->pos] != '(')
  {
    if (!ts->s[ts->pos])
    {
      if (read_treestring(ts) < 0)
      {
        /* fprintf(stderr, "file error or unexpected end of file\n"); */
        return (PHYLERR_FILE);
      }
    }
    else
      ts->pos++;
  }
  ts->pos++;
  if ((node = add_node(root)) == NULL)
  {
    fprintf(stderr, "error: out of memory\n");
    return (PHYLERR_MEM);
  }
  lengthinfo_present = 0;
  while (ts->s[ts->pos] != ')')
  {
    while (isspace(ts->s[ts->pos]))
    {
      ts->pos++;
    }
    switch (ts->s[ts->pos])
    {
    case '\0':
      if (read_treestring(ts) < 0)
      {
        /* fprintf(stderr, "file error or unexpected end of file\n"); */
        return (PHYLERR_FILE);
      }
      break;
    case ':':
      ts->pos++;
      /* printf("getting length\n"); */
      if (!ts->s[ts->pos])
      {
	if (read_treestring(ts) < 0)
	  return (PHYLERR_FILE);
      }
      if (lengthinfo_present)
      {
        fprintf(stderr, "error: multiple length specification\n");
        fprint_treestring(stderr, ts);
        return (PHYLERR_PARSE);
      }
      if ((ret_code = get_branchlength(ts, node)) < 0)
      {
        fprintf(stderr, "error: invalid length specification\n");
        fprint_treestring(stderr, ts);
        return (ret_code);
      }
      lengthinfo_present = 1;
      break;
    case ',':
      ts->pos++;
      /* fprintf(stderr, "found node separator\n"); */
      if ((node = add_node(root)) == NULL)
      {
        fprintf(stderr, "error: out of memory\n");
        return (PHYLERR_MEM);
      }
      tree->lengthinfo_complete &= lengthinfo_present;
      lengthinfo_present = 0;
      break;
    case '(':
      /* fprintf(stderr, "found_subtree\n"); */
      if ((ret_code = read_node(ts, node, tree)) < 0)
        return (ret_code);
      break;
    default:
      /* fprintf(stderr, "found leaf, %d descendants\n", node->num_descendants); */
      if ((ret_code = get_nodename(ts, node)) < 0)
      {
        fprintf(stderr, "error: invalid node name\n");
        fprint_treestring(stderr, ts);
        return (ret_code);
      }
      if (node->num_descendants == 0)
      {
        if (tree->num_leaves)
        {
          if ((leaf = (PHYLTREE_NODE **) realloc(tree->leaf, (tree->num_leaves + 1) * sizeof(PHYLTREE_NODE *))) == NULL)
          {
            fprintf(stderr, "error: out of memory\n");
            return (PHYLERR_MEM);
          }
          tree->leaf = leaf;
        }
        else
        {
          if ((tree->leaf = (PHYLTREE_NODE **) malloc(sizeof(PHYLTREE_NODE *))) == NULL)
          {
            fprintf(stderr, "error: out of memory\n");
            return (PHYLERR_MEM);
          }
        }
        tree->leaf[tree->num_leaves] = node;
        node->leaf_index = tree->num_leaves;
        /* printf("added leaf \"%s\"\n", tree->leaf[tree->num_leaves]->name); */
        tree->num_leaves++;
      }
    }
  }
  tree->lengthinfo_complete &= lengthinfo_present;
  ts->pos++;
  return (0);
}


int phyl_read_tree(FILE *f, PHYLTREE *tree)
{
  TREE_SOURCE tree_source;
  int ret_code;

  tree_source.f = f;
  tree_source.s[0] = '\0';
  tree_source.pos = 0;
  tree->lengthinfo_complete = 1;
  if ((tree->root = add_node(NULL)) == NULL)
  {
    fprintf(stderr, "error: out of memory\n");
    return (PHYLERR_MEM);
  }
  if ((ret_code = read_node(&tree_source, tree->root, tree)) < 0)
  {
    if ((ret_code != PHYLERR_FILE) || (tree->root->num_descendants != 0))
    {
      phyl_free_tree(tree);
      return (ret_code);
    }
    phyl_free_tree(tree);
  }
  return (0);
}
