/* input_ul() emulates the basic input command for unsigned longs */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

unsigned long input_ul(const char *txt)
{
  char buf[MAX_SLEN];

  fprintf(stdout, "%s", txt);
  fgets(buf, MAX_SLEN, stdin);
  return (atol(buf));
}


