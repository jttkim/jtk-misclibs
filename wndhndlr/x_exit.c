#include "wndincl.h"


/*
 * close the virtual workstation opened by gem_init(), and call
 * appl_exit() to orderly terminate an AES application.
 */

void gem_exit(void)
{
#ifdef WND_DEBUG
  wnd_report("*** gem_exit()");
  if (wdebug_report != NULL)
  {
    fclose(wdebug_report);
  }
#endif
  v_clsvwk(_wndh_gl.vwk_screen);
  appl_exit();
}

