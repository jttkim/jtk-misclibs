#include "wndhndlr.h"
#include "wndincl.h"

/*
 * process an AES message. The message must indicate some window
 * operation. Menu or accessory messages cannot be processed, they
 * must be handled by other parts of the application code.
 */

void process_message(const int mbuf[8])
{
  switch (mbuf[0])
  {
  case WM_REDRAW:
    /* printf("redrawing window %d\n", mbuf[3]); */
    w_redraw(mbuf[3], mbuf[4], mbuf[5], mbuf[6], mbuf[7]);
    break;
  case WM_TOPPED:
  case WM_NEWTOP:
    /* printf("topping window %d\n", mbuf[3]); */
#ifdef WND_DEBUG
    wnd_report("wind_set");
#endif
    wind_set(mbuf[3], WF_TOP);
    break;
  case WM_CLOSED:
    /* printf("closing window %d\n", mbuf[3]); */
    remove_window(mbuf[3]);
    break;
  case WM_FULLED:
    /* printf("fulling window %d\n", mbuf[3]); */
    w_fulled(mbuf[3]);
    break;
  case WM_ARROWED:
    /* printf("arrowing window %d\n", mbuf[3]); */
    w_arrow(mbuf[3], mbuf[4]);
    break;
  case WM_HSLID:
    /* printf("hsliding window %d\n", mbuf[3]); */
    w_hslid(mbuf[3], mbuf[4]);
    break;
  case WM_VSLID:
    /* printf("vsliding window %d\n", mbuf[3]); */
    w_vslid(mbuf[3], mbuf[4]);
    break;
  case WM_SIZED:
    /* printf("sizing window %d\n", mbuf[3]); */
    w_sized(mbuf[3], mbuf[4], mbuf[5], mbuf[6], mbuf[7]);
    break;
  case WM_MOVED:
    /* printf("moving window %d\n", mbuf[3]); */
    w_moved(mbuf[3], mbuf[4], mbuf[5], mbuf[6], mbuf[7]);
    break;
  case WM_UNTOPPED:
  default:
    ;
  }
  /* printf("finished processing message\n"); */
}

