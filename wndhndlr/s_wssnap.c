#include "wndincl.h"

/*
 * set snapping for window size: Window width will always be
 * a multiple of wsnap, height will always be a multiple of
 * hsnap.
 */

int set_wsize_snap(int w_handle, int wsnap, int hsnap)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  if (wsnap > 0)
    wd->wsnap = wsnap;
  if (hsnap > 0)
    wd->hsnap = hsnap;
  return (0);
}

