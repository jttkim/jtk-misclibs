#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"

#include "sim.h"


int main(int argc, char *argv[])
{
  SIMULATION simulation;

  if (argc != 3)
  {
    cerr << "Usage: " << argv[0] << "<infile> <outfile>" << endl;
    exit(EXIT_FAILURE);
  }
  {
    ifstream i(argv[1]);
    ofstream o(argv[2]);

    i >> simulation;
    o << simulation;
  }
  return (EXIT_SUCCESS);
}

