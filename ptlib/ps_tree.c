#include <errno.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "jklib.h"

#include "ptlibin.h"


static char buf[PHYL_MAX_SLEN];


/*
 * Set lengths such that tree is ultrametric and perfectly
 * fits perfectly into a V shape such that node levels are
 * equidistant in height.
 */

static void phyl_vshape_lengths(PHYLTREE_NODE *node, long ancestor_level)
{
  long i, level;

  level = phyl_num_leaves(node);
  node->length = ancestor_level - level;
  for (i = 0; i < node->num_descendants; i++)
    phyl_vshape_lengths(node->descendant[i], level);
}


static void set_lengths(PHYLTREE_NODE *node, double length)
{
  long i;

  node->length = length;
  for (i = 0; i < node->num_descendants; i++)
    set_lengths(node->descendant[i], length);
}


static int ps_printleaf(FILE *f, const char *nodename, const PHYL_LEAFATTRIBUTE *attrlist, int shift_flag)
{
  const PHYL_LEAFATTRIBUTE *a, da = DEFAULT_LEAFATTRIBUTES;
  char leafname_ps[PHYL_PRINTNAME_LENGTH * 2 + 1], specname_ps[PHYL_SPECNAME_LENGTH * 2 + 1] = "";

  a = attrlist;
  while (a)
  {
    if (!strcmp(nodename, a->name))
      break;
    a = a->next;
  }
  if (a == NULL)
    a = &da;
  if (a->printname[0])
    ps_string(a->printname, leafname_ps);
  else
    ps_string(nodename, leafname_ps);
  ps_string(a->species, specname_ps);
  fprintf(f, "%f %f %f %f %f %f %d %s %s specshow\n", a->red, a->green, a->blue, a->box_red, a->box_green, a->box_blue, shift_flag, leafname_ps, specname_ps);
  return (0);
}


/*
 * draw a node in a V-shaped tree
 */

static void draw_vnode_ps(FILE *f, const PHYLTREE_NODE *node, double xpos, double ypos,
        double x0, double length_scale, double node_distance, double txt_yoffset, double fontheight,
	const PHYL_PSINFO *psinfo)
{
  long i, nl_descendant;
  double xpos_descendant, ypos_descendant;

  if (psinfo->dotted_nodes)
    fprintf(f, "newpath %f %f %f 0 360 arc gsave fill grestore stroke\n", xpos, ypos, fontheight * 0.3);
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    xpos_descendant = x0 + nl_descendant * node_distance * 0.5;
    ypos_descendant = ypos + length_scale * node->descendant[i]->length;
    if (node->descendant[i]->thick >= 0.0)
    {
      fprintf(f, "1 setlinecap %f setlinewidth\n", node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto\n",
	      xpos, ypos, xpos_descendant, ypos_descendant);
      fprintf(f, "stroke\n");
    }
    else
    {
      fprintf(f, "gsave 1 0 0 setrgbcolor\n");
      fprintf(f, "1 setlinecap %f setlinewidth\n", -node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto\n",
	      xpos, ypos, xpos_descendant, ypos_descendant);
      fprintf(f, "stroke grestore\n");
    }
    draw_vnode_ps(f, node->descendant[i], xpos_descendant, ypos_descendant,
            x0, length_scale, node_distance, txt_yoffset, fontheight, psinfo);
    x0 += nl_descendant * node_distance;
  }
  if (psinfo->print_leafnames && node->name[0])
  {
    fprintf(f, "/xpos_%s %f def\n", node->name, xpos);
    fprintf(f, "gsave %f %f moveto 90 rotate\n", xpos, ypos + txt_yoffset);
    ps_printleaf(f, node->name, psinfo->attrlist, 0);
    fprintf(f, "grestore\n");
  }
}


/*
 * Draw a node in a rectanglar dendrogram-like tree
 */

