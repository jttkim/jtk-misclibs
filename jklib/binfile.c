#include <stdio.h>

#include "jklib.h"


int fwrite_int16array(const short *x, size_t num, FILE *f)
{
#if defined(__atarist__) || defined(__sgi__)
  return (fwrite(x, sizeof(short), num, f));
#else
  size_t i;
  unsigned short s;

  for (i = 0; i < num; i++)
  {
    if (ferror(f))
      break;
    if (x[i] >= 0)
      s = x[i];
    else
    {
      s = -x[i];
      s ^= 0xffff;
      s++;
    }
    putc((s >> 8) & 0xff, f);
    putc(s & 0xff, f);
  }
  return (i);
#endif
}


int fwrite_int32array(const long *x, size_t num, FILE *f)
{
#if defined(__atarist__) || defined(__sgi__)
  return (fwrite(x, sizeof(long), num, f));
#else
  size_t i;
  unsigned long s;

  for (i = 0; i < num; i++)
  {
    if (ferror(f))
      break;
    if (x[i] >= 0)
      s = x[i];
    else
    {
      s = -x[i];
      s ^= 0xffffffff;
      s++;
    }
    putc((s >> 24) & 0xff, f);
    putc((s >> 16) & 0xff, f);
    putc((s >> 8) & 0xff, f);
    putc(s & 0xff, f);
  }
  return (i);
#endif
}


long read_int16(FILE *f)
{
#if defined(__atarist__) || defined(__sgi__)
  short int i16;

  fread(&i16, sizeof(short), 1, f);
  return (i16);

#else
  unsigned char i16[2];

  fread(i16, 2, 1, f);
  if (i16[0] & 0x80)
  {
    i16[0] ^= 0xff;
    i16[1] ^= 0xff;
    return (-i16[0] * 256 - i16[1] - 1);
  }
  else
    return (i16[0] * 256 + i16[1]);
#endif
}


long read_int32(FILE *f)
{
#if defined(__atarist__) || defined(__sgi__)
  long i32;

  fread(&i32, sizeof(long), 1, f);
  return (i32);

#else
  unsigned char i32[4];

  fread(i32, 4, 1, f);
  if (i32[0] & 0x80)
  {
    i32[0] ^= 0xff;
    i32[1] ^= 0xff;
    i32[2] ^= 0xff;
    i32[3] ^= 0xff;
    return (-i32[0] * 16777216 - i32[1] * 65536 - i32[2] * 256 - i32[3] - 1);
  }
  else
    return (+i32[0] * 16777216 + i32[1] * 65536 + i32[2] * 256 + i32[3]);
#endif
}


