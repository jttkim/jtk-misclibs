#include "wndincl.h"


int map_window(Window win)
{
  WINDOW_DESCR *wd = _wndh_gl.first_wd;

  while (wd)
  {
    if (wd->user_window == win)
    {
      XMapSubwindows(_wndh_gl.display, wd->window);
      XMapWindow(_wndh_gl.display, wd->window);
      return (0);
    }
    wd = wd->next;
  }
  fprintf(stderr, "wndhndlr: map_window: Window not found in list\n");
  return (-1);
}

