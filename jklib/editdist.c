#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "jklib.h"


long max3(long p1, long p2, long p3)
{
  p1 = (p1 > p2) ? p1 : p2;
  return((p1 > p3) ? p1 : p3);
}

long edit_distance(long xl, const char *x, long yl, const char *y)
{
  long i;
  long xp = xl - 1;
  long yp = yl - 1;
  long m = LONG_MIN;
  long **sm;

  if (xl == 0)
    return (yl);
  if (yl == 0)
    return (xl);
  sm = (long **) malloc(xl * sizeof(long *));
  if (sm == NULL)
    return (-1);
  sm[0] = (long *) malloc(xl * yl * sizeof(long));
  if (sm[0] == NULL)
  {
    free((void *) sm);
    return (-1);
  }
  for (i = 1; i < xl; i++)
    sm[i] = sm[0] + i * yl;
  for (i = 0; i < xl; i++)
    sm[i][yp] = i - xp;
  for (i = 0; i < yp; i++)
    sm[xp][i] = i - yp;
  /* printf("score matrix edges initialized\n"); */
  /* printf("starting to process score matrix\n"); */
  if (yp > 0)
  {
    while (xp > 0)
    {
      xp--;
      yp--;
      /* printf("processing %ld / %ld\n", xp, yp); */
      for(i = xp; ; i--)
      {
        sm[i + 1][yp + 1] -= (x[i + 1] != y[yp + 1]);
        sm[i][yp] = max3(sm[i][yp + 1] - 1, sm[i + 1][yp] - 1, sm[i + 1][yp + 1]);
        /* printf("%ld/%ld: %ld\n", i, yp, sm[i][yp]); */
        if (i == 0)
          break;
      }
      /* printf("half done\n"); */
      if (yp == 0)
        break;
      for (i = yp - 1; ; i--)
      {
        sm[xp + 1][i + 1] -= (x[xp + 1] != y[i + 1]);
        sm[xp][i] = max3(sm[xp][i + 1] - 1, sm[xp + 1][i] - 1, sm[xp + 1][i + 1]);
        /* printf("%ld/%ld: %ld\n", xp, i, sm[xp][i]); */
        if (i == 0)
          break;
      }
      /* printf(" ... done\n"); */
    }
  }
  /* printf("score matrix worked through\n"); */
  for (i = 0; i < xl; i++)
  {
    sm[i][0] -= (i + (x[i] != y[0]));
    m = (sm[i][0] > m) ? sm[i][0] : m;
  }
  for (i = 1; i < yl; i++)
  {
    sm[0][i] -= (i + (x[0] != y[i]));
    m = (sm[0][i] > m) ? sm[0][i] : m;
  }
  free((void *) sm[0]);
  free((void *) sm);
  /* printf("returning edit distance %ld\n", -m); */
  return (-m);
}

