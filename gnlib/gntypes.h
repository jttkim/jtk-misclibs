#ifndef H_GNTYPES
#define H_GNTYPES


#ifdef __cplusplus
extern "C" {
#endif

#include "gndefs.h"

typedef struct
{
  long generation;
  long id;
} GN_NODE_ID;


typedef struct tag_gn_node
{
  GN_NODE_ID           node_id;
  long                 branch_generation; /* time of split from ancestor */
  long                 lifespan;
  long                 num_alive;         /* number of living descendants */
  struct tag_gn_node  *ancestor;          /* pointer to ancestor node */
  long                 num_offspring;     /* number of offspring nodes */
  long                 offspring_arrsize; /* size of offspring array */
  struct tag_gn_node **offspring;         /* array of pointers to offspring nodes */
  char                 name[GN_MAX_NODENAME_LENGTH + 1];
  char                *genome;            /* a string holding the genome at time of gn_new_treenode call */
  double               xpos, ypos, val;
} GN_NODE;


typedef struct tag_nodelist_element
{
  struct tag_nodelist_element *next;
  GN_NODE                     *gnode;
} GN_NODELIST_ELEMENT;


typedef struct
{
  long         size;
  long         generation;
  long         num_used;
  GN_NODE_ID  *node;
  long       **m;
} GN_DISTANCE_MATRIX;


typedef struct
{
  int                   pruning_strategy;
  long                  num_nodes;  /* number of nodes */
  long                  num_alive;  /* number of alive nodes */
  GN_NODE_ID            last_id;    /* last node id generated */
  GN_NODELIST_ELEMENT  *root_list;  /* list of root nodes */
  GN_NODELIST_ELEMENT  *alive_list; /* list of alive nodes */
} GN_TREE;

#ifdef __cplusplus
}
#endif


#endif