static void draw_rnode_ps(FILE *f, const PHYLTREE_NODE *node, double xpos, double ypos,
        double x0, double length_scale, double node_distance, double txt_yoffset, double fontheight,
	double ymax, const PHYL_PSINFO *psinfo)
{
  long i, nl_descendant;
  double xpos_descendant = 0.0, ypos_descendant, x_left = 0.0; /* Initializations to prevent annoying warnings */
  size_t bootstrap_stringlength;
  double bootstrap_value, ym;
  char bootstrap_string[50];

  if (psinfo->dotted_nodes)
    fprintf(f, "newpath %f %f %f 0 360 arc gsave fill grestore stroke\n", xpos, ypos, fontheight * 0.3);
  if (node ->num_descendants)
    x_left = x0 + phyl_num_leaves(node->descendant[0]) * node_distance * 0.5;
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    xpos_descendant = x0 + nl_descendant * node_distance * 0.5;
    ypos_descendant = ypos + length_scale * node->descendant[i]->length;
    if (ypos_descendant > ypos)
      ym = ypos_descendant;
    else
      ym = ypos;
    if (node->descendant[i]->thick >= 0.0)
    {
      fprintf(f, "0 setlinecap %f setlinewidth\n", node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto stroke\n",
	      xpos_descendant, ypos, xpos_descendant, ypos_descendant);
    }
    else
    {
      fprintf(f, "gsave 1 0 0 setrgbcolor\n");
      fprintf(f, "0 setlinecap %f setlinewidth\n", -node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto stroke grestore\n",
	      xpos_descendant, ypos, xpos_descendant, ypos_descendant);
    }
    if (psinfo->bootstrap_numtrees)
    {
      bootstrap_value = ((double) node->descendant[i]->edge_counter) / ((double) psinfo->bootstrap_numtrees);
      sprintf(bootstrap_string, "%1.3f", bootstrap_value);
      bootstrap_stringlength = strlen(bootstrap_string);
      ps_string(bootstrap_string, buf);
      fprintf(f, "gsave bsfont setfont\n");
      fprintf(f, "newpath %f %f moveto\n", xpos_descendant, (ypos + ypos_descendant) * 0.5);
      fprintf(f, "90 rotate 0 %f rmoveto\n", 0.3 * fontheight);
      fprintf(f, "%s dup stringwidth pop -0.5 mul 0 rmoveto show\n", buf);
      fprintf(f, "grestore\n");
    }
    if (psinfo->print_edgelengths)
    {
      sprintf(bootstrap_string, "%1.3f", node->descendant[i]->length);
      ps_string(bootstrap_string, buf);
      fprintf(f, "gsave leaffont setfont\n");
      fprintf(f, "newpath %f %f moveto\n", xpos_descendant, (ypos + ypos_descendant) * 0.5);
      fprintf(f, "90 rotate 0 %f rmoveto\n", -0.7 * fontheight);
      fprintf(f, "%s dup stringwidth pop -0.5 mul 0 rmoveto show\n", buf);
      fprintf(f, "grestore\n");
    }
    draw_rnode_ps(f, node->descendant[i], xpos_descendant, ypos_descendant,
            x0, length_scale, node_distance, txt_yoffset, fontheight, ym, psinfo);
    x0 += nl_descendant * node_distance;
  }
  if (node->num_descendants)
  {
    fprintf(f, "0 setlinecap %f setlinewidth\n", psinfo->linewidth);
    fprintf(f, "newpath %f %f moveto %f %f lineto stroke\n", x_left, ypos, xpos_descendant, ypos);
    if (psinfo->print_internal_names && node->name[0])
    {
      fprintf(f, "/xpos_%s %f def\n", node->name, xpos);
      fprintf(f, "gsave %f %f moveto 90 rotate\n", xpos, ypos + txt_yoffset);
      ps_printleaf(f, node->name, psinfo->attrlist, 0);
      fprintf(f, "grestore\n");
    }
  }
  else if (psinfo->print_leafnames && node->name[0])
  {
    fprintf(f, "/xpos_%s %f def\n", node->name, xpos);
    if (psinfo->leafnames_at_max)
      fprintf(f, "gsave %f %f moveto 90 rotate\n", xpos, ymax + txt_yoffset);
    else
      fprintf(f, "gsave %f %f moveto 90 rotate\n", xpos, ypos + txt_yoffset);
    ps_printleaf(f, node->name, psinfo->attrlist, 0);
    fprintf(f, "grestore\n");
  }
}


#define UNODE_LEFTBRANCH -1
#define UNODE_MIDDLEBRANCH 0
#define UNODE_RIGHTBRANCH 1


