/* input_dbl() emulates the basic input command for doubles */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

double input_dbl(const char *txt)
{
  char buf[MAX_SLEN];

  fprintf(stdout, "%s", txt);
  fgets(buf, MAX_SLEN, stdin);
  return (strtod(buf, (char **) NULL));
}


