/*
 * Randomly shuffles the num elements of array a
 */

void random_shuffle(long num, long *a)
{
  long i, r, a_r;

  for (i = 0; i < num; i++)
  {
    r = lnd_random(num);
    a_r = a[r];
    a[r] = a[i];
    a[i] = a_r;
  }
}

