#ifndef H_JKLIB
#define H_JKLIB

#include <stdio.h>

#ifdef MEMDEBUG
#  include "memdebug.h"
#endif


typedef struct
{
  size_t byte_no;
  unsigned short bit_no;
} HUFFMAN_BITSPEC;

typedef struct tag_htnode
{
  struct tag_htnode *n0, *n1;
  unsigned char v;
} HT_NODE;

typedef struct
{
  short num_bits;
  unsigned char c[32];
} H_CODE;

typedef struct tag_freq_listnode
{
  struct tag_freq_listnode *next;
  HT_NODE *ht_node;
  unsigned long f;
  unsigned char v;
} FREQ_LISTNODE;

typedef struct
{
  long line;
  FILE *f;
  char name[FILENAME_MAX];
} LINEFILE;


extern char *char_to_hex(char *buf, const char *source, long n);
extern long compress_histogram(long num, const short *d, short *compress);
extern long compress_histogram_long(long num, const long *d, long *compress);

extern char *ps_string(const char *s, char *buf);
extern void ps_pageheader(FILE *f, const char *text, long page_no, double page_width, double page_height, double left_margin);

extern int num_setbits(unsigned long x);
extern long max3(long p1, long p2, long p3);
extern long edit_distance(long xl, const char *x, long yl, const char *y);
extern long hex_to_char(char *buf, const char *source);
extern int hexval(char c);
extern double shannon_short(int l, const short *h);
extern double shannon_long(int l, const long *h);

extern LINEFILE *l_fopen(const char *name);
extern int l_fclose(LINEFILE *lf);
extern char *l_fgets(char *s, size_t l, LINEFILE *lf);
extern char *l_fgets0(char *s, size_t l, LINEFILE *lf);
extern void l_errmsg(LINEFILE *lf, const char *msg);
extern void l_rewind(LINEFILE *lf);
extern FILE *l_fpointer(LINEFILE *lf);
#define l_feof(lf) feof((lf)->f)
#define l_ferror(lf) ferror((lf)->f)

extern int fwrite_int16array(const short *x, size_t num, FILE *f);
extern int fwrite_int32array(const long *x, size_t num, FILE *f);
extern long read_int16(FILE *f);
extern long read_int32(FILE *f);

extern double log2(double x);

extern size_t huffman_encode_8bit(size_t length, const unsigned char *s, unsigned char *h);
extern size_t huffman_decode_8bit(const unsigned char *h, unsigned char *buf);

#endif


