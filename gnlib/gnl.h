extern int gn_free_tree(GN_TREE *gtree);
extern long gn_distance(const GN_TREE *gtree, GN_NODE *p1, GN_NODE *p2, long g);
extern void gn_init_tree(GN_TREE *gtree);
extern int gn_new_treenode(GN_TREE *gtree, const GN_NODE_ID *p_ancestor_id, long generation, const char *name, GN_NODE_ID *new_nid);
extern int gn_node_death(GN_TREE *gtree, const GN_NODE_ID *gn_nid, long death_generation);
extern int gn_print_node(const GN_NODE *gn);
extern int gn_print_subtree(const GN_NODE *gn);
extern int gn_print_tree(const GN_TREE *gtree);
extern int gn_print_jftrees(const GN_TREE *gtree, long generation, FILE *f);
extern int gn_read_id(GN_NODE_ID *gn_nid, FILE *f);
extern int gn_read_tree(GN_TREE *gtree, FILE *f);
extern int gn_save_id(const GN_NODE_ID *gn_nid, FILE *f);
extern int gn_save_tree(const GN_TREE *gtree, FILE *f);

