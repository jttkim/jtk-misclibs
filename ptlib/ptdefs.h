#ifndef H_PHYLTDEF
#define H_PHYLTDEF

#include <stdio.h>  /* for NULL */

#define PHYL_MAX_NODENAMELENGTH  1023
#define PHYL_MAX_SLEN          100000
#define PHYL_PRINTNAME_LENGTH     256
#define PHYL_SPECNAME_LENGTH      256

#define PHYLERR_MEM                -1
#define PHYLERR_NOLEAVES           -2
#define PHYLERR_DIFFNUMLEAVES      -3
#define PHYLERR_INCOMPATLEAVES     -4
#define PHYLERR_PSBOXTOOSMALL      -5
#define PHYLERR_FILE               -6
#define PHYLERR_PARSE              -7
#define PHYLERR_NOCOMMONANCESTOR   -8
#define PHYLERR_NOTINTREE          -9

#define PHYLEDGINF_NONE             0
#define PHYLEDGINF_DIFFEDGE         1
#define PHYLEDGINF_IDEDGE           2

#define PHYL_LEAVES_NONE            0
#define PHYL_LEAVES_HORIZONTAL      1
#define PHYL_LEAVES_RADIAL          2

#define MAX_LINELENGTH            256

#define DEFAULT_LEAFATTRIBUTES { NULL, "", "", "", "", 0.0, 0.0, 0.0, -1.0, -1.0, -1.0 }

#endif
