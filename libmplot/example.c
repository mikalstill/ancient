#include <stdio.h>
#include <libplot.h>

int
main (int argc, char *argv[])
{
  plot_state *graph;

  if((graph = plot_newplot(300, 200)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  
}
