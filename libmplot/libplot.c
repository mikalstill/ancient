#include <stdio.h>
#include <libplot.h>

plot_state *
plot_newplot (unsigned int x, unsigned int y)
{

}

// Primitive operations
void
plot_setlinestart (plot_state *state, int x, int y)
{
  fprintf (stderr, "todo\n");
}

void
plot_addlinesegment (plot_state *state, int x, int y)
{
  fprintf (stderr, "todo\n");
}

void
plot_addcubiccurvesegment (plot_state *state, int x1, int y1, int x2, int y2, 
			   int x3, int y3)
{
  fprintf (stderr, "todo\n");
}

void
plot_addquadraticcurvesegmentone (plot_state *state, int x1, int y1, 
				  int x2, int y2)
{
  fprintf (stderr, "todo\n");
}

void
plot_addquadraticcurvesegmenttwo (plot_state *state, int x1, int y1, 
				  int x2, int y2)
{
  fprintf (stderr, "todo\n");
}

void
plot_closeline (plot_state *state)
{
  fprintf (stderr, "todo\n");
}

///////////////////////////////////////////////////////////////////////////////
// Things to do with a line once built
void
plot_endline (plot_state *state)
{
  fprintf (stderr, "todo\n");
}

void
plot_strokeline (plot_state *state)
{
  fprintf (stderr, "todo\n");
}

void
plot_fillline (plot_state *state)
{
  fprintf (stderr, "todo\n");
}

// State modification functions
void
plot_setlinewidth (plot_state *state, int w)
{
  fprintf (stderr, "todo\n");
}

void
plot_setlinecap (plot_state *state, int c)
{
  fprintf (stderr, "todo\n");
}

void
plot_setlinejoin (plot_state *state, int j)
{
  fprintf (stderr, "todo\n");
}

void
plot_setlinedash (plot_state *state, int on, int off, int interval)
{
  fprintf (stderr, "todo\n");
}

void
plot_setfillcolor (plot_state *state, int red, int green, int blue)
{
  fprintf (stderr, "todo\n");
}

void
plot_setlinecolor (plot_state *state, int red, int green, int blue)
{
  fprintf (stderr, "todo\n");
}

///////////////////////////////////////////////////////////////////////////////
// Operations based on primitive operations
void
plot_rectangle (plot_state *state, int x1, int y1, int x2, int y2)
{
  fprintf (stderr, "todo\n");
}
