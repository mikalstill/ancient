/******************************************************************************
  Copyright (C) Michael Still 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <libmplot.h>

/******************************************************************************
DOCBOOK START

FUNCTION plot_newplot
PURPOSE creates a blank plot ready for drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
plot_state *plot_newplot (unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION This function is the first call which should be called for a plot. It creates a blank canvas, ready for drawing.

RETURNS A plot state pointer, which is used by most of the other <command>libmplot</command> functions.

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}
EXAMPLE END

DOCBOOK END
******************************************************************************/

plot_state *
plot_newplot (unsigned int x, unsigned int y)
{
  plot_state *state;

  state = malloc (sizeof (plot_state));
  if (state == NULL)
    return state;

  // A blank raster
  state->raster = malloc (sizeof (plot_pixel) * x * y);
  if (state->raster == NULL)
    {
      free (state);
      return NULL;
    }
  memset (state->raster, 255, sizeof (plot_pixel) * x * y);

  state->x = x;
  state->y = y;
  state->maxptr = x * y;

  // Line attributes
  state->linewidthx = 1;
  state->linewidthy = 1;
  state->linecap = 0;
  state->linejoin = 0;
  state->linedashlen = 0;
  state->linedash = NULL;

  state->fillcolor.r = 0;
  state->fillcolor.g = 0;
  state->fillcolor.b = 0;

  state->linecolor.r = 0;
  state->linecolor.g = 0;
  state->linecolor.b = 0;

  state->fontcolor.r = 0;
  state->fontcolor.g = 0;
  state->fontcolor.b = 0;

  state->line = (plot_lineseg *) NULL;

#if defined HAVE_LIBFREETYPE
  state->face = NULL;

  if (((state->ft = (FT_Library *) malloc (sizeof (FT_Library))) == NULL) ||
      FT_Init_FreeType (state->ft))
#endif
    state->ft = NULL;

  return state;
}

