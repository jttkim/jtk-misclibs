#include "wndincl.h"

int get_slider_positions(int w_handle, long *xpos, long *ypos)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  *xpos = wd->slider_xpos;
  *ypos = wd->slider_ypos;
  return (0);
}

