#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "genomein.h"


#define free0(p) free(p); (p) = NULL


/*
 * Allocate memory for the g array of the genome and for
 * the usg_count and the bp_count array if the corresponding
 * flags are set.
 * No allocation occurs if requested length is 0.
 * Return: -1: Error
 *          0: ok
 */

int alloc_genome(GENOME *genome, unsigned long length, unsigned long num_genes, unsigned long flags)
{
  unsigned long i;

  genome->flags = flags;
  genome->length = 0;
  genome->num_genes = 0;
  genome->g = NULL;
  genome->usg_count = NULL;
  genome->bp_count = NULL;
  genome->mut_flag = 0;
  genome->num_mutations = 0;
  if (length > 0)
  {
    genome->g = (char *) malloc(length * sizeof(char));
    if (genome->g == NULL)
      return (-1);
    genome->length = length;
  }
  if (num_genes > 0)
  {
    if (genome->flags & GNM_USG)
    {
      genome->usg_count = (long *) malloc(num_genes * sizeof(long));
      if (genome->usg_count == NULL)
      {
	if (genome->g)
	  free0(genome->g);
	genome->length = 0;
	return (-1);
      }
      for (i = 0; i < num_genes; i++)
	genome->usg_count[i] = 0;
    }
    else
      genome->usg_count = NULL;
    if (genome->flags & GNM_BP)
    {
      genome->bp_count = (long *) malloc(num_genes * sizeof(long));
      if (genome->bp_count == NULL)
      {
	if (genome->usg_count)
	  free0(genome->usg_count);
	if (genome->g)
	  free0(genome->g);
	genome->length = 0;
	return (-1);
      }
      for (i = 0; i < num_genes; i++)
	genome->bp_count[i] = 0;
    }
    else
      genome->bp_count = NULL;
    genome->num_genes = num_genes;
  }
  return (0);
}


void free_genome(GENOME *genome)
{
  if (genome->g)
    free0(genome->g);
  if ((genome->flags & GNM_USG) && genome->usg_count)
    free0(genome->usg_count);
  if ((genome->flags & GNM_BP) && genome->bp_count)
    free0(genome->bp_count);
  genome->length = 0;
  genome->num_genes = 0;
}


/*
 * Duplicate source genome into genome. flags indicate which
 * accessory arrays are to be copied if they are present
 * in the source.
 * Return: 0 upon success, -1 otherwise (failures possible
 *   due to memory depletion).
 */

int duplicate_genome(GENOME *genome, const GENOME *source, unsigned long flags)
{
  long i;

  if (alloc_genome(genome, source->length, source->num_genes, source->flags & flags) < 0)
  {
    return (-1);
  }
  memcpy((void *) genome->g, (void *) source->g, genome->length);
  genome->mut_flag = source->mut_flag;
  genome->num_mutations = source->num_mutations;
  if (genome->flags & GNM_USG)
  {
    for (i = 0; i < source->num_genes; i++)
      genome->usg_count[i] = 0;
  }
  if (genome->flags & GNM_BP)
  {
    for (i = 0; i < source->num_genes; i++)
      genome->bp_count[i] = source->bp_count[i];
  }
  return (0);
}


/*
 * Resize the arrays of a genome (g, usg_count and bp_count if present)
 * to a new size. Only the first new_size elements in the genome arrays
 * are retained if the genome is shrinked. If the genome is enlarged,
 * all elements are preserved and the dangling new genes are
 * uninitialized.
 * Return: -1: Error (failed to allocate new genome arrays)
 *          0: success
 */

int resize_genome(GENOME *genome, unsigned long length, unsigned long num_genes)
{
  long *counter;
  unsigned char *g;

  if (genome->length != length)
  {
    if (length == 0)
    {
      free0(genome->g);
      genome->length = 0;
    }
    else
    {
      if (genome->length == 0)
      {
	genome->g = malloc(length * sizeof(char));
	if (genome->g == NULL)
	  return (-1);
      }
      else
      {
	g = realloc(genome->g, length * sizeof(unsigned char));
	if (g == NULL)
	  return (-1);
	genome->g = g;
      }
      genome->length = length;
    }
  }
  if (genome->num_genes != num_genes)
  {
    if (num_genes == 0)
    {
      if ((genome->flags & GNM_USG) && genome->usg_count)
	free0(genome->usg_count);
      if ((genome->flags & GNM_BP) && genome->bp_count)
	free0(genome->bp_count);
      genome->num_genes = 0;
    }
    else
    {
      if (genome->num_genes == 0)
      {
	if (genome->flags & GNM_USG)
	{
	  genome->usg_count = (long *) malloc(num_genes * sizeof(long));
	  if (genome->usg_count == NULL)
	    return (-1);
	}
	if (genome->flags & GNM_BP)
	{
	  genome->bp_count = (long *) malloc(num_genes * sizeof(long));
	  if (genome->bp_count == NULL)
	  {
	    if (genome->usg_count)
	      free0(genome->usg_count);
	    return (-1);
	  }
	}
      }
      else
      {
	if (genome->flags & GNM_USG)
	{
	  counter = (long *) realloc(genome->usg_count, num_genes * sizeof(long));
	  if (counter == NULL)
	  {
	    genome->num_genes = 0;
	    if ((genome->flags & GNM_BP) && genome->bp_count)
	      free0(genome->bp_count);
	    return (-1);
	  }
	  genome->usg_count = counter;
	}
	if (genome->flags & GNM_BP)
	{
	  counter = (long *) realloc(genome->bp_count, num_genes * sizeof(long));
	  if (counter == NULL)
	  {
	    genome->num_genes = 0;
	    if ((genome->flags & GNM_USG) && genome->usg_count)
	      free0(genome->usg_count);
	    return (-1);
	  }
	  genome->bp_count = counter;
	}
      }
      genome->num_genes = num_genes;
    }
  }
  return (0);
}

