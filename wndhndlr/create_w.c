#include "wndincl.h"


static int create_auxiliary_windows(WINDOW_DESCR *wd, int w, int h)
{
  int x, y, sw, thumbsize, thumbpos;
  XSetWindowAttributes xswa;

  if ((wd->flags & SIZER))
  {
    if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE))
      wd->sizer = XCreateSimpleWindow(_wndh_gl.display, wd->window, w + 1, h - 18, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
    else
      wd->sizer = XCreateSimpleWindow(_wndh_gl.display, wd->window, w + 1, h + 1, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
    if (wd->sizer == BadAlloc)
      return (-1);
    XSelectInput(_wndh_gl.display, wd->sizer, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
    XSetWindowBackgroundPixmap(_wndh_gl.display, wd->sizer, _wndh_gl.sizer_pix);
    xswa.win_gravity = SouthEastGravity;
    XChangeWindowAttributes(_wndh_gl.display, wd->sizer, CWWinGravity, &xswa);
  }
  if ((wd->flags & LFARROW) || (wd->flags & RTARROW) || (wd->flags & HSLIDE))
  {
    x = 0;
    wd->hcontrols = XCreateSimpleWindow(_wndh_gl.display, wd->window, 0, h + 1, w, 16, 1, _wndh_gl.black, _wndh_gl.white);
    if (wd->hcontrols == BadAlloc)
      return (-1);
    XSelectInput(_wndh_gl.display, wd->hcontrols, NoEventMask | StructureNotifyMask | VisibilityChangeMask);
    xswa.win_gravity = SouthWestGravity;
    XChangeWindowAttributes(_wndh_gl.display, wd->hcontrols, CWWinGravity, &xswa);
    if ((wd->flags & LFARROW))
    {
      wd->lfarrow = XCreateSimpleWindow(_wndh_gl.display, wd->hcontrols, -1, -1, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
      if (wd->lfarrow == BadAlloc)
	return (-1);
      XSelectInput(_wndh_gl.display, wd->lfarrow, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
      XSetWindowBackgroundPixmap(_wndh_gl.display, wd->lfarrow, _wndh_gl.lfarr_pix);
      xswa.win_gravity = WestGravity;
      XChangeWindowAttributes(_wndh_gl.display, wd->lfarrow, CWWinGravity, &xswa);
      x = 16;
    }
    sw = w;
    if ((wd->flags & LFARROW))
      sw -= 17;
    if ((wd->flags & RTARROW))
      sw -= 17;
    if ((wd->flags & HSLIDE))
    {
      if (sw > 0)
      {
	wd->hslide = XCreateSimpleWindow(_wndh_gl.display, wd->hcontrols, x, -1, sw, 16, 1, _wndh_gl.black, _wndh_gl.white);
	if (wd->hslide == BadAlloc)
	  return (-1);
	XSelectInput(_wndh_gl.display, wd->hslide, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
	thumbsize = w * sw / wd->xsize;
	if (thumbsize > sw - 2)
	  thumbsize = sw - 2;
	if (thumbsize < MIN_THUMBSIZE)
	  thumbsize = MIN_THUMBSIZE;
	if (wd->xsize > w)
	  thumbpos = (sw - 2 - thumbsize) * wd->slider_xpos / (wd->xsize - w);
	else
	  thumbpos = 0;
	wd->hthumb = XCreateSimpleWindow(_wndh_gl.display, wd->hslide, thumbpos + 1, 1, thumbsize, 14, 0, _wndh_gl.black, _wndh_gl.black);
	if (wd->hthumb == BadAlloc)
	  return (-1);
	XSelectInput(_wndh_gl.display, wd->hthumb, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask | ButtonMotionMask);
	XMapWindow(_wndh_gl.display, wd->hthumb);
      }
    }
    x += sw + 1;
    if ((wd->flags & RTARROW))
    {
      wd->rtarrow = XCreateSimpleWindow(_wndh_gl.display, wd->hcontrols, x, -1, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
      if (wd->rtarrow == BadAlloc)
	return (-1);
      XSelectInput(_wndh_gl.display, wd->rtarrow, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
      XSetWindowBackgroundPixmap(_wndh_gl.display, wd->rtarrow, _wndh_gl.rtarr_pix);
      xswa.win_gravity = EastGravity;
      XChangeWindowAttributes(_wndh_gl.display, wd->rtarrow, CWWinGravity, &xswa);
    }
    XMapSubwindows(_wndh_gl.display, wd->hcontrols);
  }

  if ((wd->flags & UPARROW) || (wd->flags & DNARROW) || (wd->flags & VSLIDE) || (wd->flags & SIZER))
  {
    y = 0;
    if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
      wd->vcontrols = XCreateSimpleWindow(_wndh_gl.display, wd->window, w + 1, 0, 16, h - 17, 1, _wndh_gl.black, _wndh_gl.white);
    else
      wd->vcontrols = XCreateSimpleWindow(_wndh_gl.display, wd->window, w + 1, 0, 16, h, 1, _wndh_gl.black, _wndh_gl.white);
    if (wd->vcontrols == BadAlloc)
      return (-1);
    XSelectInput(_wndh_gl.display, wd->vcontrols, NoEventMask | StructureNotifyMask | VisibilityChangeMask);
    xswa.win_gravity = NorthEastGravity;
    XChangeWindowAttributes(_wndh_gl.display, wd->vcontrols, CWWinGravity, &xswa);
    if ((wd->flags & UPARROW))
    {
      wd->uparrow = XCreateSimpleWindow(_wndh_gl.display, wd->vcontrols, -1, -1, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
      if (wd->uparrow == BadAlloc)
	return (-1);
      XSelectInput(_wndh_gl.display, wd->uparrow, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
      XSetWindowBackgroundPixmap(_wndh_gl.display, wd->uparrow, _wndh_gl.uparr_pix);
      xswa.win_gravity = NorthGravity;
      XChangeWindowAttributes(_wndh_gl.display, wd->uparrow, CWWinGravity, &xswa);
      y = 16;
    }
    sw = h;
    if ((wd->flags & UPARROW))
      sw -= 17;
    if ((wd->flags & DNARROW))
      sw -= 17;
    if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
      sw -= 19;
    if ((wd->flags & VSLIDE))
    {
      if (sw > 0)
      {
	wd->vslide = XCreateSimpleWindow(_wndh_gl.display, wd->vcontrols, -1, y, 16, sw, 1, _wndh_gl.black, _wndh_gl.white);
	if (wd->vslide == BadAlloc)
	  return (-1);
	XSelectInput(_wndh_gl.display, wd->vslide, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
	thumbsize = h * sw / wd->ysize;
	if (thumbsize > sw - 2)
	  thumbsize = sw - 2;
	if (thumbsize < MIN_THUMBSIZE)
	  thumbsize = MIN_THUMBSIZE;
	if (wd->ysize > h)
	  thumbpos = (sw - 2 - thumbsize) * wd->slider_ypos / (wd->ysize - h);
	else
	  thumbpos = 0;
	wd->vthumb = XCreateSimpleWindow(_wndh_gl.display, wd->vslide, 1, thumbpos + 1, 14, thumbsize, 0, _wndh_gl.black, _wndh_gl.black);
	if (wd->vthumb == BadAlloc)
	  return (-1);
	XSelectInput(_wndh_gl.display, wd->vthumb, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask | ButtonMotionMask);
	XMapWindow(_wndh_gl.display, wd->vthumb);
      }
    }
    y += sw + 1;
    if ((wd->flags & DNARROW))
    {
      wd->dnarrow = XCreateSimpleWindow(_wndh_gl.display, wd->vcontrols, -1, y, 16, 16, 1, _wndh_gl.black, _wndh_gl.white);
      if (wd->dnarrow == BadAlloc)
	return (-1);
      XSelectInput(_wndh_gl.display, wd->dnarrow, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | VisibilityChangeMask);
      XSetWindowBackgroundPixmap(_wndh_gl.display, wd->dnarrow, _wndh_gl.dnarr_pix);
      xswa.win_gravity = SouthGravity;
      XChangeWindowAttributes(_wndh_gl.display, wd->dnarrow, CWWinGravity, &xswa);
    }
    XMapSubwindows(_wndh_gl.display, wd->vcontrols);
  }
  return (0);
}


/*
 * create a window that is to be managed by wndhndlr. Parameters:
 * flags       : The flags indicating which elements the window should have
 * xsize, ysize: Initial size of the virtual plane
 * redrawfunc  : The function to be called to redraw the window.
 * closefunc   : The function to be called when window closer is clicked on.
 * x, y, w, h  : Initial window geometry
 *
 * All attributes managed and observed by wndhndlr are initialized to more or less
 * sensible values, see code. See the set_* functions for changing them.
 */

Window create_window(int flags, long xsize, long ysize, redrawfunc redraw, closefunc wclose,
        int x, int y, unsigned int w, unsigned int h)
{
  WINDOW_DESCR *wd;
  unsigned int w0, h0, w1, h1;
  char *dummy_name = "wh-dummy", *dummy_icname = "ic-dummy";

  if ((flags & SIZER) || (flags & UPARROW) || (flags & DNARROW) || (flags & VSLIDE))
    w1 = 19;
  else
    w1 = 0;
  if ((flags & LFARROW) || (flags & RTARROW) || (flags & HSLIDE))
    h1 = 19;
  else
    h1 = 0;
  w0 = w + w1;
  h0 = h + h1;
  
  wd = (WINDOW_DESCR *) malloc(sizeof(WINDOW_DESCR));
  if (wd == NULL)
  {
    fprintf(stderr, "wndhndlr: create_window: Out of memory\n");
    return (0);
  }
  else
  {
    wd->window = XCreateSimpleWindow(_wndh_gl.display, _wndh_gl.root_window, x, y, w0, h0, 1, _wndh_gl.black, _wndh_gl.black);
    if (wd->window == BadAlloc)
    {
      fprintf(stderr, "create_window: BadAlloc error\n");
      free(wd);
      return (BadAlloc);
    }
    else if (wd->window == BadMatch)
    {
      fprintf(stderr, "create_window: BadMatch error\n");
      free(wd);
      return (BadMatch);
    }
    else if (wd->window == BadValue)
    {
      fprintf(stderr, "create_window: BadValue error\n");
      free(wd);
      return (BadValue);
    }
    else if (wd->window == BadWindow)
    {
      fprintf(stderr, "create_window: BadWindow error\n");
      free(wd);
      return (BadWindow);
    }
    else
    {
      add_window(wd);
      XSelectInput(_wndh_gl.display, wd->window, ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | ExposureMask | StructureNotifyMask | VisibilityChangeMask | SubstructureNotifyMask | PropertyChangeMask);
      wd->flags = flags;
      wd->xsize = xsize;
      wd->ysize = ysize;
      wd->slider_xpos = 0;
      wd->slider_ypos = 0;
      wd->xsnap = 1;
      wd->ysnap = 1;
      wd->wsnap = 1;
      wd->hsnap = 1;
      wd->arrow_xstep = 10;
      wd->arrow_ystep = 10;
      wd->vplane_xsnap = 0;
      wd->vplane_ysnap = 0;
      wd->bg_fillcolor = _wndh_gl.white;
      wd->redraw = redraw;
      wd->wclose = wclose;
      wd->icon_pixmap = BadValue;
      wd->user_window = XCreateSimpleWindow(_wndh_gl.display, wd->window, 0, 0, w, h, 1, _wndh_gl.black, _wndh_gl.white);
      if (wd->user_window == BadAlloc)
	return (-1);
      XSelectInput(_wndh_gl.display, wd->user_window, WH_EVENTMASK);
      if (create_auxiliary_windows(wd, w, h) < 0)
      {
	fprintf(stderr, "wndhndlr: create_window: Problems creating auxiliary windows\n");
	return (BadValue);
      }
      if (XStringListToTextProperty(&dummy_name, 1, &(wd->win_name)) == 0)
      {
	fprintf(stderr, "wndhndlr: create_window: Failed to allocate text property for window name\n");
	return (BadAlloc);
      }
      if (XStringListToTextProperty(&dummy_icname, 1, &(wd->icon_name)) == 0)
      {
	fprintf(stderr, "wndhndlr: create_window: Failed to allocate text property for icon name\n");
	return (BadAlloc);
      }
      wd->size_hints.x = x;
      wd->size_hints.y = y;
      wd->size_hints.width = w0;
      wd->size_hints.height = h0;
      wd->size_hints.min_width = MIN_WINSIZE;
      if (flags & LFARROW)
	wd->size_hints.min_width += 16;
      if (flags & RTARROW)
	wd->size_hints.min_width += 16;
      wd->size_hints.min_height = MIN_WINSIZE;
      if (flags & UPARROW)
	wd->size_hints.min_height += 16;
      if (flags & DNARROW)
	wd->size_hints.min_height += 16;
      wd->size_hints.max_width = (wd->xsize + w1 < _wndh_gl.width) ? wd->xsize + w1 : _wndh_gl.width;
      wd->size_hints.max_height = (wd->ysize + h1 < _wndh_gl.height) ? wd->ysize + h1 : _wndh_gl.height;
      wd->size_hints.width_inc = wd->wsnap;
      wd->size_hints.height_inc = wd->hsnap;
      if (!(wd->flags & SIZER))
      {
	wd->size_hints.min_width = w0;
	wd->size_hints.min_height = h0;
	wd->size_hints.max_width = w0;
	wd->size_hints.max_height = h0;
      }
      wd->size_hints.flags = PPosition | PSize | PMinSize | PResizeInc | PMaxSize;
      wd->wm_hints.input = True;
      wd->wm_hints.initial_state = NormalState;
      wd->wm_hints.flags = InputHint | StateHint;
      wd->class_hints.res_name = _wndh_gl.prgname;
      wd->class_hints.res_class = _wndh_gl.classname;
      XSetWMProperties(_wndh_gl.display, wd->window, &(wd->win_name), &(wd->icon_name), _wndh_gl.argv, _wndh_gl.argc,
	      &(wd->size_hints), &(wd->wm_hints), &(wd->class_hints));
      if (_wndh_gl.wm_del_atom != None)
	XSetWMProtocols(_wndh_gl.display, wd->window, &_wndh_gl.wm_del_atom, 1);
      return (wd->user_window);
    }
  }
}

