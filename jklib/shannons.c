#include <errno.h>
#include <math.h>
#include <stdio.h>

#include "jklib.h"


double shannon_short(int l, const short *h)
{
  auto long sum = 0;
  int  i;
  double p, q;
  double s = 0;

  for (i = 0; i < l; i++)
    sum += h[i];
  for (i = 0;i < l; i++)
  {
    if (h[i] > 0)
    {
      p = ((double) h[i]) / ((double) sum);
      errno = 0;
      q =  -p * log(p);
      if (errno)
        fprintf(stderr, "In function shannon_short: error computing log(%f)\n", p);
      else
	s += q;
    }
  }
  return (s);
}

