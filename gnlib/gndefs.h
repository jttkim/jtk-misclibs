#ifndef H_GN_DEFS
#define H_GN_DEFS


#ifdef __cplusplus
extern "C" {
#endif

#define GN_MAX_NODENAME_LENGTH  20
#define GN_MAX_LINELENGTH      512

#define GN_STILL_ALIVE      -1

#define GNERR_MEM            -1
#define GNERR_UNKNODE        -2
#define GNERR_NEGLSPAN       -3
#define GNERR_DUPLICDEATH    -4
#define GNERR_NOTINMATRIX    -5
#define GNERR_NODESLOST      -6
#define GNERR_DUPLICID       -7
#define GNERR_PARSE          -8
#define GNERR_FILE           -9
#define GNERR_CORRUPTFILE   -10
#define GNERR_IDGENERATION  -11
#define GNERR_CORRUPTTREE   -12
#define GNERR_NOTEMPTY      -13
#define GNERR_BADGENERATION -14

#define GN_NODE_BORN         0
#define GN_NODE_DEAD         1
#define GN_UNSPEC_ID         2

#define GN_PRUNE_NEVER       1
#define GN_PRUNE_DEAD        2
#define GN_PRUNE_MONOLINKS   3

#ifdef __cplusplus
}
#endif


#endif

