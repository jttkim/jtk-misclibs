#include "wndincl.h"

int get_vplane_size(int w_handle, long *xsize, long *ysize)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  *xsize = wd->xsize;
  *ysize = wd->ysize;
  return (0);
}

