#include "wndhndlr.h"
#include "wndincl.h"

/*
 * process WM_VSLID message
 */

void w_vslid(int w_handle, int pos)
{
  WINDOW_DESCR *wd;
  int x, y, w, h;

  wd = get_window_descr(w_handle);
  if (wd != NULL)
  {
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
    wind_get(w_handle, WF_WORKXYWH, &x, &y, &w, &h);
    do_vslider(wd, ((wd->ysize - h) * pos) / 1000, h);
    w_redraw_all(w_handle);
  }
}

