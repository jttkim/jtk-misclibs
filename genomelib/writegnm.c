#include <stdio.h>

#include "genomein.h"


/*
 * Write a genome in hex representation out to file f. flags
 * is used to control which additional arrays should be
 * written. write_genome makes sure not to try to write
 * nonexistent arrays itself.
 * Return: Always 0 (success), no checking for file errors.
 */

int write_genome(FILE *f, const GENOME *genome, unsigned long flags)
{
  long j;

  flags &= genome->flags;
  fprintf(f, "%lu\n", flags);
  fprintf(f, "%lu\n", genome->length);
  fprintf(f, "%lu\n", genome->num_genes);
  for (j = 0; j < genome->length; j++)
  {
    fprintf(f, "%02x", genome->g[j]);
  }
  fprintf(f, "\n");
  fprintf(f, "%ld\n", genome->mut_flag);
  fprintf(f, "%ld\n", genome->num_mutations);
  if (flags & GNM_USG)
  {
    for (j = 0; j < genome->num_genes; j++)
    {
      fprintf(f, "%ld\n", genome->usg_count[j]);
    }
  }
  if (flags & GNM_BP)
  {
    for (j = 0; j < genome->num_genes; j++)
    {
      fprintf(f, "%ld\n", genome->bp_count[j]);
    }
  }
  return (0);
}

