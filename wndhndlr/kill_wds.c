#include "wndhndlr.h"
#include "wndincl.h"

/*
 * remove all windows managed by wndhndlr
 */

void kill_windows(void)
{
  while (_wndh_gl.first_wd != NULL)
  {
    remove_window(_wndh_gl.first_wd->handle);
  }
}

