/*
 * gnexample.c
 *
 * A simple GA in which gnlib is used to keep track of genealogy
 */

#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"

#define POPSIZE 10


typedef struct
{
  unsigned int genome;
  double fitness;
  GN_NODE_ID id;
} INDIVIDUAL;


void init_population(INDIVIDUAL pop[POPSIZE], GN_TREE *gn_tree)
{
  int i;

  for (i = 0; i < POPSIZE; i++)
  {
    pop[i].genome = rand();
    gn_new_treenode(gn_tree, NULL, 0, NULL, &(pop[i].id));
  }
}


void fitness(INDIVIDUAL pop[POPSIZE])
{
  int f, i;
  unsigned int g;

  for (i = 0; i < POPSIZE; i++)
  {
    g = pop[i].genome;
    f = 0;
    while (g)
    {
      g &= g - 1;
      f++;
    }
    pop[i].fitness = f;
  }
}


void selection(INDIVIDUAL pop[POPSIZE], GN_TREE *gn_tree, int generation)
{
  int i_high, i_low, i;

  i_high = 0;
  i_low = POPSIZE - 1;
  for (i = 0; i < POPSIZE; i++)
  {
    if (pop[i].fitness < pop[i_low].fitness)
      i_low = i;
    if (pop[i].fitness > pop[i_high].fitness)
      i_high = i;
  }
  gn_node_death(gn_tree, &(pop[i_low].id), generation);
  pop[i_low].genome = pop[i_high].genome;
  gn_new_treenode(gn_tree, &(pop[i_high].id), generation, NULL, &(pop[i_low].id));
}


void mutation(INDIVIDUAL pop[POPSIZE])
{
  int i, j;
  unsigned int x;

  for (i = 0; i < POPSIZE; i++)
  {
    x = 1;
    for (j = 0; j < sizeof(unsigned int) * 8; j++)
    {
      if ((rand() % 100) == 0)
	pop[i].genome ^= x;
      x <<= 1;
    }
  }
}


int main(int argc, char *argv[])
{
  INDIVIDUAL pop[POPSIZE];
  GN_TREE gn_tree;
  int generation;

  gn_init_tree(&gn_tree);
  init_population(pop, &gn_tree);
  for (generation = 0; generation < 100; generation++)
  {
    if ((generation % 30) == 0)
    {
      fprintf(stdout, "g %d\n", generation);
      gn_print_jftrees(&gn_tree, generation, stdout);
    }
    fitness(pop);
    selection(pop, &gn_tree, generation);
    mutation(pop);
  }
  gn_free_tree(&gn_tree);
  return (EXIT_SUCCESS);
}

