#include "wndincl.h"

int set_background(int w_handle, int fillstyle, int fillindex, int fillcolor)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  if (fillstyle != -1)
    wd->bg_fillstyle = fillstyle;
  if (fillindex != -1)
    wd->bg_fillindex = fillindex;
  if (fillcolor != -1)
    wd->bg_fillcolor = fillcolor;
  return (0);
}

