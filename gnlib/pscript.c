#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gnlibin.h"


static int value_from_genome(const char *genome, const char *label, double *v)
{
  char *s, *s1;
  double x;

  s = strstr(genome, label);
  if (s == NULL)
    return (-1);
  s += strlen(label);
  x = strtod(s, &s1);
  if (s == s1)
    return (-1);
  *v = x;
  /* fprintf(stderr, "value_from_genome: found %s = %f in \"%s\"\n", label, x, genome); */
  return (0);
}


static int cmp_branch_generation(const void *p1, const void *p2)
{
  long g1 = ((GN_NODE *) p1)->branch_generation;
  long g2 = ((GN_NODE *) p2)->branch_generation;

  if (g1 < g2)
    return (-1);
  else if (g1 > g2)
    return (1);
  else
    return (0);
}


static int postscript_subtree(const GN_NODE *gn, FILE *f, long generation, const char *label, double x0, double y0, double x_node_distance, double yscale, double circle_radius, double gray0, double gray_scale)
{
  long i, j, k, n;
  long nb_alive, n_remaining;
  GN_NODE **nodelist;
  double circle_x, circle_y, line_x, line_y, x0_offspring, y_offspring, xpos_right;
  double gray;
  int return_code;
  char nodename_buf[256];

  gn_sprint_nodename(nodename_buf, gn);
  /* fprintf(f, "%% node %s: starting, x0 = %f, y0 = %f, %ld alive nodes\n", nodename_buf, x0, y0, gn->num_alive); */
  circle_x = x0 + (gn->num_alive - 1) * x_node_distance * 0.5;
  circle_y = y0 + gn->branch_generation * yscale;
  line_x = circle_x;
  line_y = y0 + gn->branch_generation * yscale;
  if (gn->num_alive == 0)
  {
    fprintf(f, "%% node %s is in a dead branch -- ignored\n", nodename_buf);
    return (0);
  }
  if ((gn->num_offspring == 1) && (gn->lifespan != GN_STILL_ALIVE))
  {
    /* fprintf(f, "%% %s is a dead monolink\n", nodename_buf); */
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if (gn->offspring[i])
        break;
    }
    if (i == gn->offspring_arrsize)
      return (GNERR_CORRUPTTREE);
    y_offspring = y0 + gn->offspring[i]->branch_generation * yscale;
    /* fprintf(f, "%% line from offspring to dead monolink %s\n", nodename_buf); */
    fprintf(f, "%1.8f %1.8f %1.8f V\n", y_offspring - line_y, line_x, line_y);
    line_y = y_offspring;
    return_code = postscript_subtree(gn->offspring[i], f, generation, label, x0, y0, x_node_distance, yscale, circle_radius, gray0, gray_scale);
    if (return_code)
      return (return_code);
  }
  else if (gn->num_offspring > 0)
  {
    nodelist = (GN_NODE **) malloc(gn->num_offspring * sizeof(GN_NODE *));
    if (nodelist == NULL)
    {
      fprintf(stderr, "postscript_subtree: malloc failed for %ld node pointers (node %s)\n", gn->num_offspring, nodename_buf);
      return (GNERR_MEM);
    }
    n = 0;
    for (i = 0; i < gn->offspring_arrsize; i++)
    {
      if (gn->offspring[i])
      {
        if (n == gn->num_offspring)
        {
          fprintf(stderr, "postscript_subtree: more offspring than num_offspring (%ld)\n", gn->num_offspring);
          free(nodelist);
          return (GNERR_CORRUPTTREE);
        }
        nodelist[n++] = gn->offspring[i];
      }
    }
    if (n != gn->num_offspring)
    {
      fprintf(stderr, "postscript_subtree: less offspring than num_offspring (%ld, n = %ld)\n", gn->num_offspring, n);
      free(nodelist);
      return (GNERR_CORRUPTTREE);
    }
    /* qsort(nodelist, n, sizeof(GN_TREE *), cmp_branch_generation); */
    {
      GN_NODE *tmp_g;
      long tmp_min, tmp_i;
      for (i = 0; i < n - 1; i++)
      {
        tmp_i = i;
        tmp_min = nodelist[i]->branch_generation;
        for (j = i + 1; j < n; j++)
        {
          if (tmp_min > nodelist[j]->branch_generation)
          {
            tmp_min = nodelist[j]->branch_generation;
            tmp_i = j;
          }
        }
        tmp_g = nodelist[i];
        nodelist[i] = nodelist[tmp_i];
        nodelist[tmp_i] = tmp_g;
      }
    }
    x0_offspring = x0 + (gn->num_alive - 1) * x_node_distance;
    n_remaining = gn->num_alive;
    for (i = 0; i < n; i = j)
    {
      j = i;
      nb_alive = 0;
      while ((j < n) && (nodelist[i]->branch_generation == nodelist[j]->branch_generation))
      {
        nb_alive += nodelist[j]->num_alive;
        /* fprintf(f, "%% node %s: offspring %ld (%05ld-%04ld has %ld alive nodes ->nb_alive = %ld\n", */
                /* nodename_buf, j, nodelist[j]->node_id.generation, nodelist[j]->node_id.id, */
                /* nodelist[j]->num_alive, nb_alive); */
        j++;
      }
      /* if (j == (n - 1)) */
        /* j++; */
      /* fprintf(f, "%%node %s: n_remaining is: %ld ...", nodename_buf, n_remaining); */
      n_remaining -= nb_alive;
      /* fprintf(f, "%% now set to %ld\n", n_remaining); */
      y_offspring = y0 + nodelist[i]->branch_generation * yscale;
      fprintf(f, "%1.8f %1.8f %1.8f V\n", y_offspring - line_y, line_x, line_y);
      line_x = x0 + (n_remaining - 1) * x_node_distance * 0.5;
      line_y = y_offspring;
      if (n_remaining)
      {
        xpos_right = x0_offspring - (nodelist[i]->num_alive - 1) * x_node_distance * 0.5;
        /* fprintf(f, "%% horizontal line %f -> %f\n", line_x, xpos_right); */
        /* fprintf(f, "gsave 1 0 0 setrgbcolor %f %f moveto (%s) dup stringwidth pop neg 0 rmoveto show grestore\n", line_x - 2 * circle_radius, y_offspring, nodename_buf); */
        fprintf(f, "%1.8f %1.8f %1.8f H\n", xpos_right - line_x, line_x, y_offspring);
      }
      for (k = i; k < j; k++)
      {
        x0_offspring -= (nodelist[k]->num_alive - 1) * x_node_distance;
        return_code = postscript_subtree(nodelist[k], f, generation, label, x0_offspring, y0, x_node_distance, yscale, circle_radius, gray0, gray_scale);
        if (return_code)
        {
          free(nodelist);
          return (return_code);
        }
        x0_offspring -= x_node_distance;
      }
    }
    /* fprintf(f, "%% after loop (node %s): n_remaining = %ld\n", nodename_buf, n_remaining); */
    free(nodelist);
  }
  gn_sprint_nodename(nodename_buf, gn);
  /* fprintf(f, "%% node %s: finishing up\n", nodename_buf); */
  if (gn->lifespan == GN_STILL_ALIVE)
  {
    fprintf(f, "%1.8f %1.8f %1.8f V\n",
            y0 + generation * yscale - line_y, line_x, line_y);
    fprintf(f, "( %s) %1.8f %1.8f S\n", nodename_buf, line_x, y0 + generation * yscale);
  }
  gray = 0.0;
  if (label)
  {
    if (value_from_genome(gn->genome, label, &gray) == 0)
    {
      /* fprintf(f, "%% node %s has value %f\n", nodename_buf, gray); */
      gray = (gray - gray0) / gray_scale;
      /* fprintf(f, "%% node %s has gray value %f\n", nodename_buf, gray); */
    }
    else
      gray = 0.0;
  }
  if (gray < 0.0)
    gray = 0.0;
  if (gray > 1.0)
    gray = 1.0;
  /* fprintf(f, "gsave %1.8f setlinewidth %1.8f %1.8f %1.8f 0 360 arc gsave %1.8f setgray fill grestore stroke grestore\n", */
          /* circle_radius * 0.1, circle_x, circle_y, circle_radius, gray); */
  fprintf(f, "%1.8f %1.8f %1.8f %1.8f C\n", gray, circle_x, circle_y, circle_radius);
  /* fprintf(f, "gsave 0.3 1 0 setrgbcolor %f %f moveto (%s) show grestore\n", */
          /* circle_x + 2 * circle_radius, circle_y - circle_radius, nodename_buf); */
  return (0);
}


