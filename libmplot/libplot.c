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

#include <libplot.h>

/******************************************************************************
DOCBOOK START

FUNCTION plot_newplot
PURPOSE creates a blank plot ready for drawing

SYNOPSIS START
#include&lt;libplot.h&gt;
plot_state *plot_newplot (unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION This function is the first call which should be called for a plot. It creates a blank canvas, ready for drawing.

RETURNS A plot state pointer, which is used by most of the other <command>libplot</command> functions.

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}
EXAMPLE END
SEEALSO plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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

  // Line attributes
  state->linewidth = 1;
  state->linecap = 0;
  state->linejoin = 0;
  state->linedash = 0;

  state->fillcolor.r = 0;
  state->fillcolor.g = 0;
  state->fillcolor.b = 0;

  state->linecolor.r = 0;
  state->linecolor.g = 0;
  state->linecolor.b = 0;

  state->line = (plot_lineseg *) NULL;
  return state;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_getraster
PURPOSE return a pointer the the raster we have drawn

SYNOPSIS START
#include&lt;libplot.h&gt;
char *plot_getraster(plot_state *state);
SYNOPSIS END

DESCRIPTION Returns the graph you have drawn.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;
char *raster;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

... drawing commands ...

raster = plot_getraster(graph);
EXAMPLE END
SEEALSO plot_newplot plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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
#include&lt;libplot.h&gt;
void plot_setlinestart (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION Set the starting point for the sequence of curves and lines that it to be drawn on the current plot. This call is compulsory for almost all of the line drawing functions. It is not required for the <command>plot_rectangle plot_rectanglerot</command> function.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 200, 200);
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_addlinesegment
PURPOSE add a straight line segment to the polygon

SYNOPSIS START
#include&lt;libplot.h&gt;
void plot_addlinesegment (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION This function adds a straight line segment to the polygon currently being drawn. The line travels from the current pen location (the end of the previous drawing command, or <command>plot_setlinestart</command> function call), to the point specified.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 10, 10);
plot_addlinesegment(graph, 20, 20);
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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
}

void
plot_addcubiccurvesegment (plot_state * state, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
			   unsigned int x3, unsigned int y3)
{
  fprintf (stderr, "todo\n");
}

void
plot_addquadraticcurvesegmentone (plot_state * state, unsigned int x1, unsigned int y1,
				  unsigned int x2, unsigned int y2)
{
  fprintf (stderr, "todo\n");
}

void
plot_addquadraticcurvesegmenttwo (plot_state * state, unsigned int x1, unsigned int y1,
				  unsigned int x2, unsigned int y2)
{
  fprintf (stderr, "todo\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_closeline
PURPOSE return to the start of this polygon

SYNOPSIS START
#include&lt;libplot.h&gt;
void plot_closeline(plot_state *state);
SYNOPSIS END

DESCRIPTION This function draws a straight line segment between the current pen location, and the starting point of the polygon.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinestart(graph, 10, 10);
... drawing commands ...
plot_closeline(graph);
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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
#include&lt;libplot.h&gt;
void plot_endline (plot_state *state);
SYNOPSIS END

DESCRIPTION It is very important that you use this function to free resources before drawing another polygon. If you do not, then subsequent polygons might have incorrect appearances, and your application will leak memory.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

... line drawing commands ...

plot_endline(graph);
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
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
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_strokeline
PURPOSE draw the outline specified by the current polygon

SYNOPSIS START
#include&lt;libplot.h&gt;
void plot_strokeline(plot_state *state);
SYNOPSIS END

DESCRIPTION 

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectangle plot_rectanglerot
DOCBOOK END
******************************************************************************/


void
plot_strokeline (plot_state * state)
{
  // todo: support for non straight lines
  plot_lineseg *current;
  unsigned int x1, y1, c, d;
  unsigned int rise, run;
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
      printf ("%d, %d -> %d, %d\n", x1, y1, current->x, current->y);

      if (y1 == current->y)
	{
	  for (c = plot_min (x1, current->x);
	       c < plot_max (x1, current->x) + 1; c++)
	    {
	      state->raster[state->x * y1 + c] = state->linecolor;
	    }
	}
      else if (x1 == current->x)
	{
	  for (c = plot_min (y1, current->y);
	       c < plot_max (y1, current->y) + 1; c++)
	    {
	      state->raster[state->x * c + x1] = state->linecolor;
	    }
	}
      else
	{
	  rise = current->y - y1;
	  run = current->x - x1;

	  for (c = plot_min (x1, current->x);
	       c < plot_max (x1, current->x) + 1; c++)
	    {
	      prevy = (int) newy;
	      newy = ((float) rise / run) * c;

	      if(c == plot_min (x1, current->x))
		prevy = (int) newy;

	      if(abs(prevy - (int) newy) > 1){
		for(d = plot_min(prevy, (int) newy); 
		    d < plot_max(prevy, (int) newy); d++){
		  state->raster[state->x * d + c - 1] = state->linecolor;
		}
	      }

	      state->raster[state->x * ((int) newy) + c] = state->linecolor;
	    }
	}

      x1 = current->x;
      y1 = current->y;
      current = current->next;
    }
}

void
plot_fillline (plot_state * state)
{
  fprintf (stderr, "todo\n");
}

// State modification functions
void
plot_setlinewidth (plot_state * state, int w)
{
  fprintf (stderr, "todo\n");
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

void
plot_setlinedash (plot_state * state, int on, int off, int interval)
{
  fprintf (stderr, "todo\n");
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setfillcolor
PURPOSE sets the colour that the finished polygon will be filled with

SYNOPSIS START
#include&lt;libplot.h&gt;
void plot_setfillcolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to fill the polygon with when the <command>plot_fillline</command> function is called.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfillcolor(state, 255, 0, 0); // red
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setlinecolor plot_rectangle plot_rectanglerot
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
#include&lt;libplot.h&gt;
void plot_setlinecolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to draw the polygon with when the <command>plot_strokeline</command> function is called.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setlinecolor(state, 255, 0, 0); // red
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_rectangle plot_rectanglerot
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

FUNCTION plot_rectangle
PURPOSE draw a rectangle

SYNOPSIS START
#include&lt;libplot.h&gt;
void plot_rectangle plot_rectanglerot (plot_state *state, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
SYNOPSIS END

DESCRIPTION Draw a rectangle on the plot. x1, y1 is the top left corner, and x2, y2 is the bottom right corner of the rectangle. After this call you can use <command>plot_strokeline</command> and <command>plot_fillline</command> to actaully affect the raster.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}
plot_rectangle(graph, 10, 10, 20, 30);
EXAMPLE END
SEEALSO plot_newplot plot_getraster plot_setlinestart plot_addlinesegment plot_addcubiccurvesegment plot_addquadraticcurvesegmentone plot_addquadraticcurvesegmenttwo plot_closeline plot_endline plot_strokeline plot_fillline plot_setlinewidth plot_setlinecap plot_setlinejoin plot_setlinedash plot_setfillcolor plot_setlinecolor plot_rectanglerot
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

void
plot_circle (plot_state * state, unsigned int x, unsigned int y, 
	     unsigned int r)
{

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
