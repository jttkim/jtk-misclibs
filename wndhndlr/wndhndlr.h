#ifndef H_WNDHNDLR
#define H_WNDHNDLR

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#define USER_WINDOW     0x0000
#define BACKGROUND_WINDOW 0x7fff
#define HTHUMB          0x08ff
#define VTHUMB          0x01ff

#define NAME            0x0001
#define CLOSER          0x0002
#define FULLER          0x0004
#define MOVER           0x0008
#define INFO            0x0010
#define SIZER           0x0020
#define UPARROW         0x0040
#define DNARROW         0x0080
#define VSLIDE          0x0100
#define LFARROW         0x0200
#define RTARROW         0x0400
#define HSLIDE          0x0800

#define WH_EVENTMASK    ExposureMask
#define WH_SELECTMASK   ButtonPressMask | ButtonReleaseMask | ButtonMotionMask

#define is_valid_window(w) ((w != BadAlloc) && (w != BadMatch) && (w != BadValue) && (w != BadWindow))

typedef void (*redrawfunc)(Window win, int x, int y, unsigned int w, unsigned int h, long slider_xpos, long slider_ypos);
typedef int (*closefunc)(Window win);

extern Display *xwin_init(const char *display_name, const char *prgname, const char *classname, int argc, char **argv, int *pscreen, unsigned int *pw, unsigned int *ph);
extern Window create_window(int flags, long xsize, long ysize, redrawfunc redraw, closefunc wclose,
        int x, int y, unsigned int w, unsigned int h);
extern int map_window(Window win);
extern void w_redraw_all(Window win);

extern int remove_window(Window win);

extern int process_xevent(const XEvent *xevent);

extern void mouse_boxselect(Window win, int x, int y, int *w, int *h);

#endif

