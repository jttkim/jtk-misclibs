#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include "gnlib.h"

#include "indiv.h"
#include "sim.h"


INDIVIDUAL::INDIVIDUAL()
{
  // fprintf(stderr, "temporary constructor\n");
  m_simulation = NULL;
  m_genome = 4711;
  // fprintf(stderr, "temporary constructor done\n");
}


INDIVIDUAL::INDIVIDUAL(SIMULATION *simulation)
{
  char buf[256];

  m_genome = simulation->random();
  evaluate_fitness();
  m_simulation = simulation;
  sprintf(buf, "g:%08x, f:%f", m_genome, m_fitness);
  gn_new_treenode(&(m_simulation->gn_tree), NULL, m_simulation->generation, NULL, buf, &m_id);
  /* How can constructor indicate success of construction?? */
  // fprintf(stderr, "spontaneous constructor: individual %s\n", gn_node_idstring(&m_id, buf));
}


INDIVIDUAL::INDIVIDUAL(const INDIVIDUAL *parent)
{
  char buf[256];

  // cerr << "clone constructor: starting" << endl;
  m_genome = parent->m_genome;
  m_simulation = parent->m_simulation;
  mutate();
  evaluate_fitness();
  sprintf(buf, "g:%08x, f:%f", m_genome, m_fitness);
  // cerr << "clone constructor: adding to tree" << endl;
  gn_new_treenode(&(m_simulation->gn_tree), &(parent->m_id), m_simulation->generation, NULL, buf, &m_id);
  // fprintf(stderr, "clone constructor: individual %s\n", gn_node_idstring(&m_id, buf));
}


INDIVIDUAL::~INDIVIDUAL()
{
  int return_code;

  // fprintf(stderr, "destructor: individual %s\n", gn_node_idstring(&m_id, buf));
  if (m_simulation != NULL)
  {
    // cerr << "INDIVIDUAL destructor: removing individual from tree" << endl;
    return_code = gn_node_death(&(m_simulation->gn_tree), &m_id, m_simulation->generation);
    if (return_code)
      cerr << "gn_node_death returned " << return_code << endl;
  }
}


void INDIVIDUAL::evaluate_fitness()
{
  unsigned int g;

  g = m_genome;
  m_fitness = 0;
  while (g)
  {
         g &= g - 1;
         m_fitness++;
  }
}


void INDIVIDUAL::mutate()
{
  unsigned int j;
  unsigned int x;

  x = 1;
  for (j = 0; j < sizeof(unsigned int) * 8; j++)
  {
         if ((m_simulation->random() % 100) == 0)
                m_genome ^= x;
         x <<= 1;
  }
}


ostream &operator << (ostream &s, const INDIVIDUAL &ind)
{
  s << ind.m_genome << endl;
  s << ind.m_id;
  return (s);
}


istream &operator >> (istream &s, INDIVIDUAL &ind)
{
  INDIVIDUAL x;

  s >> x.m_genome;
  // fprintf(stderr, ">> genome: %u\n", x.m_genome);
  s >> x.m_id;
  // cerr << x.m_id << endl;
  x.evaluate_fitness();
  ind = x;
  // cerr << "fitness evaluated:" << ind.m_fitness << " returning" << endl;
  return (s);
}

