#include "wndincl.h"


#ifdef WND_DEBUG

FILE * wdebug_report = NULL;
unsigned long wdbg_call_no = 0;
char wdbg_tmpbuf[256];

#endif

/*
typedef struct
{
  Display *display;
  int default_screen;
  Window root_window;
  int depth;
  unsigned int width, height;
  unsigned long black, white;
  Pixmap uparr_pix, dnarr_pix, lfarr_pix, rtarr_pix, sizer_pix;
  Atom wm_del_atom;
  WINDOW_DESCR *first_wd;
  char prgname[WH_MAX_NAMELENGTH], classname[WH_MAX_NAMELENGTH];
  int argc;
  char **argv;
} WNDHNDLR_GLOBLS;
*/


WNDHNDLR_GLOBLS _wndh_gl = { NULL,
			    0,
			    0,
			    0,
			    0, 0,
			    0, 0,
			    0, 0, 0, 0, 0,
			    0,
			    NULL,
			    "", "",
			    0, NULL };



WINDOW_DESCR *find_window(Window win, int *kind)
{
  WINDOW_DESCR *wd = _wndh_gl.first_wd;

  while (wd)
  {
    if (wd->window == win)
    {
      *kind = BACKGROUND_WINDOW;
      return (wd);
    }
    if (wd->user_window == win)
    {
      *kind = USER_WINDOW;
      return (wd);
    }
    if ((wd->flags & UPARROW) && (wd->uparrow == win))
    {
      *kind = UPARROW;
      return (wd);
    }
    if ((wd->flags & DNARROW) && (wd->dnarrow == win))
    {
      *kind = DNARROW;
      return (wd);
    }
    if ((wd->flags & LFARROW) && (wd->lfarrow == win))
    {
      *kind = LFARROW;
      return (wd);
    }
    if ((wd->flags & RTARROW) && (wd->rtarrow == win))
    {
      *kind = RTARROW;
      return (wd);
    }
    if ((wd->flags & HSLIDE))
    {
      if (wd->hslide == win)
      {
	*kind = HSLIDE;
	return (wd);
      }
      if (wd->hthumb == win)
      {
	*kind = HTHUMB;
	return (wd);
      }
    }
    if ((wd->flags & VSLIDE))
    {
      if (wd->vslide == win)
      {
	*kind = VSLIDE;
	return (wd);
      }
      if (wd->vthumb == win)
      {
	*kind = VTHUMB;
	return (wd);
      }
    }
    wd = wd->next;
  }
  return (NULL);
}


#ifdef NOT_YET

/*
 * align a window's position and size according to the information
 * stored in the descriptor.
 */

void align_window(WINDOW_DESCR *wd, int x, int y, int w, int h)
{
  int x0, y0, w0, h0;

  x -= x % wd->xsnap;
  y -= y % wd->ysnap;
  w -= w % wd->wsnap;
  h -= h % wd->hsnap;
  if (w == 0)
    w = wd->wsnap;
  if (h == 0)
    h = wd->hsnap;
  wind_calc(WC_BORDER, wd->flags, x, y, w, h, &x0, &y0, &w0, &h0);
  if (x0 < 0)
  {
    x0 += -x0 - (-x0 % wd->xsnap) + wd->xsnap;
  }
  if (y0 < 0)
  {
    y0 += -y0 - (-y0 % wd->ysnap) + wd->ysnap;
  }
  wind_set(wd->handle, WF_CURRXYWH, x0, y0, w0, h0);
}


/*
 * redraw a rectangle, given by x, y, w, h, in window w_handle.
 * w_redraw() checks whether the specified rectangle has an intersection
 * with the background window. If so, it walks through the window's
 * rectangle list and calls the redraw function for the window for
 * each intersection of the rectangle passed to the function, the
 * background window and the rectangle from the list.
 * Note: The redraw function for a window must not call wind_update().
 * This has to be done by this function, w_redraw(), as the rectangle
 * list needs to be frozen during a walk through it.
 */

