#include "wndincl.h"

/*
 * process WM_MOVED message
 */

void w_moved(int w_handle, int x, int y, int w, int h)
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
    sprintf(wdbg_tmpbuf, "moved window %1d", wd->handle);
    wnd_report(wdbg_tmpbuf);
#endif
  }
}

