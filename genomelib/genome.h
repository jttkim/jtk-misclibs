#ifndef H_GENOME
#define H_GENOME

#define GNM_USG           1
#define GNM_BP            2

typedef struct
{
  unsigned long  flags;          /* indicators which arrays are used */
  unsigned long  num_genes;      /* number of genes in genome, specifies length of counter arrays */
  unsigned long  length;         /* length of the genome in genes */
  unsigned char *g;              /* the genome itself */
  long          *usg_count;      /* usage counter array */
  long          *bp_count;       /* Bedau & Packard counter array */
  long           mut_flag;       /* flag for mutability */
  long           num_mutations;  /* how often the genome was mutated in */
                                 /* the current time step */
} GENOME;

#endif