static void find_unode_extrema(const PHYLTREE_NODE *node, double xpos, double ypos,
        double angle, double angle_range, double angle_min, double angle_limit,
        double *xmin_x, double *xmin_y, double *ymin_x, double *ymin_y,
        double *xmax_x, double *xmax_y, double *ymax_x, double *ymax_y)
{
  long i, nl_descendant, num_leaves;
  double xpos_descendant, ypos_descendant, angle_descendant, ar_descendant;

  angle = fmod(angle + 2 * PI, 2 * PI);
  num_leaves = phyl_num_leaves(node);
  if ((angle_range < angle_limit) && ((angle_range / ((double) num_leaves)) < angle_min))
  {
    /* printf("num_leaves  = %2ld: enlarged angle from %f ", num_leaves, angle_range); */
    angle_range = (2 * PI - angle_min) * 0.5 / PI * angle_range + angle_min;
    /* angle_range = (2 * PI - angle_min) * 0.25 / PI / PI * angle_range * angle_range + angle_min; */
    /* angle_range = angle_min * num_leaves; */
    /* printf("to %f\n", angle_range); */
  }
/*
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    ar_descendant = angle_range * nl_descendant / ((double) num_leaves);
    if (ar_descendant < angle_min)
      ar += angle_min - ar_descendant;
  }
*/
  angle_descendant = fmod(angle - angle_range * 0.5 + 2 * PI, 2 * PI);
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    ar_descendant = angle_range * nl_descendant / ((double) num_leaves);
/*
    if (ar_descendant < angle_min)
      ard = angle_min;
    else
      ard = ar_descendant;
*/
    angle_descendant = fmod(angle_descendant + ar_descendant * 0.5, 2 * PI);
    xpos_descendant = xpos + node->descendant[i]->length * cos(angle_descendant);
    ypos_descendant = ypos + node->descendant[i]->length * sin(angle_descendant);
    if (xpos_descendant < *xmin_x)
    {
      *xmin_x = xpos_descendant;
      *xmin_y = ypos_descendant;
    }
    if (ypos_descendant < *ymin_y)
    {
      *ymin_x = xpos_descendant;
      *ymin_y = ypos_descendant;
    }
    if (xpos_descendant > *xmax_x)
    {
      *xmax_x = xpos_descendant;
      *xmax_y = ypos_descendant;
    }
    if (ypos_descendant > *ymax_y)
    {
      *ymax_x = xpos_descendant;
      *ymax_y = ypos_descendant;
    }
    find_unode_extrema(node->descendant[i], xpos_descendant, ypos_descendant,
            angle_descendant, ar_descendant, angle_min, angle_limit,
            xmin_x, xmin_y, ymin_x, ymin_y, xmax_x, xmax_y, ymax_x, ymax_y);
    angle_descendant = fmod(angle_descendant + ar_descendant * 0.5, 2 * PI);
  }
}


/*
 * Draw a node in an unrooted tree
 */

