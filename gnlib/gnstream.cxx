#include <ctype.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gnlibin.h"

#define skip_newline(s) { char c; s.get(c); if (c != '\n') s.putback(c); }


static int gn_defined_id(const GN_NODE_ID *id)
{
  return (id->id >= 0);
}


int gn_ostream_ddistribution_rle(const GN_TREE *gtree, long generation, ostream &s)
{
  long res, d, i, *dd, rle_length;
  GN_NODELIST_ELEMENT *gl, *gl1;

  res = gn_tree_height(gtree, generation);
  if (res <= 0)
  {
    s << 0 << endl;
    return (0);
  }
  res++;
  dd = (long *) malloc(res * sizeof(long));
  if (dd == NULL)
  {
    s << 0 << endl;
    return (GNERR_MEM);
  }
  for (d = 0; d < res; d++)
    dd[d] = 0;
  for (gl = gtree->alive_list; gl; gl = gl->next)
  {
    for (gl1 = gl->next; gl1; gl1 = gl1->next)
    {
      d = gn_distance(gtree, gl->gnode, gl1->gnode, generation);
      if (d < 0)
        continue;
      dd[d]++;
    }
  }
  rle_length = 0;
  for (d = 0; d < res; d++)
  {
    for (i = d + 1; (i < res) && (dd[i] == dd[d]); i++)
      ;
    if ((i - d) > 2)
    {
      dd[rle_length++] = d - i;
      dd[rle_length++] = dd[i - 1];
      d = i - 1;
    }
    else
      dd[rle_length++] = dd[d];
  }
  s << rle_length << endl;
  for (i = 0; i < rle_length; i++)
    s << dd[i] << endl;
  free(dd);
  return (0);
}


int gn_ostream_jftrees(const GN_TREE *gtree, long generation, ostream &s)
{
  GN_NODELIST_ELEMENT *gl;
  char *treestring;
  long num_rootnodes;

  if (!s.good())
    cerr << "gnlib warning: writing New Hampshire tree to stream that is not good" << endl;
  num_rootnodes = 0;
  gl = gtree->root_list;
  while (gl)
  {
    num_rootnodes++;
    gl = gl->next;
  }
  s << num_rootnodes << endl;
  gl = gtree->root_list;
  while (gl)
  {
    treestring = gn_jf_treespec(gl->gnode, generation);
    if (treestring)
    {
      s << treestring << endl;
      free(treestring);
    }
    else
    {
      // cerr << "gn_ostream_jftrees: No treestring produced (generation " << generation << ')' << endl;
      return (GNERR_MEM);
    }
    gl = gl->next;
  }
  return (0);
}


ostream &operator << (ostream &s, const GN_NODE_ID &nid)
{
  if (!s.good())
    cerr << "gnlib warning: writing ID to stream that is not good" << endl;
  // s << "---------------" << endl;
  s << nid.generation << endl;
  s << nid.id << endl;
  // s << "- - - - - - - -" << endl;
  return (s);
}


istream &operator >> (istream &s, GN_NODE_ID &nid)
{
  GN_NODE_ID n;
  char buf[256];

  if (!s.good())
    cerr << "gnlib warning: reading ID from stream that is not good" << endl;
  skip_newline(s);
  // s.get(buf, 256, '\n');  // read separator
  // cerr << "GN_NODE_ID >> buf = \"" << buf << "\"" << endl;
  skip_newline(s);
  s.get(buf, 256, '\n');
  if (buf[0] == '!')
  {
    n.id = -1;
    // cerr << "GN_NODE_ID >> undefined ID" << endl;
  }
  else
  {
    n.generation = strtol(buf, NULL, 10);
    s >> n.id;
    // cerr << "GN_NODE_ID >> generation: " << n.generation << ", id: " << n.id << endl;
  }
  skip_newline(s);
  // s.get(buf, 256, '\n');  // read separator
  // cerr << "GN_NODE_ID >> buf = \"" << buf << "\"" << endl;
  nid = n;
  return (s);
}


static ostream &operator << (ostream &s, GN_NODE *gn)
{
  long i;

  if (!s.good())
    cerr << "gnlib warning: writing node to stream that is not good" << endl;
  s << gn->node_id;
  if (gn->ancestor)
         s << gn->ancestor->node_id;
  else
  {
    // s << "-------------" << endl;
    s << '!' << endl;
    // s << "- - - - - - -" << endl;
  }
  s << gn->branch_generation << endl;
  s << gn->num_offspring << endl;
  s << gn->lifespan << endl;
  s << gn->num_alive << endl;
  s << gn->name << endl;
  if (gn->genome)
  {
    s << strlen(gn->genome) << endl;
    s << gn->genome << endl;
  }
  else
    s << "0" << endl;
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
      s << gn->offspring[i];
  }
  return (s);
}


ostream &operator << (ostream &s, const GN_TREE &gn_tree)
{
  GN_NODELIST_ELEMENT *gl;
  long num_rootnodes;

  if (!s.good())
    cerr << "gnlib warning: writing tree to stream that is not good" << endl;
  s << "BEGIN STORED PHYLO TREE" << endl;
  s << gn_tree.pruning_strategy << endl;
  s << gn_tree.num_nodes << endl;
  s << gn_tree.num_alive << endl;
  num_rootnodes = 0;
  gl = gn_tree.root_list;
  while (gl)
  {
    num_rootnodes++;
    gl = gl->next;
  }
  s << num_rootnodes << endl;
  s << gn_tree.last_id;
  gl = gn_tree.root_list;
  while (gl)
  {
    s << gl->gnode;
    gl = gl->next;
  }
  return (s);
}


