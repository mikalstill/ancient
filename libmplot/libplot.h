// Libplot, a simple C library to draw graphs into rasters. Many of the drawing
// commands here are modelled on those available in PDF...

#if defined HAVE_LIBFREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifndef LIBPLOT_HEADER
#define LIBPLOT_HEADER

#define LIBPLOT_TRUE 0
#define LIBPLOT_FALSE -1

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
  int linedash;
  plot_pixel fillcolor;
  plot_pixel linecolor;
  plot_pixel fontcolor;

  unsigned int textx;
  unsigned int texty;

  unsigned int penx;
  unsigned int peny;

#if defined HAVE_LIBFREETYPE
  FT_Library *ft;
  FT_Face face;
#endif
}
plot_state;

plot_state *plot_newplot (unsigned int, unsigned int);
char *plot_getraster (plot_state * state);

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
void plot_setlinedash (plot_state *, int, int, int);
void plot_setfillcolor (plot_state *, int, int, int);
void plot_setlinecolor (plot_state *, int, int, int);

// Font state functions
void plot_setfont(plot_state * state, char *font, int charsize);
void plot_setfontcolor (plot_state *, int, int, int);
void plot_settextlocation (plot_state *, unsigned int, unsigned int);
void plot_gettextlocation (plot_state *, unsigned int *, unsigned int *);

// Text opeations
int plot_writestring(plot_state *state, char *string);
int plot_writestringrot(plot_state *state, char *string, float angle);
unsigned int plot_stringwidth(plot_state *state, char *string);

// Operations based on primitive operations
void plot_rectangle (plot_state *, unsigned int, unsigned int, 
		     unsigned int, unsigned int);
void plot_circle (plot_state *, unsigned int, unsigned int, unsigned int);

// Internal methods
unsigned int plot_min (unsigned int one, unsigned int two);
unsigned int plot_max (unsigned int one, unsigned int two);
int plot_loadglyph(plot_state *, char);
int plot_paintglyph(plot_state *, char, int);
void plot_drawpoint(plot_state *, unsigned int, unsigned int);
void plot_drawpointactual(plot_state *, unsigned int, unsigned int);

#ifdef __cplusplus
}
#endif

#endif
