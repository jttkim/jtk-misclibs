#include "jklib.h"


long hex_to_char(char *buf, const char *source)
{
  long i;
  int h1, h2;

  for (i = 0; ; i++)
  {
    if ((h1 = hexval(*source)) < 0)
      break;
    source++;
    if ((h2 = hexval(*source)) < 0)
      break;
    source++;
    buf[i] = (h1 << 4) | h2;
  }
  return (i);
}


