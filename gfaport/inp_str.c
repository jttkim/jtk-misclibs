/* input_str() requests a string from stdin. A null-terminated
   string without any trailing \r's or \n's is written into buf. */

#include <stdio.h>
#include <string.h>

char *input_str(const char *txt, char *buf, unsigned long max_len)
{
  unsigned long i;

  fprintf(stdout, "%s", txt);
  fgets(buf, max_len, stdin);

  for (i = strlen(buf) - 1; (buf[i] == '\r') || (buf[i] == '\n'); i--)
  {
    buf[i] = '\0';
    if (i == 0)
    {
      break;
    }
  }
  return (buf);
}


