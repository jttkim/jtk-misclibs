#include "wndincl.h"

/* gem_init does the appl_init() call and opens a virtual workstation
   on the screen. If either of these fail, the program exits with a
   message. Return value is the AES application ID. The AES's sreenhandle
   is returned in *aes_screenhandle, the screenhandle of the virtual screen
   workstation opened is returned in *screenhandle. *px, *py, *pw and *ph contain
   the size of the desktop window after a successful call. work_in and work_out
   are the  */

int gem_init(int *px, int *py, int *pw, int *ph, int *aes_screenhandle, int *screenhandle, int work_in[], int work_out[])
{
  int ap_id;
  int wc, hc, wb, hb;

  ap_id = appl_init();
  if (ap_id == -1)
  {
    fprintf(stderr, "Error initialising AES process\n");
    getchar();
    exit(1);
  }
#ifdef WND_DEBUG
  wdebug_report = fopen("wndhndlr.dbg", "w");
  sprintf(wdbg_tmpbuf, "wind_set(%1d, WF_NEWDESK, p%p, %d)", 0, (void *) NULL, 0);
  wnd_report(wdbg_tmpbuf);
#endif
  /* wind_set(0, WF_NEWDESK, NULL, 0); */
#ifdef WND_DEBUG
  wnd_report("wind_get");
#endif
  wind_get(0, WF_WORKXYWH, px, py, pw, ph);
  _wndh_gl.dw_x = *px;
  _wndh_gl.dw_y = *py;
  _wndh_gl.dw_w = *pw;
  _wndh_gl.dw_h = *ph;
  *aes_screenhandle = graf_handle(&wc, &hc, &wb, &hb);
  _wndh_gl.aes_screen = *aes_screenhandle;
  work_in[0] = 1;         /* current screen driver */
  work_in[1] = LT_SOLID;  /* line type */
  work_in[2] = BLACK;     /* line color */
  work_in[3] = MT_DOT;    /* marker type */
  work_in[4] = BLACK;     /* marker color */
  work_in[5] = 1;         /* font number (?) */
  work_in[6] = BLACK;     /* text color */
  work_in[7] = FIS_SOLID; /* fill pattern */
  work_in[8] = 1;         /* fill pattern index */
  work_in[9] = BLACK;     /* fill color */
  work_in[10] = 2;        /* RC coordinate system */
  *screenhandle = *aes_screenhandle;
  v_opnvwk(work_in, screenhandle, work_out);
  if (*screenhandle == 0)
  {
    fprintf(stderr, "Error initialising VDI screen workstation\n");
    getchar();
    exit(1);
  }
  _wndh_gl.vwk_screen = *screenhandle;
  graf_mouse(ARROW, NULL);
  return (ap_id);
}

