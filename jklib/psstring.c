#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jklib.h"


/*
 * Convert an arbitrary string into a PostScript string.
 * This means that parentheses are placed around the string
 * and that special characters are replaced with their
 * PostScript escapes.
 * If a NULL pointer is passed as a target, ps_string allocates
 * memory for the string. In that case, the target must be
 * free()d after use.
 * Return: Pointer to the converted string, NULL if target could
 *   not be allocated (possible only when allocation was requested
 *   by the caller by passing a NULL pointer).
 * Note: When the caller supplies a target string, it is also the
 *   callers responsibility to make sure the target string is long
 *   enough. In worst case, a length of 2 * (source length) + 3
 *   is necessary for the target string.
 */

char *ps_string(const char *s, char *buf)
{
  size_t i, bpos = 0;
  const size_t l = strlen(s);

  if (buf == NULL)
  {
    if ((buf = (char *) malloc((2 * l + 3) * sizeof(char))) == NULL)
      return (NULL);
  }
  buf[bpos++] = '(';
  for (i = 0; i < l; i++)
  {
    switch (s[i])
    {
    case '(': case ')': case '\\':
      buf[bpos++] = '\\';
      buf[bpos++] = s[i];
      break;
    case '\b':
      buf[bpos++] = '\\';
      buf[bpos++] = 'b';
      break;
    case '\f':
      buf[bpos++] = '\\';
      buf[bpos++] = 'f';
      break;
    case '\n':
      buf[bpos++] = '\\';
      buf[bpos++] = 'n';
      break;
    case '\r':
      buf[bpos++] = '\\';
      buf[bpos++] = 'r';
      break;
    case '\t':
      buf[bpos++] = '\\';
      buf[bpos++] = 't';
      break;
    default:
      buf[bpos++] = s[i];
      break;
    }
  }
  buf[bpos++] = ')';
  buf[bpos] = '\0';
  return (buf);
}

