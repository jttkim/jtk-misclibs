#include "wndincl.h"


int remove_window(Window win)
{
  WINDOW_DESCR *wd, *wd1, *prev_wd;
  int kind;

  if (!(wd = find_window(win, &kind)))
    return (-1);
  if (wd->wclose(wd->user_window))
  {
    if (_wndh_gl.first_wd == wd)
    {
      _wndh_gl.first_wd = _wndh_gl.first_wd->next;
    }
    else
    {
      wd1 = _wndh_gl.first_wd;
      do
      {
        prev_wd = wd1;
        wd1 = prev_wd->next;
        if (wd == NULL)
          return (-1);
      }
      while (wd1 != wd);
      prev_wd->next = wd->next;
    }
    XDestroyWindow(_wndh_gl.display, wd->window);
    /* printf("freeing window descriptor of %d\n", w_handle); */
    free(wd);
    /* printf("successfully removed\n"); */
    return (1);
  }
  else
  {
    return (0);
  }
}

