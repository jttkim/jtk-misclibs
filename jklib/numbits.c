#include "jklib.h"


int num_setbits(unsigned long x)
{
  int n = 0;

  while (x)
  {
    x &= x - 1;
    n++;
  }
  return (n);
}

