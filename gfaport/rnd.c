/* a very hacky implementation of real valued RND(). Don't be
   surprised if results have funny non-randomnesses. */

#include <math.h>
#include <stdlib.h>

double rnd(void)
{
  return(((double) (random() & 0x7fffff)) / ((double) 0x800000));
}

