#ifndef H_PTLIBIN
#define H_PTLIBIN

#ifdef MEMDEBUG
#  include "memdebug.h"
#endif

#define free0(p) free(p); (p) = NULL

#include "ptdefs.h"
#include "pttypes.h"
#include "ptlib.h"

#ifndef PI
#  define PI 3.1415927
#endif

#endif

