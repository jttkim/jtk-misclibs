#include "wndhndlr.h"
#include "wndincl.h"

/*
 * process WM_HSLID message
 */

void w_hslid(int w_handle, int pos)
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
    do_hslider(wd, ((wd->xsize - w) * pos) / 1000, w);
    w_redraw_all(w_handle);
  }
}

