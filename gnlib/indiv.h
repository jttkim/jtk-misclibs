#ifndef INDIV_H
#define INDIV_H

#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"


class SIMULATION;

class INDIVIDUAL
{
private:
  unsigned int m_genome;
  double m_fitness;
  GN_NODE_ID m_id;

public:
  SIMULATION *m_simulation;
  INDIVIDUAL();
  INDIVIDUAL(SIMULATION *simulation);
  INDIVIDUAL(const INDIVIDUAL *parent);
  ~INDIVIDUAL();
  double fitness() const { return (m_fitness); }
  void evaluate_fitness();
  void mutate();
  friend ostream &operator << (ostream &s, const INDIVIDUAL &ind);
  friend istream &operator >> (istream &s, INDIVIDUAL &ind);
};

#endif /* INDIV_H */

