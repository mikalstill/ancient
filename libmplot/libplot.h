// Libplot, a simple C library to draw graphs into rasters. Many of the drawing
// commands here are modelled on those available in PDF...

typedef struct plot_internal_state
{
  void *raster;
  unsigned int x;
  unsigned int y;

  // Line attributes
  int linewidth;
  int linecap;
  int linejoin;
  int linedash;
  int fillcolor;
  int linecolor;
}
plot_state;

plot_state *plot_newplot (unsigned int x, unsigned int y);

// Primitive operations
void plot_setlinestart (plot_state *, int, int);
void plot_addlinesegment (plot_state *, int, int);
void plot_addcubiccurvesegment (plot_state *, int, int, int, int, int, int);
void plot_addquadraticcurvesegmentone (plot_state *, int, int, int, int);
void plot_addquadraticcurvesegmenttwo (plot_state *, int, int, int, int);
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
void plot_rectangle (plot_state *, int, int, int, int);
