#include "wndincl.h"

/*
 * modify the size of the virtual plane
 */

int set_vplane_size(int w_handle, long xsize, long ysize)
{
  WINDOW_DESCR *wd;
  int x, y, w, h;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
  if (wind_get(w_handle, WF_WORKXYWH, &x, &y, &w, &h))
  {
    if (xsize >= 0)
    {
      wd->xsize = xsize;
      do_hslider(wd, wd->slider_xpos, w);
    }
    if (ysize >= 0)
    {
      wd->ysize = ysize;
      do_vslider(wd, wd->slider_ypos, h);
    }
    return (0);
  }
  else
  {
    return (-1);
  }
}

