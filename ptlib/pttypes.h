/* $Id: pttypes.h,v 1.2 2000/01/20 01:26:11 kim Exp $ */
/*
 * $Log: pttypes.h,v $
 * Revision 1.2  2000/01/20 01:26:11  kim
 * Created kludge: In phyl_topotreedist, edge lengths are written into
 * thickness of corresponding edges as a side effect.
 *
 */

#ifndef H_PHYLTYPS
#define H_PHYLTYPS

#include <stddef.h>

#include "ptdefs.h"


typedef struct tag_treenode
{
  struct tag_treenode  *ancestor;
  double                length;
  double                thick;
  int                   tmpinfo;
  int                   edge_info;
  long                  edge_counter;
  long                  num_descendants;
  struct tag_treenode **descendant;
  long                  leaf_index;
  char                  name[PHYL_MAX_NODENAMELENGTH + 1];
} PHYLTREE_NODE;


typedef struct
{
  PHYLTREE_NODE  *root;
  long            num_leaves;
  PHYLTREE_NODE **leaf;
  int             lengthinfo_complete;
} PHYLTREE;


typedef struct
{
  PHYLTREE_NODE *node;
  size_t    size;
  char     *flag;
} PHYL_LEAFSET;


typedef struct tag_leafattr
{
  struct tag_leafattr *next;
  char name[PHYL_MAX_NODENAMELENGTH];
  char species[PHYL_SPECNAME_LENGTH];
  char printname[PHYL_PRINTNAME_LENGTH];
  char class[PHYL_MAX_NODENAMELENGTH];
  double red, green, blue;
  double box_red, box_green, box_blue;
} PHYL_LEAFATTRIBUTE;


typedef struct
{
  char   *fontname;
  char   *spec_fontname;
  double  min_fontsize;
  double  max_fontsize;
  char   *label;
  double  tic_length;
  double  label_start;
  double  label_fontsize;
  double  linewidth;
  double  angle_min;
  double  angle_limit;
  double  tree_height;
  int     print_leafnames;
  int     print_internal_names;
  int     print_edgelengths;
  int     leafnames_at_max;
  int     dotted_nodes;
  long    bootstrap_numtrees;
  PHYL_LEAFATTRIBUTE *attrlist;
} PHYL_PSINFO;

#endif

