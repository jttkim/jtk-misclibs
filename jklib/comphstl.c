#include "jklib.h"


long compress_histogram_long(long num, const long *d, long *compress)
{
  long p = 0;
  long rp, i;
  long compress_len = 0;

  while(p < num)
  {
    rp = p;
    while (rp < num)
    {
      if (d[rp] != d[p])
      {
        break;
      }
      rp++;
    }
    if ((rp - p) <= 2)
    {
      for (i = p; i < rp; i++)
      {
        compress[compress_len++] = d[i];
      }
    }
    else
    {
      compress[compress_len++] = (long) (p - rp);
      compress[compress_len++] = d[p];
    }
    p = rp;
  }
  return (compress_len);
}

