#include "wndincl.h"

/*
 * set arrow stepsize: A click on a horizontal arrow will cause
 * the virtual plane to be scrolled arrow_xstep pixels, a click
 * on a vertical arrow causes a scroll of arrow_ystep pixels.
 * The virtual plane is aligned by the arrow stepsizes in the
 * workspace if the corresponding flags are set.
 */

int set_arrow_step(int w_handle, int arrow_xstep, int arrow_ystep)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  if (arrow_xstep > 0)
    wd->arrow_xstep = arrow_xstep;
  if (arrow_ystep > 0)
    wd->arrow_ystep = arrow_ystep;
  return (0);
}

