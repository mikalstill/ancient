// Libplot, a simple C library to draw graphs into rasters. Many of the drawing
// commands here are modelled on those available in PDF...

#ifndef LIBPLOT_HEADER
#define LIBPLOT_HEADER

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

  // Line attributes
  plot_lineseg *line;

  int linewidth;
  int linecap;
  int linejoin;
  int linedash;
  plot_pixel fillcolor;
  plot_pixel linecolor;
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
void plot_setlinewidth (plot_state *, int);
void plot_setlinecap (plot_state *, int);
void plot_setlinejoin (plot_state *, int);
void plot_setlinedash (plot_state *, int, int, int);
void plot_setfillcolor (plot_state *, int, int, int);
void plot_setlinecolor (plot_state *, int, int, int);

// Operations based on primitive operations
void plot_rectangle (plot_state *, unsigned int, unsigned int, 
		     unsigned int, unsigned int);
void plot_circle (plot_state *, unsigned int, unsigned int, unsigned int);

// Internal methods
unsigned int plot_min (unsigned int one, unsigned int two);
unsigned int plot_max (unsigned int one, unsigned int two);

#ifdef __cplusplus
}
#endif

#endif
