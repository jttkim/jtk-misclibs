#include <math.h>

#include "ptlibin.h"


static void lengthtype_distr(const PHYLTREE_NODE *pnode, long type, long *hgram, long hgram_size, double hgram_area)
{
  long i, hpos;

  for (i = 0; i < pnode->num_descendants; i++)
  {
    if (pnode->descendant[i]->edge_info == type)
    {
      hpos = floor(pnode->descendant[i]->length / hgram_area * hgram_size);
      if ((hpos >= 0) && (hpos < hgram_size))
        hgram[hpos]++;
    }
    lengthtype_distr(pnode->descendant[i], type, hgram, hgram_size, hgram_area);
  }
}


int phyl_lengthtype_distr(const PHYLTREE *ptree, long type, long *hgram, long hgram_size, double hgram_area)
{
  long i;

  for (i = 0; i < hgram_size; i++)
    hgram[i] = 0;
  if (ptree->root == NULL)
    return (PHYLERR_NOLEAVES);
  lengthtype_distr(ptree->root, type, hgram, hgram_size, hgram_area);
  return (0);
}

