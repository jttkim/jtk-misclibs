#include "wndhndlr.h"
#include "wndincl.h"

/*
 * process a WM_ARROWED message
 */

void w_arrow(int w_handle, int arrow)
{
  int x, y, w, h;
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd != NULL)
  {
#ifdef WND_DEBUG
    wnd_report("wind_get");
#endif
    wind_get(w_handle, WF_WORKXYWH, &x, &y, &w, &h);
    switch (arrow)
    {
    case WA_UPPAGE:
      do_vslider(wd, wd->slider_ypos - h, h);
      break;
    case WA_DNPAGE:
      do_vslider(wd, wd->slider_ypos + h, h);
      break;
    case WA_UPLINE:
      do_vslider(wd, wd->slider_ypos - wd->arrow_ystep, h);
      break;
    case WA_DNLINE:
      do_vslider(wd, wd->slider_ypos + wd->arrow_ystep, h);
      break;
    case WA_LFPAGE:
      do_hslider(wd, wd->slider_xpos - w, w);
      break;
    case WA_RTPAGE:
      do_hslider(wd, wd->slider_xpos + w, w);
      break;
    case WA_LFLINE:
      do_hslider(wd, wd->slider_xpos - wd->arrow_xstep, w);
      break;
    case WA_RTLINE:
      do_hslider(wd, wd->slider_xpos + wd->arrow_xstep, w);
      break;
    }
    w_redraw_all(w_handle);
  }
}

