/* boolean requester. Writes a requester string to stdout and
   reads input from stdin. If the first character of the input
   is 'Y' or 'y', TRUE is returned, else FALSE is returned. */

#include <stdio.h>
#include "config.h"

short input_boolean(const char *txt)
{
  char buf[MAX_SLEN];

  fprintf(stdout, "%s", txt);
  fprintf(stdout, " (y/n)? ");
  fgets(buf, MAX_SLEN, stdin);
  return (((buf[0] == 'Y') || (buf[0] == 'y')));
}


