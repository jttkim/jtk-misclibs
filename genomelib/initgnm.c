#include <stdio.h>

#include "genomein.h"


void init_genome(GENOME *genome)
{
  genome->flags = 0;
  genome->num_genes = 0;
  genome->length = 0;
  genome->g = NULL;
  genome->usg_count = NULL;
  genome->bp_count = NULL;
  genome->mut_flag = 0;
  genome->num_mutations = 0;
}