static void draw_unode_ps(FILE *f, const PHYLTREE_NODE *node, double xpos, double ypos,
        double length_scale, double angle, double angle_range, double angle_min, double angle_limit,
        double fontheight, const PHYL_PSINFO *psinfo)
{
  long i, nl_descendant, num_leaves;
  double xpos_descendant, ypos_descendant, angle_descendant, ar_descendant;
  size_t bootstrap_stringlength;
  double bootstrap_value;
  char bootstrap_string[50];

  if (psinfo->dotted_nodes)
    fprintf(f, "newpath %f %f %f 0 360 arc gsave fill grestore stroke\n", xpos, ypos, fontheight * 0.3);
  angle = fmod(angle + 2 * PI, 2 * PI);
  num_leaves = phyl_num_leaves(node);
  if ((angle_range < angle_limit) && ((angle_range / ((double) num_leaves)) < angle_min))
  {
    /* printf("num_leaves  = %2ld: enlarged angle from %f ", num_leaves, angle_range); */
     angle_range = (2 * PI - angle_min) * 0.5 / PI * angle_range + angle_min;
    /* angle_range = (2 * PI - angle_min) * 0.25 / PI / PI * angle_range * angle_range + angle_min; */
    /* angle_range = angle_min * num_leaves; */
    /* printf("to %f\n", angle_range); */
  }
/*
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    ar_descendant = angle_range * nl_descendant / ((double) num_leaves);
    if (ar_descendant < angle_min)
      ar += angle_min - ar_descendant;
  }
*/
  angle_descendant = fmod(angle - angle_range * 0.5 + 2 * PI, 2 * PI);
  for (i = 0; i < node->num_descendants; i++)
  {
    nl_descendant = phyl_num_leaves(node->descendant[i]);
    ar_descendant = angle_range * nl_descendant / ((double) num_leaves);
/*
    if (ar_descendant < angle_min)
      ard = angle_min;
    else
      ard = ar_descendant;
*/
    angle_descendant = fmod(angle_descendant + ar_descendant * 0.5, 2 * PI);
    xpos_descendant = xpos + length_scale * node->descendant[i]->length * cos(angle_descendant);
    ypos_descendant = ypos + length_scale * node->descendant[i]->length * sin(angle_descendant);
    if (node->descendant[i]->thick >= 0.0)
    {
      fprintf(f, "1 setlinecap %f setlinewidth\n", node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto stroke\n",
	      xpos, ypos, xpos_descendant, ypos_descendant);
    }
    else
    {
      fprintf(f, "gsave 1 0 0 setrgbcolor\n");
      fprintf(f, "1 setlinecap %f setlinewidth\n", -node->descendant[i]->thick);
      fprintf(f, "newpath %f %f moveto %f %f lineto stroke grestore\n",
	      xpos, ypos, xpos_descendant, ypos_descendant);
    }
    if (psinfo->bootstrap_numtrees)
    {
      bootstrap_value = ((double) node->descendant[i]->edge_counter) / ((double) psinfo->bootstrap_numtrees);
      sprintf(bootstrap_string, "%1.3f", bootstrap_value);
      bootstrap_stringlength = strlen(bootstrap_string);
      ps_string(bootstrap_string, buf);
      fprintf(f, "gsave bsfont setfont\n");
      fprintf(f, "newpath %f %f moveto\n", (xpos + xpos_descendant) * 0.5, (ypos + ypos_descendant) * 0.5);
      if ((PI * 0.5 <= angle_descendant) && (angle_descendant < PI * 1.5))
        fprintf(f, "%f rotate 0 %f rmoveto\n", angle_descendant / PI * 180.0 - 180.0, -0.8 * fontheight);
      else
        fprintf(f, "%f rotate 0 %f rmoveto\n", angle_descendant / PI * 180.0, 0.3 * fontheight);
      fprintf(f, "%s dup stringwidth pop -0.5 mul 0 rmoveto show\n", buf);
      fprintf(f, "grestore\n");
    }
    draw_unode_ps(f, node->descendant[i], xpos_descendant, ypos_descendant,
            length_scale, angle_descendant, ar_descendant, angle_min, angle_limit,
            fontheight, psinfo);
    angle_descendant = fmod(angle_descendant + ar_descendant * 0.5, 2 * PI);
  }
  if (psinfo->print_leafnames && node->name[0])
  {
    fprintf(f, "gsave\n");
    switch (psinfo->print_leafnames)
    {
    case PHYL_LEAVES_HORIZONTAL:
      if (angle < PI * 0.5)
      {
        fprintf(f, "%f %f moveto\n", xpos + 0.3 * fontheight, ypos);
	ps_printleaf(f, node->name, psinfo->attrlist, 0);
      }
      else if (angle < PI)
      {
        fprintf(f, "%f %f moveto\n", xpos - 0.3 * fontheight, ypos);
	ps_printleaf(f, node->name, psinfo->attrlist, 1);
      }
      else if (angle < PI * 1.5)
      {
        fprintf(f, "%f %f moveto\n", xpos - 0.3 * fontheight, ypos - 0.3 * fontheight);
	ps_printleaf(f, node->name, psinfo->attrlist, 1);
      }
      else
      {
        fprintf(f, "%f %f moveto\n", xpos + 0.3 * fontheight, ypos - 0.3 * fontheight);
	ps_printleaf(f, node->name, psinfo->attrlist, 0);
      }
      fprintf(f, "grestore\n");
      break;
    case PHYL_LEAVES_RADIAL:
      if ((PI * 0.5 <= angle) && (angle < PI * 1.5))
      {
        fprintf(f, "%f %f moveto %f rotate\n", xpos, ypos, angle / PI * 180.0 - 180.0);
        fprintf(f, "%f %f rmoveto\n", -0.3 * fontheight, -0.2 * fontheight);
	ps_printleaf(f, node->name, psinfo->attrlist, 1);
      }
      else
      {
        fprintf(f, "%f %f moveto %f rotate\n", xpos, ypos, angle / PI * 180.0);
        fprintf(f, "%f %f rmoveto\n", 0.3 * fontheight, -0.2 * fontheight);
	ps_printleaf(f, node->name, psinfo->attrlist, 0);
      }
      fprintf(f, "grestore\n");
      break;
    }
  }
}


