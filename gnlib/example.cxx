/*
 * example.cxx
 *
 * A simple GA in which gnlib is used to keep track of genealogy
 * Adapted from gnexample.c to c++
 */

#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "sim.h"


int main(int argc, char *argv[])
{
  SIMULATION simulation;
  int ng;

  if (argc < 2)
  {
    ng = 100;
    cerr << "New simulation, 100 generations" << endl;
  }
  else if (argc == 2)
  {
    ng = strtol(argv[1], NULL, 10);
    cerr << "New simulation, " << ng << " generations" << endl;
  }
  else
  {
    ifstream s(argv[2]);

    ng = strtol(argv[1], NULL, 10);
    s >> simulation;
    cerr << "Continuation of \"" << argv[2] << "\", adding " << ng << " generations" << endl;
  }
  ng += simulation.generation;
  while (simulation.generation < ng)
  {
    // cerr << "processing generation " << simulation.generation << endl;
    simulation.fitness();
    // cerr << "output " << simulation.generation << endl;
    simulation.output();
    // cerr << "selection " << simulation.generation << endl;
    simulation.selection();
    // cerr << "mutation " << simulation.generation << endl;
    // simulation.mutation();
    // cerr << "generation " << simulation.generation << " done" << endl;
    simulation.generation++;
  }
  cerr << "*** EXIT FROM LOOP AT GENERATION " << simulation.generation << endl;
  // cerr << "*** TARGET GENERATION            " << ng << endl;
  {
    ofstream s("example.dat");

    s << simulation;
}

#ifdef WAIT_END
  {
    char buf[10];

    cerr << "Run complete, hit return to terminate" << endl;
    cin.get(buf, 10, '\n');
  }
#endif

  return (EXIT_SUCCESS);
}

