#ifndef H_GENOMELIB
#define H_GENOMELIB

#include <stdio.h>

#include "genome.h"

extern void init_genome(GENOME *genome);
extern int alloc_genome(GENOME *genome, unsigned long length, unsigned long num_genes, unsigned long flags);
extern void free_genome(GENOME *genome);
extern int duplicate_genome(GENOME *genome, const GENOME *source, unsigned long flags);
extern int resize_genome(GENOME *genome, unsigned long length, unsigned long num_genes);
extern int read_genome(FILE *f, GENOME *genome, unsigned long flags);
extern int write_genome(FILE *f, const GENOME *genome, unsigned long flags);
extern void fprint_dnachars(FILE *f, unsigned char d);
extern void fprint_binchars(FILE *f, unsigned char d, const char bchar[2]);
extern void write_pirdna(FILE *f, const GENOME *genome, const char *name, const char *comment);
extern void write_pirbin(FILE *f, const GENOME *genome, const char *name, const char *comment, const char bchar[2]);
extern void write_phylipdna(FILE *f, const GENOME *genome, const char *name, const char *comment);

#endif

