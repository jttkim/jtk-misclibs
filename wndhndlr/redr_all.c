#include "wndincl.h"


/*
 * redraws a window entirely
 */

void w_redraw_all(Window win)
{
  WINDOW_DESCR *wd;
  int kind;
  XWindowAttributes xwa;

  if ((wd = find_window(win, &kind)))
  {
    XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
  }
  else
    fprintf(stderr, "wndhndlr: w_redraw_all: Warning: Window not found\n");
}

