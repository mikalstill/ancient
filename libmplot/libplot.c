#include <stdio.h>
#include <string.h>

#include <libplot.h>

plot_state *
plot_newplot (unsigned int x, unsigned int y)
{
  plot_state *state;

  state = malloc(sizeof(plot_state));
  if(state == NULL) return state;

  // A blank raster
  state->raster = malloc(3 * x * y);
  if(state->raster == NULL){
    free(state);
    return NULL;
  }
  memset(state->raster, 255, 3 * x * y);

  state->x = x;
  state->y = y;

  // Line attributes
  state->linewidth = 1;
  state->linecap = 0;
  state->linejoin = 0;
  state->linedash = 0;
  state->fillcolor = 0;
  state->linecolor = 0;

  state->line = NULL;
}

// Primitive operations
void
plot_setlinestart (plot_state *state, int x, int y)
{
  // todo: free old line...
  state->line = malloc(sizeof(plot_lineseg));
  if(state->line != NULL)
    {
      state->line->next = NULL;
    }

  state->line->x = x;
  state->line->y = y;
}

void
plot_addlinesegment (plot_state *state, int x, int y)
{
  // Find the end of the line
  plot_lineseg *current;
  current = state->line;

  while(current->next != NULL)
    current = current->next;

  current->next = malloc(sizeof(plot_lineseg));
  if(current->next == NULL)
    {
      fprintf(stderr, "An error we should handle better\n");
      exit(42);
    }
  
  current = current->next;
  current->next = NULL;
  current->x = x;
  current->y = y;
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
