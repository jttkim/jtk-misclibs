/* input_long() emulates the basic input command for signed longs */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

signed long input_long(const char *txt)
{
  char buf[MAX_SLEN];

  fprintf(stdout, "%s", txt);
  fgets(buf, MAX_SLEN, stdin);
  return (strtol(buf, (char **) NULL, 10));
}

