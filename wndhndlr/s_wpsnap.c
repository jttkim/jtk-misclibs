#include "wndincl.h"

/*
 * set snapping for positioning of window: Workspace will be
 * aligned by xsnap and ysnap with respect to absolute screen
 * coordinates.
 */

int set_wpos_snap(int w_handle, int xsnap, int ysnap)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  if (xsnap > 0)
    wd->xsnap = xsnap;
  if (ysnap > 0)
    wd->ysnap = ysnap;
  return (0);
}

