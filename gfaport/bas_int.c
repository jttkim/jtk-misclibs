/* bas_int emulates the typical basic INT() function */

#include <math.h>

long bas_int(double x)
{
  double i;

  (void) modf(x, &i);
  return ((long) i);
}