static int istream_gn_subtree(GN_TREE *gtree, istream &s)
{
  GN_NODE_ID id, ancestor_id;
  GN_NODE *gn, *gn1;
  long i, num_offspring, num_alive;
  int return_code;
  size_t glen;
  char *gbuf;

  if (!s.good())
         cerr << "gnlib warning: reading subtree from stream that is not good" << endl;
  s >> id;
  s >> ancestor_id;
  if (!gn_defined_id(&ancestor_id))
  {
    if ((return_code = gn_add_treenode(gtree, &id, NULL, &gn)) < 0)
    {
      cerr << "Problems adding node to tree in istream_gn_subtree [NULL] (code " << return_code << ')' << endl;
      cerr << id;
      return (return_code);
    }
  }
  else
  {
    if ((return_code = gn_add_treenode(gtree, &id, &ancestor_id, &gn)) < 0)
    {
      cerr << "Problems adding node to tree in istream_gn_subtree [ancestor] (code " << return_code << ')' << endl;
      cerr << id;
      return (return_code);
    }
  }
  gn->num_alive = 0;
  // fprintf(stderr, "gn_add_treenode set gn to %p\n", gn);
  gn1 = gn;
  s >> gn->branch_generation;
  s >> num_offspring;
  s >> gn->lifespan;
  s >> num_alive;
  if ((num_offspring == 0) && (gn->lifespan != GN_STILL_ALIVE))
         cerr << "found a dead node with no offspring" << endl;
  if (gn->lifespan == GN_STILL_ALIVE)
  {
    if ((return_code = gn_addtonlist(&(gtree->alive_list), gn)) < 0)
    {
      cerr << "Problems adding node to living nodes list" << endl;
      gn_remove_treenode(gtree, gn);
      return (return_code);
    }
    gtree->num_alive++;
  }
  skip_newline(s);
  s.get(gn->name, GN_MAX_NODENAME_LENGTH, '\n');
  gn->name[GN_MAX_NODENAME_LENGTH] = '\0';
  for (i = strlen(gn->name) - 1; (i >= 0) && iscntrl(gn->name[i]); i--)
    gn->name[i] = '\0';
  s >> glen;
  if (glen)
  {
    gbuf = (char *) malloc(glen + 1);
    if (gbuf == NULL)
      return (GNERR_MEM);
    skip_newline(s);
    s.get(gbuf, glen + 1, '\n');
    gbuf[glen] = '\0';
    gn->genome = gbuf;
  }
  else
    gn->genome = NULL;
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
    if ((return_code = istream_gn_subtree(gtree, s)) < 0)
      return (return_code);
  }
  // cerr << "Successfully read node" << endl;
  // cerr << "    branch_generation: " << gn1->branch_generation << endl;
  // cerr << "    num_offspring: " << num_offspring <<  endl;
  // cerr << "    lifespan: " << gn1->lifespan << endl;
  return (0);
}


istream &operator >> (istream &s, GN_TREE &gn_tree)
{
  GN_NODELIST_ELEMENT *gl;
  long expect_num_nodes, expect_num_alive, num_rootnodes, i;
  int return_code;
  char buf[256], c;

  if (!s.good())
    cerr << "gnlib warning: reading tree from stream that is not good" << endl;
  gn_init_tree(&gn_tree);
  do
  {
    s.get(buf, 256, '\n');
    do
      s.get(c);
    while (c != '\n');
  }
  while (strcmp(buf, "BEGIN STORED PHYLO TREE"));
  // cerr << "*** buf: \"" << buf << "\"" << endl;
  s >> gn_tree.pruning_strategy;
  s >> expect_num_nodes;
  // cerr << "*** expect_num_nodes: " << expect_num_nodes << endl;
  s >> expect_num_alive;
  // cerr << "*** expect_num_alive: " << expect_num_alive << endl;
  s >> num_rootnodes;
  // cerr << "*** num_rootnodes: " << num_rootnodes << endl;
  s >> gn_tree.last_id;
  // cerr << "GN_TREE >> last_id: generation: " << gn_tree.last_id.generation << ", id: " << gn_tree.last_id.id << endl;
  for (i = 0; i < num_rootnodes; i++)
  {
    return_code = istream_gn_subtree(&gn_tree, s);
    if (return_code < 0)
    {
      cerr << "Problems reading phylo tree" << endl;
      return (s);
    }
    if (gn_tree.root_list->next)
    {
      gl = gn_tree.root_list->next;
      while (gl->next)
        gl = gl->next;
      gl->next = gn_tree.root_list;
      gn_tree.root_list = gn_tree.root_list->next;
      gl->next->next = NULL;
    }
  }
  if ((expect_num_nodes != gn_tree.num_nodes) || (expect_num_alive != gn_tree.num_alive))
  {
    cerr << "Problems reading GN_TREE" << endl;
    cerr << "num_nodes: expected: " << expect_num_nodes << " found: " << gn_tree.num_nodes << endl;
    cerr << "num_alive: expected: " << expect_num_alive << " found: " << gn_tree.num_alive << endl;
  }
  return (s);
}

