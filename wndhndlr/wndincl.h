#ifndef H_WNDINCL
#define H_WNDINCL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include "wndhndlr.h"

#ifdef WND_DEBUG

extern FILE * wdebug_report;
extern unsigned long wdbg_call_no;
extern char wdbg_tmpbuf[256];

#define wnd_report(x) printf("\033Y\040\160wh #%4lu: %d: %s\033K\n", wdbg_call_no, __LINE__, x); \
  if (wdebug_report != NULL) {fprintf(wdebug_report, "#%lu in %d: %s\n", wdbg_call_no, __LINE__, x); \
  fclose(wdebug_report); wdebug_report = fopen("wndhndlr.dbg", "a"); } \
  wdbg_call_no++;

#endif

#ifndef H_WNDHNDLR

typedef void (*redrawfunc)(int w_handle, int x, int y, int w, int h, long slider_xpos, long slider_ypos);
typedef int (*closefunc)(int w_handle);

#endif

#define MIN_THUMBSIZE 16
#define MIN_WINSIZE   32
#define WH_MAX_NAMELENGTH 100

typedef struct tag_wd
{
  Window  window;
  Window  user_window;
  Window  sizer;
  Window  vcontrols, uparrow, dnarrow, vslide, vthumb;
  Window  hcontrols, lfarrow, rtarrow, hslide, hthumb;
  struct tag_wd *next;
  int     flags;              /* flags indicating which elements are present */
  long    xsize;              /* size of the virtual plane */
  long    ysize;              /* of which some part is shown in the window */
  long    slider_xpos;        /* slider positions */
  long    slider_ypos;
  int     xsnap;              /* snapping for workspace position on screen */
  int     ysnap;
  int     wsnap;              /* snapping for workspace width and height */
  int     hsnap;
  char    vplane_xsnap;       /* flags indicating whether the virtual plane */
  char    vplane_ysnap;       /* should be aligned in the workspace by the arrow stepsizes */
  int     arrow_xstep;        /* number of positions moved by arrow */
  int     arrow_ystep;
  int     bg_fillcolor;       /* background fill color */
  redrawfunc redraw;          /* pointer to function for redrawing window contents */
  closefunc  wclose;          /* pointer to function called after closing a window */
  XSizeHints size_hints;
  XWMHints wm_hints;
  XClassHint class_hints;
  XTextProperty win_name, icon_name;
  Pixmap icon_pixmap;
} WINDOW_DESCR;

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

extern WNDHNDLR_GLOBLS _wndh_gl;

extern WINDOW_DESCR *find_window(Window win, int *kind);
extern void align_window(WINDOW_DESCR *wd, int x, int y, int w, int h);
extern void w_redraw(int w_handle, int x, int y, int w, int h);
extern void do_hslider(WINDOW_DESCR *wd, long pos, int w_width);
extern void do_vslider(WINDOW_DESCR *wd, long pos, int w_height);
extern void add_window(WINDOW_DESCR *wd);

#endif

