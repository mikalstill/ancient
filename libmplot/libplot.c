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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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

  state->fontcolor.r = 0;
  state->fontcolor.g = 0;
  state->fontcolor.b = 0;

  state->line = (plot_lineseg *) NULL;

#if defined HAVE_LIBFREETYPE
  state->face = NULL;

  if(((state->ft = (FT_Library *) malloc(sizeof(FT_Library))) == NULL) ||
     FT_Init_FreeType(state->ft))
#endif
    state->ft = NULL;

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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
	  startx = plot_min(x1, current->x);
	  starty = startx == x1 ? y1 : current->y;
	  rise = starty == y1 ? (current->y - y1) : (y1 - current->y);
	  run = startx == x1 ? (current->x - x1) : (x1 - current->x);

	  for (c = startx; c < plot_max (x1, current->x) + 1; c++)
	    {
	      prevy = (int) newy;
	      newy = ((float) rise / run) * (c - startx) + starty;

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
plot_setlinewidth (plot_state * state, int w, int h)
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
#include&lt;libplot.h&gt;
void plot_setfontcolor (plot_state *state, int red, int green, int blue);
SYNOPSIS END

DESCRIPTION Set the colour to draw the text with

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfontcolor(state, 255, 0, 0); // red
EXAMPLE END
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
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

#define pi 3.1415

void
plot_circle (plot_state * state, unsigned int x, unsigned int y, 
	     unsigned int r)
{
  float angle;

  // todo: this could be more efficient
  // todo: this should be proportional to the radius
  plot_setlinestart(state, x + (int) (r * sin(angle)), 
		    y + (int) (r * cos(angle)));

  for(angle = 0.01; angle < 2 * pi; angle += 0.01){
    plot_addlinesegment(state, x + (int) (r * sin(angle)), 
			y + (int) (r * cos(angle)));
  }

  plot_strokeline(state);
  plot_endline(state);
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_setfont
PURPOSE sets the starting point of a curve

SYNOPSIS START
#include&lt;libplot.h&gt;
void panda_setfont (plot_state *state, char *fontpath, int size);
SYNOPSIS END

DESCRIPTION Set the font and size to use to text drawing operations. The <command>fontpath</command> arguement is the path to a font which <command>FreeType</command> is capable of opening (for example PostScript fonts, and TrueType fonts -- refer to http://www.freetype.org for more details). The size is the point size of the text to be drawn.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_setfont(graph, "windings.ttf", 30);

EXAMPLE END
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
DOCBOOK END
******************************************************************************/

void plot_setfont(plot_state * state, char *font, int charsize)
{
  int error;
  
#if defined HAVE_LIBFREETYPE
  if(state->ft == NULL){
    fprintf(stderr, "Initialization of Freetype failed\n");
    return;
  }

  // Load the face
  error = FT_New_Face(*state->ft, font, 0, &state->face );
  if ( error == FT_Err_Unknown_File_Format )
    {
      fprintf(stderr, "The specified font file could be opened and read, but it appears that its font format is unsupported by Freetpye\n");
      return;
    }
  else if ( error )
    {
      fprintf(stderr, "Could not load font\n");
      return;
    }

  // Set the text size
  if(FT_Set_Char_Size(state->face, 0, charsize * 64, 75, 75)){
    fprintf(stderr, "Could not set font size\n");
    return;
  }
#else
    fprintf(stderr, "Freetype not found at compile time\n");
#endif
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_settextlocation
PURPOSE sets the cursor location for text drawing

SYNOPSIS START
#include&lt;libplot.h&gt;
void panda_setlinestart (plot_state *state, unsigned int x, unsigned int y);
SYNOPSIS END

DESCRIPTION Set the cursor location for text drawing.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_settextlocation(graph, 30, 30);
EXAMPLE END
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
DOCBOOK END
******************************************************************************/

void plot_settextlocation(plot_state *state, unsigned int x, unsigned int y)
{
  state->textx = x;
  state->texty = y;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_settextlocation
PURPOSE sets the cursor location for text drawing

SYNOPSIS START
#include&lt;libplot.h&gt;
void panda_getlinestart (plot_state *state, unsigned int *x, unsigned int *y);
SYNOPSIS END

DESCRIPTION Get the cursor location for text drawing.

RETURNS Nothing

EXAMPLE START
#include&lt;libplot.h&gt;
plot_state *graph;
int x, y;

if((graph = plot_newplot(400, 300)) == NULL){
  ... error ...
}

plot_settextlocation(graph, 30, 30);

... draw some text...

plot_gettextlocation(graph, &amp;x, &amp;y);
EXAMPLE END
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
DOCBOOK END
******************************************************************************/

void plot_gettextlocation(plot_state *state, unsigned int *x, unsigned int *y)
{
  *x = state->textx;
  *y = state->texty;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_writestring
PURPOSE write a string onto the bitmap

SYNOPSIS START
#include&lt;libplot.h&gt;
int plot_writestring(plot_state *state, char *string);
SYNOPSIS END

DESCRIPTION This function writes a string onto the specified bitmap. Note that the function does not implement any form of word wrap. This is the responsibility of the caller of the function. Have a look at the example below for a suggested technique for implementing word wrap.

RETURNS 0 on success, -1 otherwise

EXAMPLE START
#include&lt;libplot.h&gt;
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
DOCBOOK END
******************************************************************************/

int plot_writestring(plot_state *state, char *string){
  int count, len;

  printf("WRITE STRING\n");

  len = strlen(string);
  for(count = 0; count < len; count++){
    if(plot_paintglyph(state, string[count], LIBPLOT_TRUE) == -1)
      return -1;
  }

  return 0;
}

/******************************************************************************
DOCBOOK START

FUNCTION plot_stringwidth
PURPOSE determine how many pixels a string will take

SYNOPSIS START
#include&lt;libplot.h&gt;
unsigned int plot_stringwidth(plot_state *state, char *string);
SYNOPSIS END

DESCRIPTION This function determines how many pixels a string will consume in the current font and size.

RETURNS 0 on success, -1 otherwise

EXAMPLE START
#include&lt;libplot.h&gt;
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
SEEALSO plot_addlinesegment.sgml plot_closeline.sgml plot_endline.sgml plot_getraster.sgml plot_newplot.sgml plot_rectangle.sgml plot_setfillcolor.sgml plot_setfontcolor.sgml plot_setfont.sgml plot_setlinecolor.sgml plot_setlinestart.sgml plot_settextlocation.sgml plot_stringwidth.sgml plot_strokeline.sgml plot_writestring.sgml
DOCBOOK END
******************************************************************************/

unsigned int plot_stringwidth(plot_state *state, char *string){
  int count, len, retval;
  unsigned int width = 0;

  printf("STRING WIDTH\n");

  len = strlen(string);
  for(count = 0; count < len; count++){
    if((retval = plot_paintglyph(state, string[count], LIBPLOT_FALSE)) == -1)
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
plot_loadglyph(plot_state *state, char character)
{
#if defined HAVE_LIBFREETYPE
  FT_UInt index;

  printf("Load glyph %c\n", character);

  if(state->ft == NULL){
    fprintf(stderr, "Initialization of Freetype failed\n");
    return -1;
  }

  if(state->face == NULL){
    fprintf(stderr, "No face loaded\n");
    return -1;
  }

  // Find the index of the glyph
  index = FT_Get_Char_Index(state->face, character);

  // Load that glyph into the face's slot
  if(FT_Load_Glyph(state->face, index, FT_LOAD_RENDER)){
    fprintf(stderr, "Could not load glyph for %c into face slot\n", character);
    return -1;
  }

#else
    fprintf(stderr, "Freetype not found at compile time\n");
    return -1;
#endif
}

#define sfgb state->face->glyph->bitmap

int
plot_paintglyph(plot_state *state, char character, int dopaint)
{
#if defined HAVE_LIBFREETYPE
  int bmx, bmy;
  unsigned long p;

  printf("Paint glyph %c at %d, %d\n", character, state->textx, state->texty);

  if(state->ft == NULL){
    fprintf(stderr, "Initialization of Freetype failed\n");
    return -1;
  }

  if(state->face == NULL){
    fprintf(stderr, "No face loaded\n");
    return -1;
  }

  // Setup and paint the character
  if(plot_loadglyph(state, character) != -1){
    if(dopaint == LIBPLOT_TRUE){
      p = state->texty * state->y + state->textx;
      p += state->face->glyph->bitmap_left;
      p -= state->face->glyph->bitmap_top * state->x;
      
      for(bmy = 0; bmy < sfgb.rows; bmy++){
	for(bmx = 0; bmx < sfgb.width; bmx++){
	  if(sfgb.buffer[bmy * sfgb.width + bmx] != 0){
	    state->raster[p + bmx].r = ~sfgb.buffer[bmy * sfgb.width + bmx] * state->fontcolor.r / 
	      ~sfgb.buffer[bmy * sfgb.width + bmx];
	    state->raster[p + bmx].g = ~sfgb.buffer[bmy * sfgb.width + bmx] * state->fontcolor.g / 
	      ~sfgb.buffer[bmy * sfgb.width + bmx];
	    state->raster[p + bmx].b = ~sfgb.buffer[bmy * sfgb.width + bmx] * state->fontcolor.b / 
	      ~sfgb.buffer[bmy * sfgb.width + bmx];
	  }
	}
	p += state->x;
      }
      
      // Increment pen position
      state->textx += state->face->glyph->advance.x >> 6;
      state->texty += state->face->glyph->advance.y >> 6;
    }
    else{
      return state->face->glyph->advance.x >> 6;
    }

    return 0;
  }

  return -1;
#else
    fprintf(stderr, "Freetype not found at compile time\n");
    return -1;
#endif
}
