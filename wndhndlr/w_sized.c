#include "wndincl.h"

/*
 * process WM_SIZED message
 */

void w_sized(int w_handle, int x, int y, int w, int h)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd != NULL)
  {
#ifdef WND_DEBUG
    wnd_report("wind_calc");
#endif
    wind_calc(WC_WORK, wd->flags, x, y, w, h, &x, &y, &w, &h);
    w = (w < wd->xsize) ? w : wd->xsize;
    h = (h < wd->ysize) ? h : wd->ysize;
    align_window(wd, x, y, w, h);
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
    wind_get(w_handle, WF_WORKXYWH, &x, &y, &w, &h);
    do_hslider(wd, wd->slider_xpos, w);
    do_vslider(wd, wd->slider_ypos, h);
#ifdef WND_DEBUG
    sprintf(wdbg_tmpbuf, "sized window %1d", wd->handle);
    wnd_report(wdbg_tmpbuf);
#endif
  }
}

