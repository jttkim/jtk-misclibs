#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jklib.h"


LINEFILE *l_fopen(const char *name)
{
  LINEFILE *lf;

  if ((lf = (LINEFILE *) malloc(sizeof(LINEFILE))) == NULL)
    return (NULL);
  if ((lf->f = fopen(name, "r")) == NULL)
  {
    free(lf);
    return (NULL);
  }
  strncpy(lf->name, name, FILENAME_MAX);
  lf->line = 0;
  return (lf);
}


int l_fclose(LINEFILE *lf)
{
  int ret_code;

  if (lf)
  {
    ret_code = fclose(lf->f);
    free(lf);
    return (ret_code);
  }
  return (EOF);
}


char *l_fgets(char *s, size_t l, LINEFILE *lf)
{
  char *g;
  size_t i;

  if (s == NULL)
  {
    fprintf(stderr, "Error: l_fgets() cannot read into string pointed to by NULL\n");
    return (NULL);
  }
  g = fgets(s, l, lf->f);
  i = strlen(s);
  if (i && (s[i - 1] == '\n'))
    lf->line++;
  return (s);
}


char *l_fgets0(char *s, size_t l, LINEFILE *lf)
{
  char *g;
  size_t i;

  if (s == NULL)
  {
    fprintf(stderr, "Error: l_fgets() cannot read into string pointed to by NULL\n");
    return (NULL);
  }
  g = fgets(s, l, lf->f);
  i = strlen(s);
  if (i && (s[i - 1] == '\n'))
  {
    lf->line++;
    s[i - 1] = '\0';
  }
  return (g);
}


void l_errmsg(LINEFILE *lf, const char *msg)
{
  fprintf(stderr, "File \"%s\", line %ld: %s\n", lf->name, lf->line, msg);
}


void l_rewind(LINEFILE *lf)
{
  rewind(lf->f);
  lf->line = 0;
}


FILE *l_fpointer(LINEFILE *lf)
{
  return (lf->f);
}

