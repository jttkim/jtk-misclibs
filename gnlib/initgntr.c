#include <limits.h>

#include "gnlibin.h"


void gn_init_tree(GN_TREE *gtree)
{
  gtree->pruning_strategy = GN_PRUNE_MONOLINKS;
  gtree->num_nodes = 0;
  gtree->num_alive = 0;
  gtree->last_id.generation = LONG_MIN;
  gtree->last_id.id = 0;
  gtree->root_list = NULL;
  gtree->alive_list = NULL;
}

