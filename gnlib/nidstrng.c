#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


/*
 * Generate a unique string corresponding to a node ID
 * in buf.
 * If a NULL pointer is passed in the buf argument, the function
 * tries to allocate memory for the string. In this case, the
 * caller must free() buf after use.
 * Return: NULL if allocation was requested and failed, buf
 *   otherwise.
 */

char *gn_node_idstring(const GN_NODE_ID *id, char *buf)
{
  if (buf == NULL)
  {
    if ((buf = (char *) malloc(32)) == NULL)
      return (NULL);
  }
  if (id->generation == -1)
    sprintf(buf, "root-%ld", id->id);
  else
    sprintf(buf, "g%ld-%ld", id->generation, id->id);
  return (buf);
}

