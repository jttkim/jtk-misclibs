#include <math.h>

#include "jklib.h"


double log2(double x)
{
  double l2 = 1.0 / log(2.0);

  return (log(x) * l2);
}

