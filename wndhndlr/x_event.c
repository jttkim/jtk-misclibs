#include "wndincl.h"


static int slider_coordinate;
static XWindowAttributes xwa;


static void draw_window(WINDOW_DESCR *wd, int kind, const XExposeEvent *xexpose)
{
  switch (kind)
  {
  case USER_WINDOW:
    (wd->redraw)(wd->user_window, xexpose->x, xexpose->y, xexpose->width, xexpose->height, wd->slider_xpos, wd->slider_ypos);
    break;
  default:
    /* fprintf(stderr, "Received expose event for window of kind %04x\n", kind); */
    break;
  }
}


static void reconfigure_window(WINDOW_DESCR *wd, const XConfigureEvent *xconfigure)
{
  int x, y, w, h, sw, thumbsize, thumbpos;

  if ((wd->flags & UPARROW) || (wd->flags & DNARROW) || (wd->flags & VSLIDE) || (wd->flags & SIZER))
    w = xconfigure->width - 19;
  else
    w = xconfigure->width;
  if ((wd->flags & LFARROW) || (wd->flags & RTARROW) || (wd->flags & HSLIDE))
    h = xconfigure->height - 19;
  else
    h = xconfigure->height;
  /* printf("reconfiguring window to w = %d, h = %d\n", w, h); */
  if ((wd->flags & LFARROW) || (wd->flags & RTARROW) || (wd->flags & HSLIDE))
  {
    x = 0;
    if (wd->slider_xpos > wd->xsize - w)
      wd->slider_xpos = wd->xsize - w;
    XResizeWindow(_wndh_gl.display, wd->hcontrols, w, 16);
    /* XClearWindow(_wndh_gl.display, wd->hcontrols); */
    if ((wd->flags & LFARROW))
      x = 16;
    if ((wd->flags & HSLIDE))
    {
      sw = w;
      if ((wd->flags & LFARROW))
	sw -= 17;
      if ((wd->flags & RTARROW))
	sw -= 17;
      if (sw > 0)
      {
	XResizeWindow(_wndh_gl.display, wd->hslide, sw, 16);
	/* XClearWindow(_wndh_gl.display, wd->hslide); */
	thumbsize = w * sw / wd->xsize;
	if (thumbsize > sw - 2)
	  thumbsize = sw - 2;
	if (thumbsize < MIN_THUMBSIZE)
	  thumbsize = MIN_THUMBSIZE;
	if (w < wd->xsize)
	  thumbpos = (sw - 2 - thumbsize) * wd->slider_xpos / (wd->xsize - w);
	else
	  thumbpos = 0;
	XMoveResizeWindow(_wndh_gl.display,  wd->hthumb, thumbpos + 1, 1, thumbsize, 14);
	/* XClearWindow(_wndh_gl.display,  wd->hthumb); */
	/* printf("w = %d, xsize = %ld -> hthumb size: %d, pos: %d\n", w, wd->xsize, thumbsize, thumbpos + 1); */
      }
      x += sw + 1;
    }
  }

  if ((wd->flags & UPARROW) || (wd->flags & DNARROW) || (wd->flags & VSLIDE) || (wd->flags & SIZER))
  {
    y = 0;
    if (wd->slider_ypos > wd->ysize - h)
      wd->slider_ypos = wd->ysize - h;
    if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
      XResizeWindow(_wndh_gl.display, wd->vcontrols, 16, h - 19);
    else
      XResizeWindow(_wndh_gl.display, wd->vcontrols, 16, h);
    /* XClearWindow(_wndh_gl.display, wd->vcontrols); */
    if ((wd->flags & UPARROW))
      y = 16;
    if ((wd->flags & VSLIDE))
    {
      sw = h;
      if ((wd->flags & UPARROW))
	sw -= 17;
      if ((wd->flags & DNARROW))
	sw -= 17;
      if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
	sw -= 19;
      if (sw > 0)
      {
	XResizeWindow(_wndh_gl.display, wd->vslide, 16, sw);
	/* XClearWindow(_wndh_gl.display, wd->vslide); */
	thumbsize = h * sw / wd->ysize;
	if (thumbsize > sw - 2)
	  thumbsize = sw - 2;
	if (thumbsize < MIN_THUMBSIZE)
	  thumbsize = MIN_THUMBSIZE;
	if (wd->ysize > h)
	  thumbpos = (sw - 2 - thumbsize) * wd->slider_ypos / (wd->ysize - h);
	else
	  thumbpos = 0;
	XMoveResizeWindow(_wndh_gl.display,wd->vthumb, 1, thumbpos + 1, 14, thumbsize);
	/* XClearWindow(_wndh_gl.display,wd->vthumb); */
	/* printf("vthumb size: %d, pos: %d\n", thumbsize, thumbpos + 1); */
      }
      y += sw + 1;
    }
  }
  XResizeWindow(_wndh_gl.display, wd->user_window, w, h);
  /* XClearWindow(_wndh_gl.display, wd->user_window); */
}


