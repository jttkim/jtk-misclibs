#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "genomein.h"
#include "jklib.h"

/*
 * Read a genome from a file, including accessory arrays.
 * The flags argument controls which accessory arrays should
 * be read, however, they are only read when they are
 * stored in the file. Arrays that are stored but not
 * desired are skipped.
 * Return: 0 upon success, -1 otherwise.
 */

int read_genome(FILE *f, GENOME *genome, unsigned long flags)
{
  char buf[MAX_SLEN];
  long j;
  unsigned long stored_flags;
  int ch, cl;

  fgets(buf, MAX_SLEN, f);
  stored_flags = strtoul(buf, (char **) NULL, 10);
  genome->flags = stored_flags & flags;
  fgets(buf, MAX_SLEN, f);
  genome->length = strtoul(buf, (char **) NULL, 10);
  fgets(buf, MAX_SLEN, f);
  genome->num_genes = strtoul(buf, (char **) NULL, 10);
  if (alloc_genome(genome, genome->length, genome->num_genes, genome->flags) == -1)
    return (-1);
  for (j = 0; !iscntrl(ch = fgetc(f)); j++)
  {
    cl = fgetc(f);
    if ((ch == EOF) || (cl == EOF) || (iscntrl(cl)))
    {
      break;
    }
    genome->g[j] = (hexval(ch) << 4) | hexval(cl);
  }
  if (j != genome->length)
  {
    fprintf(stderr, "genomelib: read_genome: Warning: Expected %lu chars, got %ld chars\n", genome->length, j);
  }
  fgets(buf, MAX_SLEN, f);
  genome->mut_flag = strtol(buf, (char **) NULL, 10);
  fgets(buf, MAX_SLEN, f);
  genome->num_mutations = strtol(buf, (char **) NULL, 10);
  if (genome->flags & GNM_USG)
  {
    for (j = 0; j < genome->num_genes; j++)
    {
      fgets(buf, MAX_SLEN, f);
      genome->usg_count[j] = strtol(buf, (char **) NULL, 10);
    }
  }
  else if (stored_flags & GNM_USG)
  {
    for (j = 0; j < genome->num_genes; j++)
      fgets(buf, MAX_SLEN, f);
  }
  if (genome->flags & GNM_BP)
  {
    for (j = 0; j < genome->num_genes; j++)
    {
      fgets(buf, MAX_SLEN, f);
      genome->bp_count[j] = strtol(buf, (char **) NULL, 10);
    }
  }
  else if (stored_flags & GNM_BP)
  {
    for (j = 0; j < genome->num_genes; j++)
      fgets(buf, MAX_SLEN, f);
  }
  return (0);
}

