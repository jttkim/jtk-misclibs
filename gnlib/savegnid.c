#include <stdio.h>

#include "gnlibin.h"


int gn_save_id(const GN_NODE_ID *gn_nid, FILE *f)
{
  fprintf(f, "%ld\n", gn_nid->generation);
  if (fprintf(f, "%ld\n", gn_nid->id) == EOF)
    return (GNERR_FILE);
  return (0);
}

