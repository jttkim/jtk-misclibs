#include "wndincl.h"

/*
 * set_hslider() and set_vslider() are wrapper functions for do_hslider()
 * and do_vslider(). The only thing they do themselves is determining the
 * window descriptor (to be passed to the do_* functions) from the window
 * handle passed to them
 */

int set_hslider(int w_handle, long pos, int w_height)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  do_hslider(wd, pos, w_height);
  return (0);
}