static void move_hslider(WINDOW_DESCR *wd, const XWindowAttributes *xwa)
{
  int x, sw, thumbsize, thumbpos;

  if (!(wd->flags & HSLIDE))
    return;
  sw = xwa->width;
  if ((wd->flags & LFARROW))
  {
    x = 16;
    sw -= 17;
  }
  else
    x = 0;
  if ((wd->flags & RTARROW))
    sw -= 17;
  if (sw > 0)
  {
    /* XClearWindow(_wndh_gl.display, wd->hslide); */
    thumbsize = xwa->width* sw / wd->xsize;
    if (thumbsize > sw - 2)
      thumbsize = sw - 2;
    if (thumbsize < MIN_THUMBSIZE)
      thumbsize = MIN_THUMBSIZE;
    thumbpos = (sw - 2 - thumbsize) * wd->slider_xpos / (wd->xsize - xwa->width);
    XMoveWindow(_wndh_gl.display, wd->hthumb, thumbpos + 1, 1);
    /* XClearWindow(_wndh_gl.display,wd->hthumb); */
    /* printf("hthumb size: %d, pos: %d\n", thumbsize, thumbpos); */
  }
}


static void move_vslider(WINDOW_DESCR *wd, const XWindowAttributes *xwa)
{
  int y, sw, thumbsize, thumbpos;

  if (!(wd->flags & VSLIDE))
    return;
  sw = xwa->height;
  if ((wd->flags & UPARROW))
  {
    y = 16;
    sw -= 17;
  }
  else
    y = 0;
  if ((wd->flags & DNARROW))
    sw -= 17;
  if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
    sw -= 19;
  if (sw > 0)
  {
    /* XClearWindow(_wndh_gl.display, wd->vslide); */
    thumbsize = xwa->height * sw / wd->ysize;
    if (thumbsize > sw - 2)
      thumbsize = sw - 2;
    if (thumbsize < MIN_THUMBSIZE)
      thumbsize = MIN_THUMBSIZE;
    thumbpos = (sw - 2 - thumbsize) * wd->slider_ypos / (wd->ysize - xwa->height);
    XMoveWindow(_wndh_gl.display, wd->vthumb, 1, thumbpos + 1);
    /* XClearWindow(_wndh_gl.display,wd->vthumb); */
    /* printf("vthumb size: %d, pos: %d\n", thumbsize, thumbpos); */
  }
}


static void arrow_up(WINDOW_DESCR *wd)
{
  /* printf("up-arrow detected\n"); */
  XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
  if ((wd->ysize > xwa.height) && (wd->slider_ypos > 0))
  {
    if (wd->slider_ypos > wd->arrow_ystep)
      wd->slider_ypos -= wd->arrow_ystep;
    else
      wd->slider_ypos = 0;
    move_vslider(wd, &xwa);
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
  }
}


static void arrow_down(WINDOW_DESCR *wd)
{
  /* printf("down-arrow detected\n"); */
  XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
  if ((wd->ysize > xwa.height) && (wd->slider_ypos < wd->xsize - xwa.height))
  {
    wd->slider_ypos += wd->arrow_ystep;
    if (wd->slider_ypos > wd->ysize - xwa.height)
      wd->slider_ypos = wd->ysize - xwa.height;
    move_vslider(wd, &xwa);
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
  }
}


static void arrow_left(WINDOW_DESCR *wd)
{
  /* printf("left-arrow detected\n"); */
  XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
  if ((wd->xsize > xwa.width) && (wd->slider_xpos > 0))
  {
    if (wd->slider_xpos > wd->arrow_xstep)
      wd->slider_xpos -= wd->arrow_xstep;
    else
      wd->slider_xpos = 0;
    move_hslider(wd, &xwa);
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
  }
}


static void arrow_right(WINDOW_DESCR *wd)
{
  /* printf("right-arrow detected\n"); */
  XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
  if ((wd->xsize > xwa.width) && (wd->slider_xpos < wd->xsize - xwa.width))
  {
    wd->slider_xpos += wd->arrow_xstep;
    if (wd->slider_xpos > wd->xsize - xwa.width)
      wd->slider_xpos = wd->xsize - xwa.width;
    move_hslider(wd, &xwa);
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
  }
}


static int dispatch_button(WINDOW_DESCR *wd, int kind, const XButtonEvent *xbutton)
{
  switch (kind)
  {
  case UPARROW:
    arrow_up(wd);
    return (1);
  case DNARROW:
    arrow_down(wd);
    return (1);
  case LFARROW:
    arrow_left(wd);
    return (1);
  case RTARROW:
    arrow_right(wd);
    return (1);
  case HTHUMB:
    slider_coordinate = xbutton->x_root;
    XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
    return (1);
  case VTHUMB:
    slider_coordinate = xbutton->y_root;
    XGetWindowAttributes(_wndh_gl.display, wd->user_window, &xwa);
    return (1);
  }
  return (0);
}


