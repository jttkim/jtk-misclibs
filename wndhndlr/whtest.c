#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include "wndhndlr.h"

#ifndef PI
#  define PI 3.1415927
#endif


char *event_name[] = {
"",
"",
"KeyPress",
"KeyRelease",
"ButtonPress",
"ButtonRelease",
"MotionNotify",
"EnterNotify",
"LeaveNotify",
"FocusIn",
"FocusOut",
"KeymapNotify",
"Expose",
"GraphicsExpose",
"NoExpose",
"VisibilityNotify",
"CreateNotify",
"DestroyNotify",
"UnmapNotify",
"MapNotify",
"MapRequest",
"ReparentNotify",
"ConfigureNotify",
"ConfigureRequest",
"GravityNotify",
"ResizeRequest",
"CirculateNotify",
"CirculateRequest",
"PropertyNotify",
"SelectionClear",
"SelectionRequest",
"SelectionNotify",
"ColormapNotify",
"ClientMessage",
"MappingNotify" };


static int game_over = 0;
static int screen_no;
static Window window, window1;
static GC gc;
static Display *display;


static unsigned char *(starfont[256]) = {
/*   0 */ "",
/*   1 */ "",
/*   2 */ "",
/*   3 */ "",
/*   4 */ "",
/*   5 */ "",
/*   6 */ "",
/*   7 */ "",
/*   8 */ "",
/*   9 */ "",
/*  10 */ "",
/*  11 */ "",
/*  12 */ "",
/*  13 */ "",
/*  14 */ "",
/*  15 */ "",
/*  16 */ "",
/*  17 */ "",
/*  18 */ "",
/*  19 */ "",
/*  20 */ "",
/*  21 */ "",
/*  22 */ "",
/*  23 */ "",
/*  24 */ "",
/*  25 */ "",
/*  26 */ "",
/*  27 */ "",
/*  28 */ "",
/*  29 */ "",
/*  30 */ "",
/*  31 */ "",
/*  32 */ "",
/*  33 */ "\012\007\377\006",
/*  34 */ "\012\004\377\017\011",
/*  35 */ "\002\014\377\003\015\377\011\001\377\016\006",
/*  36 */ "\016\004\003\015\014\002\377\012\006",
/*  37 */ "\005\012\011\004\005\377\017\001\377\007\014\013\006\007",
/*  38 */ "",
/*  39 */ "\012\004",
/*  40 */ "\017\011\007\013",
/*  41 */ "\005\011\007\001",
/*  42 */ "\003\015\377\011\007\377\004\014\377\016\002",
/*  43 */ "\003\015\377\011\007",
/*  44 */ "\007\001",
/*  45 */ "\003\015",
/*  46 */ "\006\006",
/*  47 */ "\001\017",
/*  48 */ "\001\005\017\013\001\017",
/*  49 */ "\004\012\006\377\001\013",
/*  50 */ "\005\017\015\003\001\013",
/*  51 */ "\005\017\013\001\377\010\015",
/*  52 */ "\005\003\015\377\017\013",
/*  53 */ "\017\005\004\011\015\014\006\001",
/*  54 */ "\005\001\013\015\003",
/*  55 */ "\005\017\001",
/*  56 */ "\005\017\013\001\005\377\003\015",
/*  57 */ "\015\003\005\017\013",
/*  58 */ "\010\010\377\007\007",
/*  59 */ "\010\010\377\007\001",
/*  60 */ "\016\003\014",
/*  61 */ "\003\015\377\002\014",
/*  62 */ "\004\015\002",
/*  63 */ "\005\017\016\010\007\377\006\006",
/*  64 */ "\016\011\010\015\017\005\001\013",
/*  65 */ "\001\004\012\016\013\377\003\015",
/*  66 */ "\001\005\012\016\010\003\010\014\006",
/*  67 */ "\017\005\001\013",
/*  68 */ "\005\001\006\014\016\012\005",
/*  69 */ "\017\005\001\013\377\003\010",
/*  70 */ "\017\005\001\377\003\010",
/*  71 */ "\017\005\001\013\015\010",
/*  72 */ "\005\001\377\017\013\377\003\015",
/*  73 */ "\005\017\377\012\006\377\001\013",
/*  74 */ "\017\013\006\002",
/*  75 */ "\005\001\377\017\003\013",
/*  76 */ "\005\001\013",
/*  77 */ "\001\005\011\017\013",
/*  78 */ "\001\005\013\017",
/*  79 */ "\001\005\017\013\001",
/*  80 */ "\001\005\017\015\003",
/*  81 */ "\001\005\017\014\006\001\377\007\013",
/*  82 */ "\001\005\017\015\003\013",
/*  83 */ "\017\005\003\015\013\001",
/*  84 */ "\005\017\377\012\006",
/*  85 */ "\005\001\013\017",
/*  86 */ "\005\006\017",
/*  87 */ "\005\001\007\013\017",
/*  88 */ "\005\013\377\017\001",
/*  89 */ "\005\010\377\017\010\006",
/*  90 */ "\005\017\001\013",
/*  91 */ "\017\012\006\013",
/*  92 */ "\005\013",
/*  93 */ "\005\012\006\001",
/*  94 */ "\004\012\016",
/*  95 */ "\001\013",
/*  96 */ "",
/*  97 */ "\001\003\011\015\013\377\002\014",
/*  98 */ "\004\001\013\014\003",
/*  99 */ "\016\003\001\013",
/* 100 */ "\015\002\001\013\016",
/* 101 */ "\002\015\016\004\001\013",
/* 102 */ "\016\004\001\377\003\010",
/* 103 */ "\016\004\001\013\015\010",
/* 104 */ "\004\001\377\003\010\014\013",
/* 105 */ "\011\006",
/* 106 */ "\016\013\006\002",
/* 107 */ "\004\001\377\015\002\013",
/* 108 */ "\004\011\006\377\001\013",
/* 109 */ "\001\004\016\013\377\011\006",
/* 110 */ "\001\004\013\016",
/* 111 */ "\001\004\016\013\001",
/* 112 */ "\001\004\016\015\002",
/* 113 */ "\013\016\004\003\014",
/* 114 */ "\001\004\016\015\002\013",
/* 115 */ "\016\004\003\014\013\001",
/* 116 */ "\004\016\377\011\006",
/* 117 */ "\004\001\013\016",
/* 118 */ "\004\006\016",
/* 119 */ "\004\001\007\013\016",
/* 120 */ "\004\013\377\001\016",
/* 121 */ "\004\003\014\377\016\013\001",
/* 122 */ "\004\016\001\013",
/* 123 */ "\017\012\011\003\007\006\013",
/* 124 */ "\012\006",
/* 125 */ "\005\012\011\015\007\006\001",
/* 126 */ "",
/* 127 */ "\001\013\010\001",
/* 128 */ "",
/* 129 */ "",
/* 130 */ "",
/* 131 */ "",
/* 132 */ "",
/* 133 */ "",
/* 134 */ "",
/* 135 */ "",
/* 136 */ "",
/* 137 */ "",
/* 138 */ "",
/* 139 */ "",
/* 140 */ "",
/* 141 */ "",
/* 142 */ "",
/* 143 */ "",
/* 144 */ "",
/* 145 */ "",
/* 146 */ "",
/* 147 */ "",
/* 148 */ "",
/* 149 */ "",
/* 150 */ "",
/* 151 */ "",
/* 152 */ "",
/* 153 */ "",
/* 154 */ "",
/* 155 */ "",
/* 156 */ "",
/* 157 */ "",
/* 158 */ "",
/* 159 */ "",
/* 160 */ "",
/* 161 */ "",
/* 162 */ "",
/* 163 */ "",
/* 164 */ "",
/* 165 */ "",
/* 166 */ "",
/* 167 */ "",
/* 168 */ "",
/* 169 */ "",
/* 170 */ "",
/* 171 */ "",
/* 172 */ "",
/* 173 */ "",
/* 174 */ "",
/* 175 */ "",
/* 176 */ "",
/* 177 */ "",
/* 178 */ "",
/* 179 */ "",
/* 180 */ "",
/* 181 */ "",
/* 182 */ "",
/* 183 */ "",
/* 184 */ "",
/* 185 */ "",
/* 186 */ "",
/* 187 */ "",
/* 188 */ "",
/* 189 */ "",
/* 190 */ "",
/* 191 */ "",
/* 192 */ "",
/* 193 */ "",
/* 194 */ "",
/* 195 */ "",
/* 196 */ "",
/* 197 */ "",
/* 198 */ "",
/* 199 */ "",
/* 200 */ "",
/* 201 */ "",
/* 202 */ "",
/* 203 */ "",
/* 204 */ "",
/* 205 */ "",
/* 206 */ "",
/* 207 */ "",
/* 208 */ "",
/* 209 */ "",
/* 210 */ "",
/* 211 */ "",
/* 212 */ "",
/* 213 */ "",
/* 214 */ "",
/* 215 */ "",
/* 216 */ "",
/* 217 */ "",
/* 218 */ "",
/* 219 */ "",
/* 220 */ "",
/* 221 */ "",
/* 222 */ "",
/* 223 */ "",
/* 224 */ "",
/* 225 */ "",
/* 226 */ "",
/* 227 */ "",
/* 228 */ "",
/* 229 */ "",
/* 230 */ "",
/* 231 */ "",
/* 232 */ "",
/* 233 */ "",
/* 234 */ "",
/* 235 */ "",
/* 236 */ "",
/* 237 */ "",
/* 238 */ "",
/* 239 */ "",
/* 240 */ "",
/* 241 */ "",
/* 242 */ "",
/* 243 */ "",
/* 244 */ "",
/* 245 */ "",
/* 246 */ "",
/* 247 */ "",
/* 248 */ "",
/* 249 */ "",
/* 250 */ "",
/* 251 */ "",
/* 252 */ "",
/* 253 */ "",
/* 254 */ "",
/* 255 */ ""
};