int phyl_pstree_funcs(FILE *f, const PHYL_PSINFO *info, double leaffont_size)
{
  fprintf(f, "/leaffont /%s findfont %f scalefont def\n", info->fontname, leaffont_size);
  fprintf(f, "/bsfont /%s findfont %f scalefont def\n", info->fontname, leaffont_size);
  fprintf(f, "/specfont /%s findfont %f scalefont def\n", info->spec_fontname, leaffont_size);
  fprintf(f, "/familyfont /Helvetica-Bold findfont 40 scalefont def\n");
  fprintf(f, "\n");
  fprintf(f, "/highlightbox\n");
  fprintf(f, "{\n");
  fprintf(f, "  gsave\n");
  fprintf(f, "  %f setlinewidth\n", leaffont_size * 0.05);
  fprintf(f, "  0.0 %f rmoveto 0 %f rlineto dup 0.0 rlineto 0 %f rlineto neg 0 rlineto\n",
          -0.3 * leaffont_size, leaffont_size, -leaffont_size);
  fprintf(f, "  gsave\n");
  fprintf(f, "  setrgbcolor fill\n");
  fprintf(f, "  grestore\n");
  fprintf(f, "  stroke\n");
  fprintf(f, "  grestore\n");
  fprintf(f, "} bind def\n");
  fprintf(f, "\n");
  fprintf(f, "/specshow\n");
  fprintf(f, "{\n");
  fprintf(f, "  save\n");
  fprintf(f, "  gsave\n");
  fprintf(f, "  leaffont setfont\n");
  fprintf(f, "  exch\n");
  fprintf(f, "  /specname exch def\n");
  fprintf(f, "  exch\n");
  fprintf(f, "  /leafname exch def\n");
  fprintf(f, "  exch\n");
  fprintf(f, "  1 eq\n");
  fprintf(f, "  {\n");
  fprintf(f, "    leafname stringwidth pop\n");
  fprintf(f, "    specname stringwidth pop\n");
  fprintf(f, "    dup 0.0 ne\n");
  fprintf(f, "    {\n");
  fprintf(f, "      (\\(\\) ) stringwidth pop add\n");
  fprintf(f, "    }\n");
  fprintf(f, "    if\n");
  fprintf(f, "    add neg\n");
  fprintf(f, "    0.0 rmoveto\n");
  fprintf(f, "  }\n");
  fprintf(f, "  if\n");
  fprintf(f, "  exch\n");
  fprintf(f, "  dup\n");
  fprintf(f, "  0.0 ge\n");
  fprintf(f, "  {\n");
  fprintf(f, "    /b exch def\n");
  fprintf(f, "    exch /g exch def\n");
  fprintf(f, "    exch /r exch def\n");
  fprintf(f, "    r g b\n");
  fprintf(f, "    leafname stringwidth pop\n");
  fprintf(f, "    specname stringwidth pop\n");
  fprintf(f, "    dup 0.0 ne\n");
  fprintf(f, "    {\n");
  fprintf(f, "      (\\(\\) ) stringwidth pop add\n");
  fprintf(f, "    }\n");
  fprintf(f, "    if\n");
  fprintf(f, "    add\n");
  fprintf(f, "    highlightbox\n");
  fprintf(f, "  }\n");
  fprintf(f, "  {\n");
  fprintf(f, "    pop exch pop exch pop\n");
  fprintf(f, "  }\n");
  fprintf(f, "  ifelse\n");
  fprintf(f, "  exch\n");
  fprintf(f, "  dup\n");
  fprintf(f, "  0.0 ge\n");
  fprintf(f, "  {\n");
  fprintf(f, "    /b exch def\n");
  fprintf(f, "    exch /g exch def\n");
  fprintf(f, "    exch /r exch def\n");
  fprintf(f, "    r g b setrgbcolor\n");
  fprintf(f, "  }\n");
  fprintf(f, "  {\n");
  fprintf(f, "    pop exch pop exch pop\n");
  fprintf(f, "  }\n");
  fprintf(f, "  ifelse\n");
  fprintf(f, "  leaffont setfont leafname show\n");
  fprintf(f, "  specname stringwidth pop 0.0 ne\n");
  fprintf(f, "  {\n");
  fprintf(f, "    specfont setfont ( \\() show specname show (\\)) show\n");
  fprintf(f, "  }\n");
  fprintf(f, "  if\n");
  fprintf(f, "  grestore\n");
  fprintf(f, "  restore\n");
  fprintf(f, "} bind def\n");
  return (0);
}