static int dispatch_motion(WINDOW_DESCR *wd, int kind, const XMotionEvent *xmotion)
{
  int d, thumbsize, sw;

  if (kind == HTHUMB)
  {
    if ((d = xmotion->x_root - slider_coordinate))
    {
      if (((d > 0) && (wd->slider_xpos < wd->xsize - xwa.width)) || ((d < 0) && (wd->slider_xpos > 0)))
      {
	sw = xwa.width;
	if ((wd->flags & LFARROW))
	  sw -= 17;
	if ((wd->flags & RTARROW))
	  sw -= 17;
	if (sw > 0)
	{
	  thumbsize = xwa.width * sw / wd->xsize;
	  if (thumbsize > sw - 2)
	    thumbsize = sw - 2;
	  if (thumbsize < MIN_THUMBSIZE)
	    thumbsize = MIN_THUMBSIZE;
	  wd->slider_xpos += d * (wd->xsize - xwa.width) / (xwa.width - thumbsize);
	  if (wd->slider_xpos < 0)
	    wd->slider_xpos = 0;
	  if (wd->slider_xpos > wd->xsize - xwa.width)
	    wd->slider_xpos = wd->xsize - xwa.width;
	  move_hslider(wd, &xwa);
	}
      }
      slider_coordinate = xmotion->x_root;
    }
    /* printf("root relative coordinates: x = %d, y = %d\n", xmotion->x_root, xmotion->y_root); */
    return (1);
  }
  else if (kind == VTHUMB)
  {
    if ((d = xmotion->y_root - slider_coordinate))
    {
      if (((d > 0) && (wd->slider_ypos < wd->ysize - xwa.height)) || ((d < 0) && (wd->slider_ypos > 0)))
      {
	sw = xwa.height;
	if ((wd->flags & UPARROW))
	  sw -= 17;
	if ((wd->flags & DNARROW))
	  sw -= 17;
	if (!(wd->flags & LFARROW) && !(wd->flags & RTARROW) && !(wd->flags & HSLIDE) && (wd->flags & SIZER))
	  sw -= 19;
	if (sw > 0)
	{
	  thumbsize = xwa.height * sw / wd->ysize;
	  if (thumbsize > sw - 2)
	    thumbsize = sw - 2;
	  if (thumbsize < MIN_THUMBSIZE)
	    thumbsize = MIN_THUMBSIZE;
	  wd->slider_ypos += d * (wd->ysize - xwa.height) / (xwa.height - thumbsize);
	  if (wd->slider_ypos < 0)
	    wd->slider_ypos = 0;
	  if (wd->slider_ypos > wd->ysize - xwa.height)
	    wd->slider_ypos = wd->ysize - xwa.height;
	  move_vslider(wd, &xwa);
	}
      }
      slider_coordinate = xmotion->y_root;
    }
    /* printf("root relative coordinates: x = %d, y = %d\n", xmotion->x_root, xmotion->y_root); */
    return (1);
  }
  return (0);
}


static int dispatch_button_release(WINDOW_DESCR *wd, int kind, const XButtonEvent *xbutton)
{
  if ((kind == HTHUMB) || (kind == VTHUMB))
  {
    XClearWindow(_wndh_gl.display, wd->user_window);
    (wd->redraw)(wd->user_window, xwa.x, xwa.y, xwa.width, xwa.height, wd->slider_xpos, wd->slider_ypos);
    return (1);
  }
  return (0);
}


int process_xevent(const XEvent *xevent)
{
  WINDOW_DESCR *wd;
  int kind = 0;
  long msg_data;

  switch (xevent->type)
  {
  case Expose:
    if ((wd = find_window(xevent->xexpose.window, &kind)))
      draw_window(wd, kind, &(xevent->xexpose));
    return (1);
  case ConfigureNotify:
    if ((wd = find_window(xevent->xconfigure.window, &kind)))
    {
      if (kind == BACKGROUND_WINDOW)
	reconfigure_window(wd, &(xevent->xconfigure));
    }
    return (1);
  case ButtonPress:
    if ((wd = find_window(xevent->xbutton.window, &kind)))
    {
      /* printf("window of kind %04x received button press\n", kind); */
      return (dispatch_button(wd, kind, &(xevent->xbutton)));
    }
    return (0);
  case ButtonRelease:
    if ((wd = find_window(xevent->xbutton.window, &kind)))
    {
      /* printf("window of kind %04x received button release\n", kind); */
      return (dispatch_button_release(wd, kind, &(xevent->xbutton)));
    }
    return (0);
  case ClientMessage:
    if ((wd = find_window(xevent->xclient.window, &kind)))
    {
      /* printf("window of kind %04x received client message event, type %ld\n", kind, (long) xevent->xclient.message_type); */
      if (xevent->xclient.format == 8)
	msg_data = xevent->xclient.data.b[0];
      else if (xevent->xclient.format == 16)
	msg_data = xevent->xclient.data.s[0];
      else if (xevent->xclient.format == 32)
	msg_data = xevent->xclient.data.l[0];
      /* printf("message data: %ld\n", msg_data); */
      if ((wd->flags & CLOSER) && (msg_data == _wndh_gl.wm_del_atom))
	remove_window(xevent->xclient.window);
    }
    return (1);
  case MotionNotify:
    if ((wd = find_window(xevent->xmotion.window, &kind)))
    {
      /* printf("window of kind %04x received button motion event\n", kind); */
      return (dispatch_motion(wd, kind, &(xevent->xmotion)));
    }
    return (0);
  }
  return (0);
}

