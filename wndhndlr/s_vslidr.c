#include "wndincl.h"

int set_vslider(int w_handle, long pos, int w_height)
{
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd == NULL)
  {
    return (-1);
  }
  do_vslider(wd, pos, w_height);
  return (0);
}

