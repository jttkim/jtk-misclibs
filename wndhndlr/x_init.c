#include "wndincl.h"

#include "downarrow"
#include "uparrow"
#include "leftarrow"
#include "rightarrow"
#include "sizer"


/*
static void frickelfix(unsigned char *s)
{
  int i, j;
  unsigned char x;

  for (i = 0; i < 32; i++)
  {
    x = 0;
    for (j = 0; j < 8; j++)
    {
      if ((s[i] & (1 << j)))
	x |= (1 << (7 - j));
    }
    s[i] = x;
  }
}
*/


/*
 * xwin_init() initializes the connection to an X server. The name
 * of the server can be passed in the first argument, it may be
 * NULL, in which case establishing a connection to the default
 * X server is attempted.
 * In case of failure, NULL is returned, when the connection is
 * successfully established, the corresponding pointer to Display
 * is returned. In this case, the default screen number, the width
 * of the default screen and its heigth are returned in the variables
 * pointed to by the remaining arguments.
 */

Display *xwin_init(const char *display_name, const char *prgname, const char *classname, int argc, char **argv, int *pscreen, unsigned int *pw, unsigned int *ph)
{
  XWindowAttributes xwa;

/*
  FILE *f;
  f = fopen("bee.raw", "rb");
  fread(leftarrow_bits, 1, 32, f);
  fclose(f);
  f = fopen("beemask.raw", "rb");
  fread(rightarrow_bits, 1, 32, f);
  fclose(f);
  frickelfix(leftarrow_bits);
  frickelfix(rightarrow_bits);
*/

  if ((_wndh_gl.display = XOpenDisplay(display_name)) == NULL)
  {
    fprintf(stderr, "failed to connect to X server %s\n", XDisplayName(display_name));
    exit (EXIT_FAILURE);
  }
  XSynchronize(_wndh_gl.display, True);
  _wndh_gl.default_screen = DefaultScreen(_wndh_gl.display);
  _wndh_gl.width = DisplayWidth(_wndh_gl.display, _wndh_gl.default_screen);
  _wndh_gl.height = DisplayHeight(_wndh_gl.display, _wndh_gl.default_screen);
  _wndh_gl.black = BlackPixel(_wndh_gl.display, _wndh_gl.default_screen); 
  _wndh_gl.white = WhitePixel(_wndh_gl.display, _wndh_gl.default_screen);
  _wndh_gl.root_window = RootWindow(_wndh_gl.display, _wndh_gl.default_screen);
  XGetWindowAttributes(_wndh_gl.display, _wndh_gl.root_window, &xwa);
  _wndh_gl.depth = xwa.depth;
  _wndh_gl.uparr_pix = XCreatePixmapFromBitmapData(_wndh_gl.display, _wndh_gl.root_window,
	  uparrow_bits, uparrow_width, uparrow_height, _wndh_gl.black, _wndh_gl.white, _wndh_gl.depth);
  _wndh_gl.dnarr_pix = XCreatePixmapFromBitmapData(_wndh_gl.display, _wndh_gl.root_window,
	  downarrow_bits, downarrow_width, downarrow_height, _wndh_gl.black, _wndh_gl.white, _wndh_gl.depth);
  _wndh_gl.lfarr_pix = XCreatePixmapFromBitmapData(_wndh_gl.display, _wndh_gl.root_window,
	  leftarrow_bits, leftarrow_width, leftarrow_height, _wndh_gl.black, _wndh_gl.white, _wndh_gl.depth);
  _wndh_gl.rtarr_pix = XCreatePixmapFromBitmapData(_wndh_gl.display, _wndh_gl.root_window,
	  rightarrow_bits, rightarrow_width, rightarrow_height, _wndh_gl.black, _wndh_gl.white, _wndh_gl.depth);
  _wndh_gl.sizer_pix = XCreatePixmapFromBitmapData(_wndh_gl.display, _wndh_gl.root_window,
	  sizer_bits, sizer_width, sizer_height, _wndh_gl.black, _wndh_gl.white, _wndh_gl.depth);
  _wndh_gl.wm_del_atom = XInternAtom(_wndh_gl.display, "WM_DELETE_WINDOW", True);
  strncpy(_wndh_gl.prgname, prgname, WH_MAX_NAMELENGTH);
  strncpy(_wndh_gl.classname, classname, WH_MAX_NAMELENGTH);
  _wndh_gl.argc = argc;
  _wndh_gl.argv = argv;
  *pscreen = _wndh_gl.default_screen;
  *pw = _wndh_gl.width;
  *ph = _wndh_gl.height;

#ifdef DEBUG
  printf("connected to X server %s\n", XDisplayName(display_name));
#endif

  return (_wndh_gl.display);
}

