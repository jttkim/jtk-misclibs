#include "wndincl.h"

/*
 * process WM_FULLED message. The full size is either the size of the
 * background window or the size that allows accomodation  of the entire
 * virtual plane, whichever is smaller.
 */

void w_fulled(int w_handle)
{
  WINDOW_DESCR *wd;
  int x, y, w, h, x0, y0, w0, h0;

  wd = get_window_descr(w_handle);
  if (wd != NULL)
  {
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
    wind_get(w_handle, WF_WORKXYWH, &x0, &y0, &w0, &h0);
#ifdef WND_DEBUG
    wnd_report("wind_calc");
#endif
    wind_calc(WC_WORK, wd->flags, _wndh_gl.dw_x, _wndh_gl.dw_y, _wndh_gl.dw_w, _wndh_gl.dw_h, &x, &y, &w, &h);
    w0 = (w < wd->xsize) ? w : wd->xsize;
    h0 = (h < wd->ysize) ? h : wd->ysize;
    if (x0 + w0 > x + w)
    {
      x0 = x + w - w0;
    }
    if (y0 + h0 > y + h)
    {
      y0 = y + h - h0;
    }
    align_window(wd, x0, y0, w0, h0);
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
    wind_get(w_handle, WF_WORKXYWH, &x, &y, &w, &h);
    do_hslider(wd, wd->slider_xpos, w);
    do_vslider(wd, wd->slider_ypos, h);
#ifdef WND_DEBUG
    sprintf(wdbg_tmpbuf, "fulled window %1d", wd->handle);
    wnd_report(wdbg_tmpbuf);
#endif
  }
}

