// Libplot, a simple C library to draw graphs into rasters. Many of the drawing
// commands here are modelled on those available in PDF...

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#if defined HAVE_LIBFREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifndef LIBPLOT_HEADER
#define LIBPLOT_HEADER

#define LIBMPLOT_TRUE 0
#define LIBMPLOT_FALSE -1

#define LIBMPLOT_VERTICAL 0
#define LIBMPLOT_HORIZONTAL 1
#define LIBMPLOT_NOTAPPLICABLE 2

// These are used for automated conversion of this header to other languages
#define SUPPRESS(x)
#define INTERNAL

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct plot_internal_pixel
{
  unsigned char r, g, b;
}
plot_pixel;

typedef struct plot_internal_lineseg
{
  unsigned int x;
  unsigned int y;
  struct plot_internal_lineseg *next;
}
plot_lineseg;

typedef struct plot_internal_state
{
  plot_pixel *raster;
  unsigned int x;
  unsigned int y;
  unsigned long maxptr;

  // Line attributes
  plot_lineseg *line;

  int linewidthx, linewidthy;
  int linecap;
  int linejoin;

  int linedashlen;
  char *linedash;
  int linedashcount;

  plot_pixel fillcolor;
  plot_pixel linecolor;
  plot_pixel fontcolor;

  unsigned int textx;
  unsigned int texty;

  unsigned int penx;
  unsigned int peny;

  float ctm[6];

#if defined HAVE_LIBFREETYPE
  FT_Library *ft;
  FT_Face face;

  char *fontpath;
  int fontsize;
#endif
}
plot_state;

plot_state *plot_newplot (unsigned int, unsigned int);

// Primitive operations
void plot_setlinestart (plot_state *, unsigned int, unsigned int);
void plot_addlinesegment (plot_state *, unsigned int, unsigned int);
void plot_addcubiccurvesegment (plot_state *, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void plot_addquadraticcurvesegmentone (plot_state *, unsigned int, unsigned int, unsigned int, unsigned int);
void plot_addquadraticcurvesegmenttwo (plot_state *, unsigned int, unsigned int, unsigned int, unsigned int);
void plot_closeline (plot_state *);

// Things to do with a line once built
void plot_endline (plot_state *);
void plot_strokeline (plot_state *);
void plot_fillline (plot_state *);

// State modification functions
void plot_setlinewidth (plot_state *, int, int);
void plot_setlinecap (plot_state *, int);
void plot_setlinejoin (plot_state *, int);
void plot_setlinedash (plot_state *, int, char *);
void plot_setfillcolor (plot_state *, int, int, int);
void plot_setlinecolor (plot_state *, int, int, int);

// Font state functions
int plot_setfont(plot_state * state, char *font, int charsize);
void plot_setfontcolor (plot_state *, int, int, int);
void plot_settextlocation (plot_state *, unsigned int, unsigned int);
void plot_gettextlocation (plot_state *, unsigned int *, unsigned int *);

// Text operations
int plot_writestring(plot_state *state, char *string);
int plot_writestringrot(plot_state *state, char *string, float angle);
unsigned int plot_stringwidth(plot_state *state, char *string);
unsigned int plot_stringheight(plot_state *state, char *string);

// Operations based on primitive operations
void plot_rectangle (plot_state *, unsigned int, unsigned int, 
		     unsigned int, unsigned int);
void plot_circle (plot_state *, unsigned int, unsigned int, unsigned int);

// Raster operations
char *plot_getraster (plot_state * state);
void plot_overlayraster(plot_state * state, char *raster,
			unsigned int x1, unsigned int y1, 
			unsigned int x2, unsigned int y2, 
			unsigned int rx, unsigned int ry,
			int debug);

// Current Transformation Matrix
// [a b 0]
// [c d 0]
// [e f 1]
void plot_setctm (plot_state * state, float a, float b, float c,
		  float d, float e, float f);

// Persistance of state methods
char *plot_persiststate(plot_state * state);
void plot_applystate(plot_state * state, char *pstr);

// Internal methods
INTERNAL int plot_min (int one, int two);
INTERNAL int plot_max (int one, int two);
INTERNAL int plot_loadglyph(plot_state *, char);
INTERNAL int plot_paintglyph(plot_state *, char, int, int);
INTERNAL void plot_drawpoint(plot_state *, plot_pixel, 
			     int isLine, unsigned int, unsigned int);
INTERNAL void plot_drawpointactual(plot_state *, plot_pixel, 
				   int isLine, unsigned int, unsigned int);

#ifdef __cplusplus
}
#endif

#endif
