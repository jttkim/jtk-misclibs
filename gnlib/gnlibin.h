#ifndef H_GNLIBIN
#define H_GNLIBIN

#include <stdio.h>  /* necessary for definition of FILE */
#ifdef MEMDEBUG
#  ifdef __atarist__
#    include <memdebug.h>
#  else
#    include "memdebug.h"
#  endif
#endif

#include "gndefs.h"
#include "gntypes.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int gn_addtonlist(GN_NODELIST_ELEMENT **p_nodelist, GN_NODE *gn);
extern int gn_add_treenode(GN_TREE *gtree, const GN_NODE_ID *gn_nid, const GN_NODE_ID *p_ancestor_id, GN_NODE **gn_return);
extern GN_NODE *gn_find_node(const GN_TREE *gtree, const GN_NODE_ID *gn_nid);
extern void gn_free_node(GN_NODE *gn);
extern void gn_free_nodelist(GN_NODELIST_ELEMENT **p_nodelist);
extern void gn_free_subtree(GN_NODE *gn);
extern char *gn_jf_treespec(GN_NODE *root, long curr_generation);
extern int gn_rmfromnlist(GN_NODELIST_ELEMENT **p_nodelist, GN_NODE *gn);
extern int gn_remove_treenode(GN_TREE *gtree, GN_NODE *gn);
extern void gn_sprint_nodename(char *s, const GN_NODE *gn);
extern int gn_trim_branch(GN_TREE *gtree, GN_NODE *gn);

#ifdef __cplusplus
}
#endif

#include "gnlib.h"

#endif

