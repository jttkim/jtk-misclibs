#include <float.h>
#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"

#include "sim.h"


SIMULATION::SIMULATION()
{
  int i;

  generation = -1;
  random_state = 4711;
  gn_init_tree(&gn_tree);
  gn_tree.pruning_strategy = GN_PRUNE_DEAD;
  for (i = 0; i < POPSIZE; i++)
         pop[i] = new INDIVIDUAL(this);
  pro_stream.open("example.pro");
  pro_stream << "example test" << endl;
  pro_stream << "@" << endl;
  // info vars here
  pro_stream << "@" << endl;
  pro_stream << "n" << endl;
  pro_stream << "generation" << endl;
  pro_stream << "n" << endl;
  pro_stream << "min_fitness" << endl;
  pro_stream << "n" << endl;
  pro_stream << "max_fitness" << endl;
  pro_stream << "n" << endl;
  pro_stream << "mean_fitness" << endl;
  pro_stream << "n" << endl;
  pro_stream << "PhyloTreeLength" << endl;
  pro_stream << "n" << endl;
  pro_stream << "PhyloTreeHeight" << endl;
  pro_stream << "n" << endl;
  pro_stream << "PhyloTreeDDC" << endl;
  pro_stream << "d i5" << endl;
  pro_stream << "PhyloTreeDDistribution" << endl;
  pro_stream << "@" << endl;
  generation++;
}


SIMULATION::~SIMULATION()
{
  int i;

  for (i = 0; i < POPSIZE; i++)
         delete pop[i];
  gn_free_tree(&gn_tree);
  fprintf(stderr, "* * *  G A M E   O V E R  * * *\n");
  pro_stream.close();
}


void SIMULATION::fitness()
{
  int i;

  for (i = 0; i < POPSIZE; i++)
         pop[i]->evaluate_fitness();
}


void SIMULATION::selection()
{
  int i_high, i_low, i;

  i_high = 0;
  i_low = POPSIZE - 1;
  for (i = 0; i < POPSIZE; i++)
  {
         if (pop[i]->fitness() < pop[i_low]->fitness())
                i_low = i;
         if (pop[i]->fitness() > pop[i_high]->fitness())
                i_high = i;
  }
  // cerr << "selection: hi=" << i_high <<", lo=" << i_low << endl;
  delete pop[i_low];
  // cerr << "selection: deleted low" << endl;
  pop[i_low] = new INDIVIDUAL(pop[i_high]);
  // cerr << "selection: duplicatated high" << endl;
}


void SIMULATION::mutation()
{
  int i;

  for (i = 0; i < POPSIZE; i++)
         pop[i]->mutate();
}


void SIMULATION::write_pro_file()
{
  double min_fitness, max_fitness, mean_fitness, f;
  int i;

  min_fitness = DBL_MAX;
  max_fitness = -min_fitness;
  mean_fitness = 0.0;
  for (i = 0; i < POPSIZE; i++)
  {
    f = pop[i]->fitness();
    if (min_fitness > f)
      min_fitness = f;
    if (max_fitness < f)
      max_fitness = f;
    mean_fitness += f;
  }
  mean_fitness /= POPSIZE;
  pro_stream << generation << endl;
  pro_stream << min_fitness << endl;
  pro_stream << max_fitness << endl;
  pro_stream << mean_fitness << endl;
  pro_stream << gn_tree_length(&gn_tree, generation) << endl;
  pro_stream << gn_tree_height(&gn_tree, generation) << endl;
  pro_stream << gn_ddc(&gn_tree, generation) << endl;
  if ((generation % 5) == 0)
  {
    pro_stream << "# distance distribution, g = " << generation << endl;
    gn_ostream_ddistribution_rle(&gn_tree, generation, pro_stream);
    pro_stream << "# end distance distribution" << endl;
  }
}


void SIMULATION::output()
{
  if ((generation % 1) == 0)
  {
	 // printf("g %d\n", generation);
    cout << "g " << generation << endl;
    gn_ostream_jftrees(&gn_tree, generation, cout);
  }
  cout << "L: " << generation << " " << gn_tree_length(&gn_tree, generation) << endl;
  cout << "H: " << generation << " " << gn_tree_height(&gn_tree, generation) << endl;
  cout << "C: " << generation << " " << gn_ddc(&gn_tree, generation) << endl;
  write_pro_file();
}


ostream &operator << (ostream &s, const SIMULATION &sim)
{
  int i;

  s << sim.generation << endl;
  s << sim.random_state << endl;
  for (i = 0; i < POPSIZE; i++)
         s << *sim.pop[i];
  s << sim.gn_tree;
  // gn_print_tree(&sim.gn_tree);
  return (s);
}

istream &operator >> (istream &s, SIMULATION &sim)
{
  INDIVIDUAL tmp;
  int i;

  gn_free_tree(&sim.gn_tree);
  s >> sim.generation;
  s >> sim.random_state;
  cerr << "last generation of saved state is: " << sim.generation << endl;
  for (i = 0; i < POPSIZE; i++)
  {
    // cerr << "reading individual " << i << endl;
    s >> tmp;
    // cerr << "----- Individual " << i << " ---------------"<< endl;
    // cerr << tmp;
    // cerr << "-----------------------------------" << endl;
    *sim.pop[i] = tmp;
    sim.pop[i]->m_simulation = &sim;
    // cerr << "new individual " << i << endl;
    // cerr << *sim.pop[i];
  }
  s >> sim.gn_tree;
  cerr << "SIMULATION >> success" << endl;
  return (s);
}

