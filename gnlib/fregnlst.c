#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Free (discard) a list of nodes.
 */

void gn_free_nodelist(GN_NODELIST_ELEMENT **p_nodelist)
{
  GN_NODELIST_ELEMENT *a;

  while (*p_nodelist)
  {
    a = *p_nodelist;
    *p_nodelist = a->next;
    free(a);
  }
}

