/* $Id: ptlib.h,v 1.3 2000/01/24 00:56:19 kim Exp $ */
/*
 * $Log: ptlib.h,v $
 * Revision 1.3  2000/01/24 00:56:19  kim
 * added phyl_max_abs_thickness() function, implemented plotting edges with
 * negative thickness in red
 *
 * Revision 1.2  2000/01/20 01:26:11  kim
 * Created kludge: In phyl_topotreedist, edge lengths are written into
 * thickness of corresponding edges as a side effect.
 *
 */

#ifndef H_PTLIB
#define H_PTLIB

#include <stdio.h>

#include "pttypes.h"

/*** init, manipulate and free trees ****************************************/

extern void phyl_init_tree(PHYLTREE *ptree);
extern PHYLTREE_NODE *add_node(PHYLTREE_NODE *root);
extern void phyl_free_tree(PHYLTREE *tree);


/*** get info about trees ***************************************************/

extern long phyl_num_leaves(const PHYLTREE_NODE *node);
extern long phyl_max_edgecounter(const PHYLTREE *ptree);
extern double phyl_max_edgelength(const PHYLTREE *ptree);
extern double phyl_max_thickness(const PHYLTREE *ptree);
extern double phyl_max_abs_thickness(const PHYLTREE *ptree);
extern int phyl_max_nodenamelength(const PHYLTREE *ptree, const PHYL_LEAFATTRIBUTE *attrlist);

/* compute distance between nodes / leaves */
extern double phyl_nodedistance(PHYLTREE_NODE *node1, PHYLTREE_NODE *node2);
extern double phyl_leafdistance(PHYLTREE *ptree, const char *name1, const char *name2);

extern double phyl_min_nodelevel(const PHYLTREE_NODE *node, double ancestor_level);
extern double phyl_max_nodelevel(const PHYLTREE_NODE *node, double ancestor_level);

extern double phyl_treeheight(const PHYLTREE_NODE *node);

extern int phyl_lengthtype_distr(const PHYLTREE *ptree, long type, long *hgram, long hgram_size, double hgram_area);

/* compute dT score */
extern long phyl_topotreedist(PHYLTREE *tree1, PHYLTREE *tree2);

/*** print functions (mainly for debugging purposes) ************************/

extern void print_descendants(PHYLTREE_NODE *node);
extern void print_subtree(PHYLTREE_NODE *node);
extern void print_tree(PHYLTREE *tree);
extern void print_set(PHYL_LEAFSET *set, PHYLTREE_NODE **leaf);


/*** generate a PostScript representation of a tree *************************/

extern int phyl_pstree(FILE *f, const PHYLTREE *tree, double x0, double y0, double width, double height,
        const PHYL_PSINFO *psinfo);
int phyl_ps_utree(FILE *f, const PHYLTREE *tree, double x0, double y0, double width, double height,
        const PHYL_PSINFO *psinfo);

/* stuff for coloring leaves in PostScript printouts */
const PHYL_LEAFATTRIBUTE *phyl_find_leafattr(const char *name, const PHYL_LEAFATTRIBUTE *attrlist);
extern void phyl_compute_leafcolors(PHYL_LEAFATTRIBUTE *attrlist);
extern void phyl_free_attrlist(PHYL_LEAFATTRIBUTE *attrlist);
extern int phyl_read_leafattrs(FILE *f, PHYL_LEAFATTRIBUTE **attr);

/* Add this function for a standalone version of ptlib */
/* extern char *ps_string(const char *s, char *buf);   */


/*** file stuff *************************************************************/

extern int phyl_read_tree(FILE *f, PHYLTREE *tree);


/*** generate random binary tree ********************************************/

extern int phyl_rndbin_tree(long num_leaves, PHYLTREE *tree, int seed);


/*** manipulate tree nodes and trees ****************************************/

/* sort leaves in leaf array, lexicographically according to names */
extern void phyl_sort_leaves(PHYLTREE *tree);

extern void phyl_set_constlength(PHYLTREE *ptree, double length);
extern void phyl_set_minlength(PHYLTREE *ptree, double min_length);
extern void phyl_set_thickness(PHYLTREE *ptree, double thickness);
extern void phyl_set_edges(PHYLTREE *ptree, long edge_info, long edge_counter);
extern void phyl_inf2thick(PHYLTREE *ptree, int thickinfo, double thick);
extern void phyl_multiply_thick(PHYLTREE *ptree, double factor);

extern void phyl_counter2thick(PHYLTREE *ptree, double thick);
extern int phyl_tree2thick(PHYLTREE *tree1, const PHYLTREE *tree2, double max_thickness);

/* for transforming consensus trees into strict consensus trees */
extern int phyl_merge_shortbranches(PHYLTREE *ptree, double threshold);


/*** stuff for handling leaf sets *******************************************/

extern void phyl_get_leafsets(const PHYLTREE_NODE *node, long *num_edges, PHYL_LEAFSET *set);
extern int phyl_cmp_leafset(const void *s1, const void *s2);
extern int phyl_alloc_set(const PHYLTREE *ptree, PHYL_LEAFSET *set);
extern void phyl_free_set(PHYL_LEAFSET *set);
extern int phyl_strtoset(const PHYLTREE *ptree, char **name, PHYL_LEAFSET *set);

#endif

