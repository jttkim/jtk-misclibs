#include <stdio.h>
#include <string.h>


#include "genomein.h"


void fprint_dnachars(FILE *f, unsigned char d)
{
  int i;
  char dna[5];

  for (i = 0; i < 4; i++)
  {
    switch (d & 0xc0)
    {
    case 0x00:
      dna[i] = 'a';
      break;
    case 0x40:
      dna[i] = 'c';
      break;
    case 0x80:
      dna[i] = 'g';
      break;
    case 0xc0:
      dna[i] = 't';
      break;
    }
    d <<= 2;
  }
  dna[4] = '\0';
  fprintf(f, dna);
}


void fprint_binchars(FILE *f, unsigned char d, const char bchar[2])
{
  int i;
  char b[9];

  for (i = 0; i < 8; i++)
  {
    if ((d & 0x80))
      b[i] = bchar[1];
    else
      b[i] = bchar[0];
    d <<= 1;
  }
  b[8] = '\0';
  fprintf(f, b);
}


void write_pirdna(FILE *f, const GENOME *genome, const char *name, const char *comment)
{
  long i;

  fprintf(f, ">DL;%s\n", name);
  fprintf(f, "%s\n", comment);
  for (i = 0; i < genome->length; i++)
  {
    fprint_dnachars(f, genome->g[i]);
    if ((i % 16) == 15)
      fprintf(f, "\n");
  }
  if ((i % 16) != 0)
    fprintf(f, "\n");
  fprintf(f, "*\n");
}


void write_pirbin(FILE *f, const GENOME *genome, const char *name, const char *comment, const char bchar[2])
{
  long i;

  fprintf(f, ">DL;%s\n", name);
  fprintf(f, "%s\n", comment);
  for (i = 0; i < genome->length; i++)
  {
    fprint_binchars(f, genome->g[i], bchar);
    if ((i % 8) == 7)
      fprintf(f, "\n");
  }
  if ((i % 8) != 0)
    fprintf(f, "\n");
  fprintf(f, "*\n");
}


void write_phylipdna(FILE *f, const GENOME *genome, const char *name, const char *comment)
{
  int i;
  size_t l = strlen(name);

  for (i = 0; i < 10; i++)
  {
    if (i < l)
      fputc(name[i], f);
    else
      fputc(' ', f);
  }
  for (i = 0; i < genome->length; i++)
    fprint_dnachars(f, genome->g[i]);
}

