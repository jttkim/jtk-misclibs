#include <stdio.h>
#include <stdlib.h>

#include "gnlibin.h"


int gn_read_id(GN_NODE_ID *gn_nid, FILE *f)
{
  char buf[GN_MAX_LINELENGTH], *cp, *cp1;

  if (fgets(buf, 80, f) == NULL)
    return (GNERR_FILE);
  if (buf[0] == '!')
    return (GN_UNSPEC_ID);
  gn_nid->generation = strtol(buf, &cp, 10);
  if (cp == buf)
    return (GNERR_PARSE);
  if (fgets(buf, 80, f) == NULL)
    return (GNERR_FILE);
  if (buf[0] == '!')
    return (GN_UNSPEC_ID);
  gn_nid->id = strtol(buf, &cp, 10);
  if (cp == buf)
    return (GNERR_PARSE);
#ifdef GNLIB_DEBUG
  fprintf(stderr, "gn_read_id: returning (%ld, %ld)\n", gn_nid->generation, gn_nid->id);
#endif
  return (0);
}
