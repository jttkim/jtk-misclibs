#ifndef SIM_H
#define SIM_H

#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"

#include "indiv.h"

#define POPSIZE 10


class SIMULATION
{
private:
  INDIVIDUAL *pop[POPSIZE];
  unsigned long random_state;
  ofstream pro_stream;

public:
  GN_TREE gn_tree;
  int generation;

  SIMULATION();
  ~SIMULATION();
  void fitness();
  void selection();
  void mutation();
  void write_pro_file();
  void output();
  unsigned long random()
  {
    if (random_state <= 0)
    { 
      random_state <<= 1;
      random_state ^= 0x1d872b41L;
    }
    else
      random_state <<= 1;
    return (random_state);
  }

  friend ostream &operator << (ostream &s, const SIMULATION &sim);
  friend istream &operator >> (istream &s, SIMULATION &sim);
};

#endif /* SIM_H */