void w_redraw(int w_handle, int x, int y, int w, int h)
{
  int x0, y0, w0, h0;
  int clipxy[4];
  WINDOW_DESCR *wd;

  wd = get_window_descr(w_handle);
  if (wd != NULL)
  {
    if (rc_inter(_wndh_gl.dw_x, _wndh_gl.dw_y, _wndh_gl.dw_w, _wndh_gl.dw_h, &x, &y, &w, &h))
    {
      graf_mouse(M_OFF, NULL);
#ifdef WND_DEBUG
      wnd_report("wind_update(BEG_UPDATE)");
#endif
      wind_update(BEG_UPDATE);
#ifdef WND_DEBUG
      sprintf(wdbg_tmpbuf, "wind_get(%1d, WF_FIRSTXYWH, &x0, &y0, &w0, &h0)", w_handle);
      wnd_report(wdbg_tmpbuf);
#endif
      wind_get(w_handle, WF_FIRSTXYWH, &x0, &y0, &w0, &h0);
#ifdef WND_DEBUG
      sprintf(wdbg_tmpbuf, "  got(x0 = %1d, y0 = %1d, w0 = %1d, h0 = %1d)", x0, y0, w0, h0);
      wnd_report(wdbg_tmpbuf);
#endif
      while ((w0 > 0) || (h0 > 0))
      {
        if (rc_inter(x, y, w, h, &x0, &y0, &w0, &h0))
        {
          vswr_mode(wd->vscr_handle, MD_REPLACE);
          vsf_interior(wd->vscr_handle, wd->bg_fillstyle);
          vsf_style(wd->vscr_handle, wd->bg_fillindex);
          vsf_color(wd->vscr_handle, wd->bg_fillcolor);
          clipxy[0] = x0;
          clipxy[1] = y0;
          clipxy[2] = x0 + w0 - 1;
          clipxy[3] = y0 + h0 - 1;
          vs_clip(wd->vscr_handle, 1, clipxy);
          v_bar(wd->vscr_handle, clipxy);
          wd->redraw(w_handle, x0, y0, w0, h0, wd->slider_xpos, wd->slider_ypos);
        }
#ifdef WND_DEBUG
        sprintf(wdbg_tmpbuf, "wind_get(%1d, WF_NEXTXYWH, &x0, &y0, &w0, &h0)", w_handle);
        wnd_report(wdbg_tmpbuf);
#endif
        if (!wind_get(w_handle, WF_NEXTXYWH, &x0, &y0, &w0, &h0))
        {
#ifdef WND_DEBUG
          wnd_report("  got an error");
#endif
          break;
        }
#ifdef WND_DEBUG
        sprintf(wdbg_tmpbuf, "  got(x0 = %1d, y0 = %1d, w0 = %1d, h0 = %1d)", x0, y0, w0, h0);
        wnd_report(wdbg_tmpbuf);
#endif
      }
#ifdef WND_DEBUG
      wnd_report("wind_update(END_UPDATE)");
#endif
      wind_update(END_UPDATE);
      graf_mouse(M_ON, NULL);
    }
  }
}


/*
 * position horizontal slider of window described in *wd. pos specifies the
 * new position of the left edge of the slider in coordinates of the
 * virtual plane, w_width indicates the current window height, necessary for
 * calculating the slider size.
 */

void do_hslider(WINDOW_DESCR *wd, long pos, int w_width)
{
  long p;

  if (wd->xsize > w_width)
  {
    p = wd->xsize - w_width;
    pos = (pos < p) ? pos : p;
    pos = pos > 0 ? pos : 0;
    if (wd->vplane_xsnap)
    {
      pos -= pos % wd->arrow_xstep;
    }
#ifdef WND_DEBUG
  wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_HSLIDE, (pos * 1000) / p);
#ifdef WND_DEBUG
  wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_HSLSIZE, (w_width * 1000) / wd->xsize);
  }
  else
  {
    pos = 0;
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_HSLIDE, 0);
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_HSLSIZE, 1000);
  }
  wd->slider_xpos = pos;
}


/*
 * analogous to do_hslider()
 */

void do_vslider(WINDOW_DESCR *wd, long pos, int w_height)
{
  long p;

  if (wd->ysize > w_height)
  {
    p = wd->ysize - w_height;
    pos = (pos < p) ? pos : p;
    pos = pos > 0 ? pos : 0;
    if (wd->vplane_ysnap)
    {
      pos -= pos % wd->arrow_ystep;
    }
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_VSLIDE, (pos * 1000) / p);
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_VSLSIZE, (w_height * 1000) / wd->ysize);
  }
  else
  {
    pos = 0;
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_VSLIDE, 0);
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(wd->handle, WF_VSLSIZE, 1000);
  }
  wd->slider_ypos = pos;
}


#endif /* NOT_YET */

/*
 * add a window to wndhndlr's internal list
 */

void add_window(WINDOW_DESCR *wd)
{
  WINDOW_DESCR *next_wd;

  if (_wndh_gl.first_wd == NULL)
  {
    _wndh_gl.first_wd = wd;
  }
  else
  {
    next_wd = _wndh_gl.first_wd;
    while (next_wd->next != NULL)
    {
      next_wd = next_wd->next;
    }
    next_wd->next = wd;
  }
  wd->next = NULL;
}

