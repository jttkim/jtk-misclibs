#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Remove an entry from a nodelist.
 * Note: If the element in question is the head of the list, the pointer
 * to the list is modified. Therefore, all pointers to the list other
 * than the one passed to this function must be updated after a call to
 * gn_rmfromnlist.
 */

int gn_rmfromnlist(GN_NODELIST_ELEMENT **p_nodelist, GN_NODE *gn)
{
  GN_NODELIST_ELEMENT *a, *prev_a;

  a = *p_nodelist;
  if (a->gnode == gn)
  {
    *p_nodelist = a->next;
    free(a);
    return (0);
  }
  else
  {
    prev_a = a;
    a = a->next;
    while (a)
    {
      if (a->gnode == gn)
      {
        prev_a->next = a->next;
        free(a);
        return (0);
      }
      prev_a = a;
      a = a->next;
    }
  }
  return (GNERR_UNKNODE);
}