static const double xycoord[16][2] = {
{0.0, 0.0},
{0.0, 0.0},
{0.0, 0.25},
{0.0, 0.5},
{0.0, 0.75},
{0.0, 1.0},
{0.5, 0.0},
{0.5, 0.25},
{0.5, 0.5},
{0.5, 0.75},
{0.5, 1.0},
{1.0, 0.0},
{1.0, 0.25},
{1.0, 0.5},
{1.0, 0.75},
{1.0, 1.0}
};


/*
 * Bugs: No guard against exceeding bounds of pts array when used with
 *     erroneously long polygon specs.
 */

int vecdraw_string(Display *display, Drawable drawable, GC gc, int xpos, int ypos, double pixwidth, double pixheight, double angle, const char *str)
{
  size_t p = 0;
  unsigned char i;
  int j, k;
  double x0, y0, dx, dy, s, c, xx, xy, yx, yy;
  const double spacing = 1.5;
  XPoint pts[100];

  x0 = xpos;
  y0 = ypos;
  s = sin(angle);
  c = cos(angle);
  dx = pixwidth * spacing * c;
  dy = -pixwidth * spacing * s;
  xx = pixwidth * c;
  xy = -pixheight * s;
  yx = -pixwidth * s;
  yy = -pixheight * c;
  while (str[p])
  {
    i = (unsigned char) str[p++];
    j = 0;
    while (starfont[i][j])
    {
      k = 0;
      while (starfont[i][j] && (starfont[i][j] < 240))
      {
        pts[k].x = x0 + xycoord[starfont[i][j]][0] * xx + xycoord[starfont[i][j]][1] * xy;
        pts[k++].y = y0 + xycoord[starfont[i][j]][0] * yx + xycoord[starfont[i][j]][1] * yy;
	j++;
      }
      if (k)
	XDrawLines(display, drawable, gc, pts, k, CoordModeOrigin);
      if (starfont[i][j])
        j++;
    }
    x0 += dx;
    y0 += dy;
  }
  return (0);
}


