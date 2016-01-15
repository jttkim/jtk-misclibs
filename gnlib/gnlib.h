#ifndef H_GNLIB
#define H_GNLIB

#include <stdio.h>  /* necessary for definition of FILE */
#include "gndefs.h"
#include "gntypes.h"


#ifdef __cplusplus

#include <iostream>
#include <fstream>

extern "C" {

#endif

/*
 * Functions to init and discard trees.
 */

extern int gn_free_tree(GN_TREE *gtree);
extern void gn_init_tree(GN_TREE *gtree);

/*
 * Compute genealogical distance
 */

extern long gn_distance(const GN_TREE *gtree, GN_NODE *p1, GN_NODE *p2, long g);

/*
 * Create new nodes and update upon death
 */

extern int gn_new_treenode(GN_TREE *gtree, const GN_NODE_ID *p_ancestor_id, long generation, const char *name, const char *genome, GN_NODE_ID *new_nid);
extern int gn_node_death(GN_TREE *gtree, const GN_NODE_ID *gn_nid, long death_generation);

/*
 * Generate unique string identifying a node
 */

extern char *gn_node_idstring(const GN_NODE_ID *id, char *buf);

/*
 * Copy a genealogy tree in memory
 */

extern int gn_copy_tree(GN_TREE *target, const GN_TREE *source);

/*
 * Print a PHYLIP compatible representation of current tree
 */

extern int gn_print_jftrees(const GN_TREE *gtree, long generation, FILE *f);
extern int gn_fprint_history(const GN_TREE *gtree, FILE *f, const GN_NODE_ID *id);
extern int gn_fprint_postscript(const GN_TREE *gtree, FILE *f, long generation, double width, double height, double fontheight, const char *label);

/*
 * Save and restore a tree to a file
 */

extern int gn_read_tree(GN_TREE *gtree, FILE *f);
extern int gn_save_tree(const GN_TREE *gtree, FILE *f);

/*
 * Save and restore node IDs to a file
 */

extern int gn_read_id(GN_NODE_ID *gn_nid, FILE *f);
extern int gn_save_id(const GN_NODE_ID *gn_nid, FILE *f);

/*
 * determine some statistics of tree
 */


extern long gn_tree_height(const GN_TREE *gtree, long generation);
extern long gn_tree_length(const GN_TREE *gtree, long generation);
extern double gn_ddc(const GN_TREE *gtree, long generation);

/*
 * dump node contents to stdout, for debugging purposes
 */

extern int gn_print_node(const GN_NODE *gn);
extern int gn_print_subtree(const GN_NODE *gn);
extern int gn_print_tree(const GN_TREE *gtree);

#ifdef __cplusplus
}

extern std::ostream &operator << (std::ostream &s, const GN_NODE_ID &nid);
extern std::istream &operator >> (std::istream &s, GN_NODE_ID &nid);
extern std::ostream &operator << (std::ostream &s, const GN_TREE &gn_tree);

extern std::istream &operator >> (std::istream &s, GN_TREE &gn_tree);

extern int gn_ostream_jftrees(const GN_TREE *gtree, long generation, std::ostream &s);
extern int gn_ostream_ddistribution_rle(const GN_TREE *gtree, long generation, std::ostream &s);

#endif


#endif
