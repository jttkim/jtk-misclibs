#include <stdio.h>

#include "jklib.h"


char *char_to_hex(char *buf, const char *source, long n)
{
  long i;
  char *p = buf;

  if (n)
  {
    for (i = 0; i < n; i++)
    {
      sprintf(p, "%02x", (unsigned char) source[i]);
      p += 2;
    }
  }
  else
  {
    *buf = '\0';
  }
  return (buf);
}


