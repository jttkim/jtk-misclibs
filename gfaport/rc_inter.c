/* equivalent to RC_INTERSECT. Named rc_inter() because there already is some
   rc_intersect() in GEMFAST.H, but that one uses some silly structs... */

int rc_inter(int x0, int y0, int w0, int h0, int *x, int *y, int *w, int *h)
{
  int mx, my;

  mx = (x0 + w0 < *x + *w) ? x0 + w0 : *x + *w;
  my = (y0 + h0 < *y + *h) ? y0 + h0 : *y + *h;
  if (x0 > *x)
    *x = x0;
  if (y0 > *y)
    *y = y0;
  *w = mx - (*x);
  *h = my - (*y);
  return ((*w > 0) && (*h > 0));
}

