#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Add an element to a list of nodes. The new element is put at the
 * head of the list, and therefore the p_nodelist, the pointer to the
 * list, is modified.
 * Note: If there are pointers to the list being modified in addition
 * to the one passed to gn_addtonlist, they must be updated
 * after a call to this function.
 */

int gn_addtonlist(GN_NODELIST_ELEMENT **p_nodelist, GN_NODE *gn)
{
  GN_NODELIST_ELEMENT *a;

  if ((a = (GN_NODELIST_ELEMENT *) malloc(sizeof(GN_NODELIST_ELEMENT))) == NULL)
    return (GNERR_MEM);
  a->next = *p_nodelist;
  a->gnode = gn;
  *p_nodelist = a;
  return (0);
}