/*
 * Meaning of members of PHYL_PSINFO struct:
 * fontname: The name of a PostScript font, used with findfont
 * min_fontsize, max_fontsize: Minimal and maximal font scaling
 *     factors. Actual scaling factor is chosen from this range
 *     depending on density of node names.
 * label: An unit name for the tree height axis. If not NULL, and
 *     if complete set of edge lengths is available,
 *     axis with tics is placed to the right of the tree.
 * tic_length: Length of tics on tree height axis.
 * label_start: Value of axis unit at bottom of tree.
 * label_fontsize: font size for labelling vertical axis.
 * linewidth: width of standard lines (used with setlinewidth).
 * print_leafnames: If nonzero, leaf names are printed and tree is
 *     shrunk vertically to accomodate names within tree box.
 * print_edgelengths: If nonzero, edge lengths are printed.
 * leafnames_at_max: If nonzero, leaf names are not printed at the
 *     edge terminus for edges with negative lengths, but instead,
 *     they are printed at the maximal edge height. This feature is
 *     just preliminarily implemented and only takes the last edge
 *     into account. Only works for rectangular trees.
 */

int phyl_pstree(FILE *f, const PHYLTREE *tree, double x0, double y0, double width, double height,
        const PHYL_PSINFO *psinfo)
{
  double node_distance, length_scale, xpos, ypos;
  double tree_h, fontsize;
  double label_base, label_height, label_xpos, label_ypos, label_val, label_distance;
  double leafname_height;
  int    label_mag;
  char label_str[30], label_format[20];

  if (!tree->lengthinfo_complete)
    phyl_vshape_lengths(tree->root, tree->num_leaves);
  fontsize = width * 0.9 / tree->num_leaves;
  if (fontsize < psinfo->min_fontsize)
    fontsize = psinfo->min_fontsize;
  if (fontsize > psinfo->max_fontsize)
    fontsize = psinfo->max_fontsize;
  if (psinfo->print_leafnames)
  {
    /* FIXME: glyph width:height ratio hard-coded to 0.5 */
    leafname_height = (phyl_max_nodenamelength(tree, psinfo->attrlist) + 2) * fontsize * 0.5;
  }
  else
    leafname_height = 0.0;
  if (psinfo->tree_height < 0.0)
    tree_h = phyl_treeheight(tree->root);
  else
    tree_h = psinfo->tree_height;
  if (tree_h <= 0.0)
    tree_h = 1.0;
  fprintf(f, "gsave\n");
  phyl_pstree_funcs(f, psinfo, fontsize);
  if ((psinfo->label == NULL) || !tree->lengthinfo_complete)
    length_scale = (height - leafname_height) / tree_h;
  else
  {
    if ((label_height = 2.5 * psinfo->label_fontsize) < leafname_height)
      label_height = leafname_height;
    label_base = y0 + height - label_height;
    length_scale = (height - label_height) / tree_h;
    if (length_scale < 0.0)
      return(PHYLERR_PSBOXTOOSMALL);
    width -= 8.0 * psinfo->label_fontsize + psinfo->tic_length;
    label_xpos = x0 + width + 2 * psinfo->label_fontsize;
    errno = 0;
    label_distance = pow(10, floor(log10(tree_h)));
    if (errno)
    {
      label_distance = tree_h / 10;
      label_val = psinfo->label_start;
      sprintf(label_format, "%% #g");
    }
    else
    {
      if (tree_h / label_distance <= 2.0)
        label_distance *= 0.2;
      else if (tree_h / label_distance <= 5.0)
        label_distance *= 0.5;
      label_val = floor(psinfo->label_start / label_distance) * label_distance;
      if (errno)
      {
        label_val = psinfo->label_start;
        sprintf(label_format, "%% #g");
      }
      else
      {
        if (label_val < psinfo->label_start)
          label_val += label_distance;
        if (fabs(label_val) > fabs(label_val + tree_h))
          label_mag = ceil(log10(fabs(label_val + 0.0001 * label_val)));
        else
          label_mag = ceil(log10(fabs(label_val + tree_h + 0.0001 * (label_val + tree_h))));
        if (errno)
          sprintf(label_format, "%% #g");
        else
        {
          if (label_mag > 7)
            sprintf(label_format, "%% 9.3e");
          else if (label_mag > 4)
            sprintf(label_format, "%% %1df", label_mag + 1);
          else if (label_mag > -1)
            sprintf(label_format, "%% 7.%1df", 5 - label_mag);
          else if (label_mag > -3)
            sprintf(label_format, "%% 1.%1df", 5 - label_mag);
          else
            sprintf(label_format, "%% 9.3e");
        }
      }
    }
    fprintf(f, "0 setlinecap\n");
    fprintf(f, "/%s findfont %f scalefont setfont\n", psinfo->fontname, psinfo->label_fontsize);
    fprintf(f, "%f %f moveto %s show\n", label_xpos, label_base + 1.5 * psinfo->label_fontsize, ps_string(psinfo->label, buf));
    fprintf(f, "newpath %f %f moveto %f %f lineto stroke\n", label_xpos, label_base, label_xpos, y0);
/*    while (label_val <= psinfo->label_start + tree_h + DBL_EPSILON * length_scale) */
    while (label_val <= psinfo->label_start + tree_h + label_distance * 0.1)
    {
      label_ypos = y0 + (label_val - psinfo->label_start) * length_scale;
      fprintf(f, "newpath %f %f moveto %f %f lineto stroke\n", label_xpos, label_ypos, label_xpos + psinfo->tic_length, label_ypos);
      sprintf(label_str, label_format, label_val);
      fprintf(f, "%f %f moveto %s show\n", label_xpos + psinfo->tic_length, label_ypos, ps_string(label_str, buf));
      label_val += label_distance;
    }
  }
  node_distance = width / tree->num_leaves;
  xpos = x0 + width * 0.5;
  ypos = y0 - phyl_min_nodelevel(tree->root, 0.0) * length_scale;
  fprintf(f, "/leaffont /%s findfont %f scalefont def\n", psinfo->fontname, fontsize);
  if (tree->lengthinfo_complete)
    draw_rnode_ps(f, tree->root, xpos, ypos, x0, length_scale, node_distance, fontsize * 0.5, fontsize, ypos, psinfo);
  else
    draw_vnode_ps(f, tree->root, xpos, ypos, x0, length_scale, node_distance, fontsize * 0.5, fontsize, psinfo);
  fprintf(f, "grestore\n");
  return (0);
}