static int find_minmax(const GN_NODE *gn, const char *label, double *minval, double *maxval)
{
  double v;
  long i;
  int return_value;

  if (gn->genome)
  {
    if (value_from_genome(gn->genome, label, &v) == 0)
    {
      if (v < *minval)
      {
        *minval = v;
        fprintf(stderr, "find_minmax: new min (%s) is %f\n", label, v);
      }
      if (v > *maxval)
      {
        *maxval = v;
        fprintf(stderr, "find_minmax: new max (%s) is %f\n", label, v);
      }
    }
  }
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
    {
      return_value = find_minmax(gn->offspring[i], label, minval, maxval);
      if (return_value)
        return (return_value);
    }
  }
  return (0);
}


static int find_max_namelength(const GN_NODE *gn)
{
  static char buf[256];
  int m, ms;
  long i;

  gn_sprint_nodename(buf, gn);
  m = strlen(buf);
  for (i = 0; i < gn->offspring_arrsize; i++)
  {
    if (gn->offspring[i])
    {
      ms = find_max_namelength(gn->offspring[i]);
      if (ms > m)
        m = ms;
    }
  }
  return (m);
}


int gn_fprint_postscript(const GN_TREE *gtree, FILE *f, long generation, double width, double height, double fontheight, const char *label)
{
  double minval, maxval, x0, y0, x_node_distance, yscale, circle_radius, charwidth;
  double graybox_x0, graybox_width, graybox_step, x;
  char minval_buf[256], maxval_buf[256];
  GN_NODELIST_ELEMENT *gl;
  int max_namelength, m1;

  charwidth = 0.7 * fontheight;   /* a hack, but this works reasonably for Courier etc. */
  max_namelength = 0;
  for (gl = gtree->root_list; gl; gl = gl->next)
  {
    m1 = find_max_namelength(gl->gnode);
    if (m1 > max_namelength)
      max_namelength = m1;
  }
  fprintf(stderr, "fprint_postscript: max_namelength = %d\n", max_namelength);
  fprintf(f, "save\n");
  fprintf(f, "/H\n");
  fprintf(f, "{ moveto 0 rlineto stroke } bind def\n");
  fprintf(f, "/V\n");
  fprintf(f, "{ moveto 0 exch rlineto stroke } bind def\n");
  fprintf(f, "/S\n");
  fprintf(f, "{ gsave moveto 90 rotate show grestore } bind def\n");
  fprintf(f, "/C\n");
  fprintf(f, "{\n");
  fprintf(f, "  gsave\n");
  fprintf(f, "  dup 0.1 mul setlinewidth\n");
  fprintf(f, "  0 360 arc gsave setgray fill grestore stroke\n");
  fprintf(f, "  grestore\n");
  fprintf(f, "} bind def\n");
  /* fprintf(f, "gsave 1 0 0 setrgbcolor 0 0 moveto %f 0 rlineto 0 %f rlineto %f 0 rlineto closepath stroke grestore\n", width, height, -width); */
  minval = DBL_MAX;
  maxval = -minval;
  /* fprintf(stderr, "fprint_postscript: before find_minmax: minval = %f, maxval = %f\n", minval, maxval); */
  if (label)
  {
    for (gl = gtree->root_list; gl; gl = gl->next)
      find_minmax(gl->gnode, label, &minval, &maxval);
    if (maxval == minval)
    {
      label = NULL;
      fprintf(f, "0 0 moveto (no labelling: minval = %f, maxval = %f) show\n", minval, maxval);
    }
    else
    {
      fprintf(stderr, "fprint_postscript: minval = %f, maxval = %f\n", minval, maxval);
      yscale = (height - (max_namelength + 1) * charwidth - 1.5 * fontheight) / (generation + 1);
      y0 = yscale + 1.5 * fontheight;
      sprintf(minval_buf, "%f", minval);
      sprintf(maxval_buf, "%f", maxval);
      graybox_width = width - (strlen(label) + strlen(minval_buf) + strlen(maxval_buf) + 3) * charwidth;
      graybox_x0 = (strlen(label) + strlen(minval_buf) + 2) * charwidth;
      if (graybox_width > 0.0)
      {
        graybox_step = graybox_width / 256;
        x = graybox_x0 + graybox_step * 0.5;
        fprintf(f, "save gsave %1.8f setlinewidth\n", graybox_width / 256);
        fprintf(f, "/grayline\n");
        fprintf(f, "{\n");
        fprintf(f, "  dup\n");
        fprintf(f, "  255.0 div setgray\n");
        fprintf(f, "  %1.8f mul %1.8f add 0 moveto 0 %1.8f rlineto stroke\n",
		graybox_step, graybox_x0, fontheight);
        fprintf(f, "} bind def\n");
        fprintf(f, "0 1 255 {grayline} for\n");
        /* for (i = 0; i < 256; i++) */
        /* { */
          /* fprintf(f, "%1.8f setgray %1.8f 0 moveto 0 %1.8f rlineto stroke\n" */
                  /* , i / 256.0, x, fontheight); */
          /* x += graybox_step; */
        /* } */
        fprintf(f, "grestore restore\n");
        fprintf(f, "%1.8f 0 moveto %1.8f 0 rlineto 0 %1.8f rlineto %1.8f 0 rlineto closepath stroke\n",
                graybox_x0, graybox_width, fontheight, -graybox_width);
        fprintf(f, "%1.8f 0 moveto (%s) dup stringwidth pop neg 0 rmoveto show\n",
                graybox_x0 - charwidth, minval_buf);
        fprintf(f, "%1.8f 0 moveto (%s) show\n", graybox_x0 + graybox_width + charwidth, maxval_buf);
        fprintf(f, "0 0 moveto (%s) show\n", label);
      }
    }
  }
  else
  {
    yscale = (height - (max_namelength + 1) * charwidth) / (generation + 1);
    y0 = yscale;
  }
  x_node_distance = (width - fontheight) / (gtree->num_alive - 1);
  x0 = fontheight;
  circle_radius = x_node_distance * 0.2;
  if (circle_radius > yscale * 0.4)
    circle_radius = yscale * 0.4;
  if (circle_radius < height * 0.005)
    circle_radius = height * 0.005;
  max_namelength = 0;
  for (gl = gtree->root_list; gl; gl = gl->next)
  {
    postscript_subtree(gl->gnode, f, generation, label, x0, y0, x_node_distance, yscale, circle_radius, minval, maxval - minval);
    x0 += gl->gnode->num_alive * x_node_distance;
  }
  fprintf(f, "restore\n");
  return (0);
}