void plot_closeplot(plot_state *state)
{
  fprintf(stderr, "todo\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_getraster
PURPOSE return a pointer the the raster we have drawn

SYNOPSIS START
#include&lt;libmplot.h&gt;
char *plot_getraster(plot_state *state);
SYNOPSIS END

DESCRIPTION Returns the graph you have drawn.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;
char *raster;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

... drawing commands ...

raster = plot_getraster(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

char *
plot_getraster (plot_state * state)
{
  return (char *) state->raster;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setlinestart
PURPOSE sets the starting point of series of lines and curves

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_setlinestart (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION Set the starting point for the sequence of curves and lines that it to be drawn on the current plot. This call is compulsory for almost all of the line drawing functions. It is not required for the <command>plot_rectangle plot_rectanglerot</command> function.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 200, 200);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setlinestart (plot_state * state, unsigned int x, unsigned int y)
{
  plot_endline (state);
  state->line = (plot_lineseg *) malloc (sizeof (plot_lineseg));
  if (state->line != NULL)
    {
      state->line->next = NULL;
    }

  state->line->x = x;
  state->line->y = y;

  state->penx = x;
  state->peny = y;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_addlinesegment
PURPOSE add a straight line segment to the polygon

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_addlinesegment (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION This function adds a straight line segment to the polygon currently being drawn. The line travels from the current pen location (the end of the previous drawing command, or <command>plot_setlinestart</command> function call), to the point specified.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 10, 10);
plot_addlinesegment(graph, 20, 20);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_addlinesegment (plot_state * state, unsigned int x, unsigned int y)
{
  // Find the end of the line
  plot_lineseg *current;
  current = state->line;

  while (current->next != NULL)
    current = current->next;

  current->next = (plot_lineseg *) malloc (sizeof (plot_lineseg));
  if (current->next == NULL)
    {
      fprintf (stderr, "An error we should handle better\n");
      exit (42);
    }

  current = current->next;
  current->next = NULL;
  current->x = x;
  current->y = y;

  state->penx = x;
  state->peny = y;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_addcubiccurvesegment
PURPOSE add a Bezier with two control points to the polygon we are drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_addcubiccurvesegment (plot_state * state,
			   unsigned int inx1, unsigned int iny1,
			   unsigned int inx2, unsigned int iny2,
			   unsigned int inx3, unsigned int iny3);
SYNOPSIS END

DESCRIPTION This function adds a Bezier curve between the current pen location and (inx1, iny1) to the current polygon. This curve will have two control points: (inx2, iny2) and (inx3, iny3).

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 20, 20);
plot_addcubiccurvesegment(graph, 20, 120, 40, 40, 100, 0);
plot_strokeline(graph);
plot_closeline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_addcubiccurvesegment (plot_state * state,
			   unsigned int inx1, unsigned int iny1,
			   unsigned int inx2, unsigned int iny2,
			   unsigned int inx3, unsigned int iny3)
{
  double t;
  float calcx, calcy, tstep;
  float finx0 = state->penx, finx1 = inx2, finx2 = inx3, finx3 = inx1;
  float finy0 = state->peny, finy1 = iny2, finy2 = iny3, finy3 = iny1;

  // todo: maths to make this an efficient choice...
  tstep = 0.025;
  for (t = tstep; t <= 1 + tstep; t += tstep)
    {
      calcx = (finx0 + t * (-finx0 * 3 + t * (3 * finx0 - finx0 * t))) +
	(t * (3 * finx1 + t * (-6 * finx1 + finx1 * 3 * t))) +
	(t * t * (finx2 * 3 - finx2 * 3 * t)) +
	(finx3 * t * t * t);

      calcy = (finy0 + t * (-finy0 * 3 + t * (3 * finy0 - finy0 * t))) +
	(t * (3 * finy1 + t * (-6 * finy1 + finy1 * 3 * t))) +
	(t * t * (finy2 * 3 - finy2 * 3 * t)) +
	(finy3 * t * t * t);

      plot_addlinesegment(state, (unsigned int) calcx, (unsigned int) calcy);
    }
}
/******************************************************************************
DOCBOOK START

FUNCTION plot_addquadraticcurvesegmentone
PURPOSE add a Bezier with one control point to the polygon we are drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_addcubiccurvesegmentone (plot_state * state,
			   unsigned int inx1, unsigned int iny1,
			   unsigned int inx2, unsigned int iny2);
SYNOPSIS END

DESCRIPTION This function adds a Bezier curve between the current pen location and (inx1, iny1) to the current polygon. This curve will have two control points: the current pen location and (inx3, iny3).

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 20, 20);
plot_addquadraticcurvesegmentone(graph, 20, 120, 40, 40);
plot_strokeline(graph);
plot_closeline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_addquadraticcurvesegmentone (plot_state * state,
				  unsigned int x1, unsigned int y1,
				  unsigned int x2, unsigned int y2)
{
  plot_addcubiccurvesegment(state, x1, y1, state->penx, state->peny, x2, y2);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_addquadraticcurvesegmenttwo
PURPOSE add a Bezier with one control point to the polygon we are drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_addquadraticcurvesegmenttwo (plot_state * state,
			   unsigned int inx1, unsigned int iny1,
			   unsigned int inx2, unsigned int iny2);
SYNOPSIS END

DESCRIPTION This function adds a Bezier curve between the current pen location and (inx1, iny1) to the current polygon. This curve will have two control points: (inx2, iny2) and (inx1, iny1).

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 20, 20);
plot_addquadraticcurvesegmenttwo(graph, 20, 120, 40, 40);
plot_strokeline(graph);
plot_closeline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_addquadraticcurvesegmenttwo (plot_state * state,
				  unsigned int x1, unsigned int y1,
				  unsigned int x2, unsigned int y2)
{
  plot_addcubiccurvesegment(state, x1, y1, x2, y2, x1, y1);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_closeline
PURPOSE return to the start of this polygon

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_closeline(plot_state *state);
SYNOPSIS END

DESCRIPTION This function draws a straight line segment between the current pen location, and the starting point of the polygon.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 10, 10);
... drawing commands ...
plot_closeline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_closeline (plot_state * state)
{
  if (state->line != NULL)
    plot_addlinesegment (state, state->line->x, state->line->y);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_endline
PURPOSE free the resources consumed by the polygon just drawn

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_endline (plot_state *state);
SYNOPSIS END

DESCRIPTION It is very important that you use this function to free resources before drawing another polygon. If you do not, then subsequent polygons might have incorrect appearances, and your application will leak memory.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

... line drawing commands ...

plot_endline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_endline (plot_state * state)
{
  // todo: this is inefficient
  plot_lineseg *current, *prev;

  // End case
  if (state->line == NULL)
    return;

  current = state->line;
  prev = NULL;
  while (current->next != NULL)
    {
      prev = current;
      current = current->next;
    }

  free (current);
  if (prev != NULL)
    prev->next = NULL;
  else
    state->line = NULL;

  plot_endline (state);
  state->linedashcount = 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_strokeline
PURPOSE draw the outline specified by the current polygon

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_strokeline(plot_state *state);
SYNOPSIS END

DESCRIPTION 

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_strokeline (plot_state * state)
{
  // todo: support for non straight lines
  plot_lineseg *current;
  unsigned int x1, y1, c, d;
  int rise, run, startx, starty;
  float newy;
  int prevy;

  if (state->line == NULL)
    return;

  current = state->line;
  x1 = current->x;
  y1 = current->y;
  current = current->next;
  while (current != NULL)
    {
      if (y1 == current->y)
	{
	  for (c = plot_min (x1, current->x);
	       c < plot_max (x1, current->x) + 1; c++)
	    {
	      plot_drawpoint (state, state->linecolor, LIBMPLOT_TRUE, c, y1);
	    }
	}
      else if (x1 == current->x)
	{
	  for (c = plot_min (y1, current->y);
	       c < plot_max (y1, current->y) + 1; c++)
	    {
	      plot_drawpoint (state, state->linecolor, LIBMPLOT_TRUE, x1, c);
	    }
	}
      else
	{
	  startx = plot_min (x1, current->x);
	  starty = startx == x1 ? y1 : current->y;
	  rise = starty == y1 ? (current->y - y1) : (y1 - current->y);
	  run = startx == x1 ? (current->x - x1) : (x1 - current->x);

	  for (c = startx; c < plot_max (x1, current->x) + 1; c++)
	    {
	      prevy = (int) newy;
	      newy = ((float) rise / run) * (c - startx) + starty;

	      if (c == plot_min (x1, current->x))
		prevy = (int) newy;

	      if (abs (prevy - (int) newy) > 1)
		{
		  for (d = plot_min (prevy, (int) newy);
		       d < plot_max (prevy, (int) newy); d++)
		    {
		      plot_drawpoint (state, state->linecolor, LIBMPLOT_TRUE, c - 1, d);
		    }
		}

	      plot_drawpoint (state, state->linecolor, LIBMPLOT_TRUE, c, (int) newy);
	    }
	}

      x1 = current->x;
      y1 = current->y;
      current = current->next;
    }
}

// Draw a single point on the raster, with the current pen properties
void
plot_drawpoint (plot_state * state, plot_pixel color, int isLine, unsigned int x, unsigned int y)
{
  unsigned int xc, yc, tempx, tempy;

  if((state->linewidthx == 1) && (state->linewidthy == 1)){
    plot_drawpointactual(state, color, isLine, x, y);
    return;
  }

  tempx = x - (state->linewidthx / 2);
  for(xc = 0; xc < state->linewidthx; xc++)
    {
      tempy = y - (state->linewidthy / 2);
      for(yc = 0; yc < state->linewidthy; yc++)
	{
	  plot_drawpointactual(state, color, isLine, tempx, tempy);
	  tempy++;
	}
      tempx++;
    }
}

// Turn on a single pixel
void
plot_drawpointactual (plot_state * state, plot_pixel color, int isLine, unsigned int x, unsigned int y)
{
  unsigned long ptr = state->x * y + x;
  int linedashcount;

  if (ptr > state->maxptr)
    return;
  
  state->linedashcount++;
  if(state->linedashcount > (state->linedashlen - 1)){
    state->linedashcount = 0;
  }
  linedashcount = state->linedashcount;

  if((state->linedashlen > 0) && (isLine == LIBMPLOT_TRUE) && (state->linedash[linedashcount] == '\0')){
    return;
  }
  
  state->raster[ptr] = color;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_fillline
PURPOSE fill the current polygon with the current fill color

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_fillline (plot_state * state);
SYNOPSIS END

DESCRIPTION This function uses the even odd rule (in two directions) to determine if a point is inside the current polygon, and then based on that decision colors all of the points which are inside the polygon with the current fill color.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

... draw polygon ...
plot_fillline(graph);
plot_endline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

// todo_mikal: non-zero winding rule as well
void
plot_fillline (plot_state * state)
{
  plot_state *tempstate;
  int intersects;
  unsigned int row, col, lcol;

  // Build a temporary copy of just that polygon
  tempstate = plot_newplot(state->x, state->y);
  tempstate->line = state->line;
  plot_strokeline(tempstate);
  
  // For every point in the raster
  for(row = 0; row < state->y ; row++)
    {
      for(col = 0; col < state->x; col++)
	{
	  // The first direction is forwards
	  intersects = 0;
	  if(tempstate->raster[row * state->x + col].r != 0)
	    {
	      for(lcol = col + 1; lcol < state->x; lcol++)
		{
		  if(tempstate->raster[row * state->x + lcol].r == 0)
		    {
		      if(col > 0)
			{
			  if(tempstate->raster[row * state->x + lcol - 1].r != 0)
			    intersects++;
			}
		      else
			intersects++;
		    }
		}

	      // The other direction is backwards
	      if(intersects % 2 == 1)
		{
		  intersects = 0;
		  for(lcol = col -1; lcol != -1; lcol--){
		    if(tempstate->raster[row * state->x + lcol].r == 0)
		      {
			if(col > 0)
			  {
			    if(tempstate->raster[row * state->x + lcol - 1].r != 0)
			      intersects++;
			  }
			else
			  intersects++;
		      }
		  }
		  
		  if(intersects % 2 == 1)
		    {
		      plot_drawpointactual(state, state->fillcolor, LIBMPLOT_FALSE, col, row);
		    }
		}
	    }
	}
    }
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setlinewidth
PURPOSE sets the current width of the line to be drawn

SYNOPSIS START
#include&lt;libmplot.h&gt;
void panda_setlinewidth(plot_state *state, int widthx, int widthy);
SYNOPSIS END

DESCRIPTION This function sets the size of the line (vertically and horizontally) which the polygon will be stroked with. The line drawn is centered around the actual point which is on the polygon.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinewidth(graph, 4, 1);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setlinewidth (plot_state * state, int w, int h)
{
  state->linewidthx = w;
  state->linewidthy = h;
}

void
plot_setlinecap (plot_state * state, int c)
{
  fprintf (stderr, "todo\n");
}

void
plot_setlinejoin (plot_state * state, int j)
{
  fprintf (stderr, "todo\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setlinedash
PURPOSE sets the dashing pattern used to stroke lines

SYNOPSIS START
#include&lt;libmplot.h&gt;
void panda_setlinedash(plot_state *state, int length, char *dash);
SYNOPSIS END

DESCRIPTION START
This function takes the length of the dashing pattern (in bytes), and the dashing pattern itself. The dashing pattern is expressed in the form of a char * where a given byte is \0 if the point is not to be drawn, or \1 if it is to be drawn. See the example for an example...
</para>

<para>
This dashing pattern is applied by cycling through the pattern, one step per pixel drawn. This might not have the expected effect for think lines.
DESCRIPTION END

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinedash(graph, 4, "\0\0\1\1");
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setlinedash (plot_state * state, int length, char *dashing)
{
  if(state->linedash != NULL)
    free(state->linedash);

  state->linedashlen = length;
  if((state->linedash = malloc(sizeof(char) * length)) == NULL){
    fprintf(stderr, "Could not allocate dash storage\n");
    return;
  }
  memcpy(state->linedash, dashing, length);
  state->linedashcount = 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setfillcolor
PURPOSE sets the colour that the finished polygon will be filled with

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_setfillcolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to fill the polygon with when the <command>plot_fillline</command> function is called.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfillcolor(state, 255, 0, 0); // red
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setfillcolor (plot_state * state, int red, int green, int blue)
{
  state->fillcolor.r = red;
  state->fillcolor.g = green;
  state->fillcolor.b = blue;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setlinecolor
PURPOSE sets the colour that the finished polygon will be stroked with

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_setlinecolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to draw the polygon with when the <command>plot_strokeline</command> function is called.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinecolor(state, 255, 0, 0); // red
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setlinecolor (plot_state * state, int red, int green, int blue)
{
  state->linecolor.r = red;
  state->linecolor.g = green;
  state->linecolor.b = blue;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setfontcolor
PURPOSE sets the colour that text will be drawn with

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_setfontcolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to draw the text with

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfontcolor(state, 255, 0, 0); // red
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setfontcolor (plot_state * state, int red, int green, int blue)
{
  state->fontcolor.r = red;
  state->fontcolor.g = green;
  state->fontcolor.b = blue;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_rectangle
PURPOSE draw a rectangle

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_rectangle plot_rectanglerot (plot_state *state, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
SYNOPSIS END

DESCRIPTION Draw a rectangle on the plot. x1, y1 is the top left corner, and x2, y2 is the bottom right corner of the rectangle. After this call you can use <command>plot_strokeline</command> and <command>plot_fillline</command> to actaully affect the raster.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}
plot_rectangle(graph, 10, 10, 20, 30);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_rectangle (plot_state * state, unsigned int x1, unsigned int y1,
		unsigned int x2, unsigned int y2)
{
  plot_setlinestart (state, x1, y1);
  plot_addlinesegment (state, x2, y1);
  plot_addlinesegment (state, x2, y2);
  plot_addlinesegment (state, x1, y2);
  plot_closeline (state);
}

// todo: this signature makes little sense
void
plot_rectanglerot (plot_state * state, unsigned int x1, unsigned int y1,
		   unsigned int x2, unsigned int y2, float angle)
{
  fprintf (stderr, "todo\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setcircle
PURPOSE draw a circle

SYNOPSIS START
#include&lt;libmplot.h&gt;
void plot_circle (plot_state * state, unsigned int x, unsigned int y, unsigned int r);
SYNOPSIS END

DESCRIPTION This function draws a circle centered around (x, y), and with a radius of r.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_circle(graph, 10, 10, 3);
plot_strokeline(graph);
EXAMPLE END

DOCBOOK END
******************************************************************************/

#define pi 3.1415
void
plot_circle (plot_state * state, unsigned int x, unsigned int y,
	     unsigned int r)
{
  float angle;

  // todo: this could be more efficient
  // todo: this should be proportional to the radius
  plot_setlinestart (state, x + (int) (r * sin (angle)),
		     y + (int) (r * cos (angle)));

  for (angle = 0.01; angle < 2 * pi; angle += 0.01)
    {
      plot_addlinesegment (state, x + (int) (r * sin (angle)),
			   y + (int) (r * cos (angle)));
    }

  plot_strokeline (state);
  plot_endline (state);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setfont
PURPOSE sets the starting point of a curve

SYNOPSIS START
#include&lt;libmplot.h&gt;
void panda_setfont (plot_state *state, char *fontpath, int size);
SYNOPSIS END

DESCRIPTION Set the font and size to use to text drawing operations. The <command>fontpath</command> arguement is the path to a font which <command>FreeType</command> is capable of opening (for example PostScript fonts, and TrueType fonts -- refer to http://www.freetype.org for more details). The size is the point size of the text to be drawn.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfont(graph, "windings.ttf", 30);

EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_setfont (plot_state * state, char *font, int charsize)
{
  int error;

#if defined HAVE_LIBFREETYPE
  if (state->ft == NULL)
    {
      fprintf (stderr, "Initialization of Freetype failed\n");
      return;
    }

  // Load the face
  error = FT_New_Face (*state->ft, font, 0, &state->face);
  if (error == FT_Err_Unknown_File_Format)
    {
      fprintf (stderr,
	       "The specified font file could be opened and read, but it appears that its font format is unsupported by Freetpye\n");
      return;
    }
  else if (error)
    {
      fprintf (stderr, "Could not load font\n");
      return;
    }

  // Set the text size
  if (FT_Set_Char_Size (state->face, 0, charsize * 64, 75, 75))
    {
      fprintf (stderr, "Could not set font size\n");
      return;
    }
#else
  fprintf (stderr, "Freetype not found at compile time\n");
#endif
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_settextlocation
PURPOSE sets the cursor location for text drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
void panda_setlinestart (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION Set the cursor location for text drawing.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_settextlocation(graph, 30, 30);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_settextlocation (plot_state * state, unsigned int x, unsigned int y)
{
  state->textx = x;
  state->texty = y;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_settextlocation
PURPOSE sets the cursor location for text drawing

SYNOPSIS START
#include&lt;libmplot.h&gt;
void panda_getlinestart (plot_state *state, unsigned int *x, unsigned int *y);
SYNOPSIS END

DESCRIPTION Get the cursor location for text drawing.

RETURNS Nothing

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;
int x, y;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_settextlocation(graph, 30, 30);

... draw some text...

plot_gettextlocation(graph, &amp;x, &amp;y);
EXAMPLE END

DOCBOOK END
******************************************************************************/

void
plot_gettextlocation (plot_state * state, unsigned int *x, unsigned int *y)
{
  *x = state->textx;
  *y = state->texty;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_writestring
PURPOSE write a string onto the bitmap

SYNOPSIS START
#include&lt;libmplot.h&gt;
int plot_writestring(plot_state *state, char *string);
SYNOPSIS END

DESCRIPTION This function writes a string onto the specified bitmap. Note that the function does not implement any form of word wrap. This is the responsibility of the caller of the function. Have a look at the example below for a suggested technique for implementing word wrap.

RETURNS 0 on success, -1 otherwise

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;
char *words[] = {"This", "is", "a", "string", "which", "is", "quite", "long.",
		 "It", "demonstrates", "how", "to", "do", "word", "wrap", NULL};

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfontcolor(graph, 26, 22, 249);
plot_setfont(graph, "/usr/share/fonts/default/Type1/n021004l.pfb", 12);
plot_settextlocation(graph, 20, 70);
count = 0;
while(words[count] != NULL){
  plot_gettextlocation(graph, &amp;textx, &amp;texty);
  if((textx + plot_stringwidth(graph, words[count])) > 380){
    if(texty + 70 > 150)
      break;

    plot_settextlocation(graph, 20, texty + 20);
  }

  plot_writestring(graph, words[count]);
  plot_writestring(graph, " ");
  count++;
}
EXAMPLE END

DOCBOOK END
******************************************************************************/

int
plot_writestring (plot_state * state, char *string)
{
  return plot_writestringrot (state, string, 0.0);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_writestringrot
PURPOSE write a string onto the bitmap, with the text being rotated by the given angle

SYNOPSIS START
#include&lt;libmplot.h&gt;
int plot_writestringrot(plot_state *state, char *string, float angle);
SYNOPSIS END

DESCRIPTION START
This function writes a string onto the specified bitmap. Note that the function does not implement any form of word wrap. This is the responsibility of the caller of the function. Have a look at the example below for a suggested technique for implementing word wrap.
</para>

<para>
Note that the angle is in degrees.
DESCRIPTION END

RETURNS 0 on success, -1 otherwise

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;
char *words[] = {"This", "is", "a", "string", "which", "is", "quite", "long.",
		 "It", "demonstrates", "how", "to", "do", "word", "wrap", NULL};

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfontcolor(graph, 26, 22, 249);
plot_setfont(graph, "/usr/share/fonts/default/Type1/n021004l.pfb", 12);
plot_settextlocation(graph, 20, 70);
count = 0;
while(words[count] != NULL){
  plot_gettextlocation(graph, &amp;textx, &amp;texty);
  if((textx + plot_stringwidth(graph, words[count])) > 380){
    if(texty + 70 > 150)
      break;

    plot_settextlocation(graph, 20, texty + 20);
  }

  plot_writestringrot(graph, words[count], 45);
  plot_writestringrot(graph, " ", 45);
  count++;
}
EXAMPLE END

DOCBOOK END
******************************************************************************/

int
plot_writestringrot (plot_state * state, char *string, float angle)
{
  int count, len, rep;
  float safeangle;
  FT_Matrix matrix;

  rep = angle / 360;
  safeangle = angle - (rep * 360.0);

  if (safeangle == 0.0)
    {
      FT_Set_Transform (state->face, 0, 0);
    }
  else
    {
      // Convert the angle to radians
      safeangle = safeangle * pi / 180;

      matrix.xx = (FT_Fixed) (cos (safeangle) * 0x10000);
      matrix.xy = (FT_Fixed) (-sin (safeangle) * 0x10000);
      matrix.yx = (FT_Fixed) (sin (safeangle) * 0x10000);
      matrix.yy = (FT_Fixed) (cos (safeangle) * 0x10000);

      FT_Set_Transform (state->face, &matrix, 0);
    }

  len = strlen (string);
  for (count = 0; count < len; count++)
    {
      if (plot_paintglyph (state, string[count], LIBMPLOT_TRUE) == -1)
	return -1;
    }

  return 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_stringwidth
PURPOSE determine how many pixels a string will take

SYNOPSIS START
#include&lt;libmplot.h&gt;
unsigned int plot_stringwidth(plot_state *state, char *string);
SYNOPSIS END

DESCRIPTION This function determines how many pixels a string will consume in the current font and size.

RETURNS 0 on success, -1 otherwise

EXAMPLE START
#include&lt;libmplot.h&gt;
plot_state *graph;
char *words[] = {"This", "is", "a", "string", "which", "is", "quite", "long.",
		 "It", "demonstrates", "how", "to", "do", "word", "wrap", NULL};

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfontcolor(graph, 26, 22, 249);
plot_setfont(graph, "/usr/share/fonts/default/Type1/n021004l.pfb", 12);
plot_settextlocation(graph, 20, 70);
count = 0;
while(words[count] != NULL){
  plot_gettextlocation(graph, &amp;textx, &amp;texty);
  if((textx + plot_stringwidth(graph, words[count])) > 380){
    if(texty + 70 > 150)
      break;

    plot_settextlocation(graph, 20, texty + 20);
  }

  plot_writestring(graph, words[count]);
  plot_writestring(graph, " ");
  count++;
}
EXAMPLE END

DOCBOOK END
******************************************************************************/

unsigned int
plot_stringwidth (plot_state * state, char *string)
{
  int count, len, retval;
  unsigned int width = 0;

  len = strlen (string);
  for (count = 0; count < len; count++)
    {
      if ((retval =
	   plot_paintglyph (state, string[count], LIBMPLOT_FALSE)) == -1)
	return -1;
      width += retval;
    }

  return width;
}


















///////////////////
// Internal methods
unsigned int
plot_min (unsigned int one, unsigned int two)
{
  if (one > two)
    return two;
  return one;
}

unsigned int
plot_max (unsigned int one, unsigned int two)
{
  if (one > two)
    return one;
  return two;
}

int
plot_loadglyph (plot_state * state, char character)
{
#if defined HAVE_LIBFREETYPE
  FT_UInt index;

  if (state->ft == NULL)
    {
      fprintf (stderr, "Initialization of Freetype failed\n");
      return -1;
    }

  if (state->face == NULL)
    {
      fprintf (stderr, "No face loaded\n");
      return -1;
    }

  // Find the index of the glyph
  index = FT_Get_Char_Index (state->face, character);

  // Load that glyph into the face's slot
  if (FT_Load_Glyph (state->face, index, FT_LOAD_RENDER))
    {
      fprintf (stderr, "Could not load glyph for %c into face slot\n",
	       character);
      return -1;
    }

  return 0;

#else
  fprintf (stderr, "Freetype not found at compile time\n");
  return -1;
#endif
}

#define sfgb state->face->glyph->bitmap

int
plot_paintglyph (plot_state * state, char character, int dopaint)
{
#if defined HAVE_LIBFREETYPE
  int bmx, bmy;
  unsigned long p;

  if (state->ft == NULL)
    {
      fprintf (stderr, "Initialization of Freetype failed\n");
      return -1;
    }

  if (state->face == NULL)
    {
      fprintf (stderr, "No face loaded\n");
      return -1;
    }

  // Setup and paint the character
  if (plot_loadglyph (state, character) != -1)
    {
      if (dopaint == LIBMPLOT_TRUE)
	{
	  p = state->texty * state->x + state->textx;
	  p += state->face->glyph->bitmap_left;
	  p -= state->face->glyph->bitmap_top * state->x;

	  for (bmy = 0; bmy < sfgb.rows; bmy++)
	    {
	      for (bmx = 0; bmx < sfgb.width; bmx++)
		{
		  if (sfgb.buffer[bmy * sfgb.width + bmx] != 0)
		    {
		      unsigned long ptr = p + bmx;
		      if (ptr < state->maxptr)
			{
			  state->raster[ptr].r =
			    ~sfgb.buffer[bmy * sfgb.width +
					 bmx] * state->fontcolor.r /
			    ~sfgb.buffer[bmy * sfgb.width + bmx];
			  state->raster[ptr].g =
			    ~sfgb.buffer[bmy * sfgb.width +
					 bmx] * state->fontcolor.g /
			    ~sfgb.buffer[bmy * sfgb.width + bmx];
			  state->raster[ptr].b =
			    ~sfgb.buffer[bmy * sfgb.width +
					 bmx] * state->fontcolor.b /
			    ~sfgb.buffer[bmy * sfgb.width + bmx];
			}
		    }
		}
	      p += state->x;
	    }

	  // Increment pen position
	  state->textx += state->face->glyph->advance.x >> 6;
	  state->texty += state->face->glyph->advance.y >> 6;
	}
      else
	{
	  return state->face->glyph->advance.x >> 6;
	}

      return 0;
    }

  return -1;
#else
  fprintf (stderr, "Freetype not found at compile time\n");
  return -1;
#endif
}