int phyl_ps_utree(FILE *f, const PHYLTREE *tree, double x0, double y0, double width, double height,
        const PHYL_PSINFO *psinfo)
{
  double length_scale, xpos, ypos;
  double xmin_x = 0.0, xmin_y = 0.0, ymin_x = 0.0, ymin_y = 0.0;
  double xmax_x = 0.0, xmax_y = 0.0, ymax_x = 0.0, ymax_y = 0.0;
  double tw, th, d1, d2, angle;
  double fontsize;
  long   max_nnl;

  /* fprintf(f, "gsave 0 1 0 setrgbcolor %f %f moveto %f 0.0 rlineto 0.0 %f rlineto %f 0.0 rlineto closepath stroke grestore\n", x0, y0, width, height, -width); */
  if (!tree->lengthinfo_complete)
    set_lengths(tree->root, 1.0);
  max_nnl = phyl_max_nodenamelength(tree, psinfo->attrlist);
  fontsize = width * 0.1 / max_nnl * 1.4;
  if (fontsize < psinfo->min_fontsize)
    fontsize = psinfo->min_fontsize;
  if (fontsize > psinfo->max_fontsize)
    fontsize = psinfo->max_fontsize;
  find_unode_extrema(tree->root, 0.0, 0.0, 0.0, 2 * PI, psinfo->angle_min, psinfo->angle_limit,
          &xmin_x, &xmin_y, &ymin_x, &ymin_y, &xmax_x, &xmax_y, &ymax_x, &ymax_y);
  d1 = sqrt((xmax_x - xmin_x) * (xmax_x - xmin_x) + (xmax_y - xmin_y) * (xmax_y - xmin_y));
  d2 = sqrt((ymax_x - ymin_x) * (ymax_x - ymin_x) + (ymax_y - ymin_y) * (ymax_y - ymin_y));
  if (d1 > d2)
    angle = atan((xmax_y - xmin_y) / (xmax_x - xmin_x));
  else
    angle = atan((ymax_x - ymin_x) / (ymax_y - ymin_y)) + PI * 0.5;
  if (width < height)
    angle -= PI * 0.5;
  xmin_x = 0.0;
  xmin_y = 0.0;
  ymin_x = 0.0;
  ymin_y = 0.0;
  xmax_x = 0.0;
  xmax_y = 0.0;
  ymax_x = 0.0;
  ymax_y = 0.0;
  find_unode_extrema(tree->root, 0.0, 0.0, -angle, 2 * PI, psinfo->angle_min, psinfo->angle_limit,
          &xmin_x, &xmin_y, &ymin_x, &ymin_y, &xmax_x, &xmax_y, &ymax_x, &ymax_y);
  tw = xmax_x - xmin_x;
  th = ymax_y - ymin_y;
  if ((tw == 0.0) && (th == 0.0))
  {
    tw = 1.0;
    th = 1.0;
  }
  if (tw == 0.0)
    tw = th;
  if (th == 0.0)
    th = tw;
  switch (psinfo->print_leafnames)
  {
  case PHYL_LEAVES_HORIZONTAL:
    length_scale = (width - fontsize * 2.0 * 0.8 * max_nnl) / tw;
    if ((height - 2 * fontsize) / th < length_scale)
      length_scale = (height - 2.0 * fontsize) / th;
    xpos = x0 + (tw - xmax_x) * length_scale + fontsize * 0.8 * max_nnl
            + (width - fontsize * 2.0 * 0.8 * max_nnl - tw * length_scale) * 0.5;
    ypos = y0 + (th - ymax_y) * length_scale + fontsize + (height - 2.0 * fontsize - th * length_scale) * 0.5;
    break;
  case PHYL_LEAVES_RADIAL:
    length_scale = (width - fontsize * 2.0 * 0.8 * max_nnl) / tw;
    if ((height - fontsize * 2.0 * 0.8 * max_nnl) / th < length_scale)
      length_scale = (height - fontsize * 2.0 * 0.8 * max_nnl) / th;
    xpos = x0 + fontsize * 0.8 * max_nnl -xmin_x * length_scale;
    /* + (tw - xmax_x) * length_scale + fontsize * 0.8 * max_nnl */
            /* + (width - fontsize * 2.0 * 0.8 * max_nnl - tw * length_scale) * 0.5; */
    ypos = y0 + fontsize * 0.8 * max_nnl - ymin_y * length_scale;
    /* + (th - ymax_y) * length_scale + fontsize */
            /* + (height - fontsize * 2.0 * 0.8 * max_nnl - th * length_scale) * 0.5; */
    break;
  case PHYL_LEAVES_NONE:
    length_scale = width / tw;
    if (height / th < length_scale)
      length_scale = height / th;
    xpos = x0 + (tw - xmax_x) * length_scale + (width - tw * length_scale) * 0.5;
    ypos = y0 + (th - ymax_y) * length_scale + (height - th * length_scale) * 0.5;
    break;
  default:
    fprintf(stderr, "ptlib warning: unknown leaf printing mode %d -- printing no leaves\n", psinfo->print_leafnames);
    length_scale = width / tw;
    if (height / th < length_scale)
      length_scale = height / th;
    xpos = x0 + (tw - xmax_x) * length_scale + (width - tw * length_scale) * 0.5;
    ypos = y0 + (th - ymax_y) * length_scale + (height - th * length_scale) * 0.5;
    break;
  }
  if (length_scale < 0.0)
    return(PHYLERR_PSBOXTOOSMALL);
  fprintf(f, "save\n");
  phyl_pstree_funcs(f, psinfo, fontsize);
  fprintf(f, "/leaffont /%s findfont %f scalefont def\n", psinfo->fontname, fontsize);
  draw_unode_ps(f, tree->root, xpos, ypos, length_scale, -angle, 2 * PI, psinfo->angle_min, psinfo->angle_limit,
          fontsize, psinfo);
  fprintf(f, "restore\n");
  return (0);
}
