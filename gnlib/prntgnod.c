#include <stdio.h>

#include "gnlibin.h"


int gn_print_node(const GN_NODE *gn)
{
  long i;
  char buf[256];

  gn_sprint_nodename(buf, gn);
  printf("node %s: ", buf);
  if (gn->lifespan == GN_STILL_ALIVE)
    printf("living\n");
  else
    printf("lifespan=%ld\n", gn->lifespan);
  printf("  %ld offspring nodes, %ld living descendants\n", gn->num_offspring, gn->num_alive);
  if (gn->ancestor)
  {
    gn_sprint_nodename(buf, gn->ancestor);
    printf("  ancestor: %s\n", buf);
  }
  else
    printf("  ancestor: ** unknown **\n");
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
    {
      gn_sprint_nodename(buf, gn->offspring[i]);
      printf("  offspring %6ld: %s\n", i, buf);
    }
  }
  return (0);
}