void redraw_test(Window win, int x, int y, unsigned int w, unsigned int h, long xpos, long ypos)
{
  int i, j;
  XWindowAttributes xwa;
  XPoint pts[10] = {{0, 0}, {0, 20}, {10, -10}, {10, 10}, {0, -20}};
  char str[33];

  printf("I redraw test window area x=%d, y=%d, w=%u, h=%u\n", x, y, w, h);
  XGetWindowAttributes(display, win, &xwa);
  if (win == window)
  {
    vecdraw_string(display, window, gc, 50 - xpos, 50 - ypos, 15, 30, 0.0, "Hallo Quappensnix");
    vecdraw_string(display, window, gc, 50 - xpos, 100 - ypos, 8, 16, 0.0, "Wenn der Knall der Schwipp mich aus den Traeumen reisst,");
    vecdraw_string(display, window, gc, 50 - xpos, 120 - ypos, 5, 10, 0.0, "Dann FAIX' ich ueber's weite Knesermeer!!");
    for (i = 0; i < 6; i++)
    {
      for (j = 0; j < 16; j++)
        str[j] = 32 + i * 16 + j;
      str[j] = '\0';
      vecdraw_string(display, window, gc, 50 - xpos, 650 - i * 40 - ypos, 15, 30, i * 0.25 * PI / 5.0, str);
/*
      XDrawLine(display, win, gc, -xpos, 699 - i * 14 - ypos, i * 16 - xpos, -ypos);
      XDrawLine(display, win, gc, i * 16 - xpos, 699 - ypos, 799 - xpos, 699 - i * 14 - ypos);
      pts[0].x = i * 30 - xpos;
      pts[0].y = i * 20 - ypos;
      XFillPolygon(display, win, gc, pts, 5, Nonconvex, CoordModePrevious);
*/
    }
  }
  if (win == window1)
  {
    for (i = 0; i < 50; i++)
    {
      XDrawLine(display, win, gc, i * 10 - xpos, - ypos, 499 - xpos, i * 6 -ypos);
      XDrawLine(display, win, gc, i * 10 - xpos, 299 - ypos, - xpos, i * 6 - ypos);
    }
  }
  printf("Slider positions are: x: %ld, y: %ld\n", xpos, ypos);
}


