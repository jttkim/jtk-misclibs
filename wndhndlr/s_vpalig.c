#include "wndincl.h"

/*
 * set the flags indicating whether the virtual plane should be
 * aligned by the corresponding arrow stepsize in the workspace.
 */

int set_vplane_alignment(int w_handle, int vplane_xsnap, int vplane_ysnap)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  if (vplane_xsnap > -1)
  {
    wd->vplane_xsnap = vplane_xsnap != 0;
  }
  if (vplane_ysnap > -1)
  {
    wd->vplane_ysnap = vplane_ysnap != 0;
  }
  return (0);
}

