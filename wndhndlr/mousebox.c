#include "wndincl.h"


void mouse_boxselect(Window win, int x, int y, int *w, int *h)
{
  XEvent xevent;
  GC gc;
  XGCValues values;
  int rx, ry, rw, rh;

  values.foreground = 0xffffffff;
  values.function = GXxor;
  gc = XCreateGC(_wndh_gl.display, win, GCFunction | GCForeground, &values);
  *w = 0;
  *h = 0;
  rx = x;
  ry = y;
  rw = 0;
  rh = 0;
  for (;;)
  {
    XWindowEvent(_wndh_gl.display, win, ButtonReleaseMask | ButtonMotionMask, &xevent);
    if (xevent.type == ButtonRelease)
      break;
    else if (xevent.type == MotionNotify)
    {
      if ((*w != 0) && (*h != 0))
	XDrawRectangle(_wndh_gl.display, win, gc, rx, ry, rw, rh);
      *w = xevent.xmotion.x - x;
      *h = xevent.xmotion.y - y;
      if (*w < 0)
      {
	rx = x + *w;
	rw = -*w;
      }
      else
      {
	rx = x;
	rw = *w;
      }
      if (*h < 0)
      {
	ry = y + *h;
	rh = -*h;
      }
      else
      {
	ry = y;
	rh = *h;
      }
      if ((*w != 0) && (*h != 0))
	XDrawRectangle(_wndh_gl.display, win, gc, rx, ry, rw, rh);
    }
  }
  if ((*w != 0) && (*h != 0))
    XDrawRectangle(_wndh_gl.display, win, gc, rx, ry, rw, rh);
  XFreeGC(_wndh_gl.display, gc);
}