int close_test(Window win)
{
  printf("request to close window %ld\n", (long) win);
  if (win == window)
  {
    printf("sizeable window was closed\n");
    window = BadValue;
    return (1);
  }
  else
  {
    if (window != BadValue)
    {
      printf("sizeable window still exists -- no shutdown!\n");
      return (0);
    }
    printf("other window was closed\n");
    game_over = 1;
    return (1);
  }
}


int main(int argc, char **argv)
{
  int num_queued, w, h;
  unsigned int width, height;
  XEvent xevent;
  XGCValues gcvalues;
  const char *prgname = argv[0];
  const char *classname = "Whtest";

  display = xwin_init(NULL, prgname, classname, argc, argv, &screen_no, &width, &height);
  printf("opened display with screen #%d, w = %u, h = %u\n", screen_no, width, height);
  window = create_window(0xfff, 800, 700, redraw_test, close_test, 100, 100, 640, 400);
  XSelectInput(display, window, WH_EVENTMASK | WH_SELECTMASK);
  gcvalues.foreground = BlackPixel(display, screen_no);
  gcvalues.background = WhitePixel(display, screen_no);
  gc = XCreateGC(display, window, GCForeground | GCBackground, &gcvalues);
  printf("created window\n");
  map_window(window);
  printf("mapped window\n");
  window1 = create_window(MOVER | FULLER | CLOSER | RTARROW | HSLIDE | UPARROW | DNARROW, 500, 300, redraw_test, close_test, 100, 100, 320, 200);
  XSelectInput(display, window1, WH_EVENTMASK | WH_SELECTMASK);
  printf("created another window\n");
  map_window(window1);
  printf("mapped the other window\n");
  while (!game_over)
  {
    num_queued = XEventsQueued(display, QueuedAfterFlush);
    if (num_queued)
    {
      XNextEvent(display, &xevent);
      /* printf("received event: %s, window: %d\n", event_name[xevent.type], (int) xevent.xany.window); */
      if (process_xevent(&xevent))
	continue;
      if (xevent.type == ButtonPress)
      {
	mouse_boxselect(xevent.xbutton.window, xevent.xbutton.x, xevent.xbutton.y, &w, &h);
	if (xevent.xbutton.window == window)
	  printf("selected area x = %d, y = %d, w = %d, h = %d from window\n", xevent.xbutton.x, xevent.xbutton.y, w, h);
	else if (xevent.xbutton.window == window1)
	  printf("selected area x = %d, y = %d, w = %d, h = %d from other window\n", xevent.xbutton.x, xevent.xbutton.y, w, h);
	else
	  printf("selected area x = %d, y = %d, w = %d, h = %d from unknown window %ld\n", xevent.xbutton.x, xevent.xbutton.y, w, h, (long) xevent.xbutton.window);
      }

    }
  }
  XFreeGC(display, gc);
  return (EXIT_SUCCESS);
}

