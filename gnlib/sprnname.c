#include <stdio.h>

#include "gnlibin.h"


void gn_sprint_nodename(char *s, const GN_NODE *gn)
{
  if (gn->name[0])
    sprintf(s, "%s", gn->name);
  else
    gn_node_idstring(&(gn->node_id), s);
}

