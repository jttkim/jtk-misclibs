#include <stdio.h>

#include "jklib.h"


int main(int argc, char **argv)
{
  unsigned long x;

  for (x = 0; x < 65536; x++)
    printf("%lu %d\n", x, num_setbits(x));
  return (0);
}

