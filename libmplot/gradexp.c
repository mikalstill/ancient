/******************************************************************************
  Copyright (C) Michael Still 2002, 2004
  
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
#include <png.h>
#include <libmplot.h>

#define GRADIENTSIDELENGTH 512
#define WITHGRAPHWIDTH (GRADIENTSIDELENGTH * 2 + 20)

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  char *raster;
  int imgwidth;

  if(argc != 4){
    printf("Usage: %s <top control point> <bottom control point> <draw curve: yes / no>\n", argv[0]);
    exit(1);
  }

  if(strcmp(argv[3], "yes") == 0)
    imgwidth = WITHGRAPHWIDTH;
  else
    imgwidth = GRADIENTSIDELENGTH;

  if((graph = plot_newplot(imgwidth, GRADIENTSIDELENGTH)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // Experimental code to play with drawing gradients
  {
    unsigned int inx1 = GRADIENTSIDELENGTH;
    unsigned int iny1 = GRADIENTSIDELENGTH;
    unsigned int inx2 = atoi(argv[1]);
    unsigned int iny2 = 0;
    unsigned int inx3 = atoi(argv[2]);
    unsigned int iny3 = GRADIENTSIDELENGTH;

    double t;
    float calcx, calcy, tstep;
    float finx0 = graph->penx, finx1 = inx2, finx2 = inx3, finx3 = inx1;
    float finy0 = graph->peny, finy1 = iny2, finy2 = iny3, finy3 = iny1;
    
    float intensity[GRADIENTSIDELENGTH + 1];
    int i, j;

    plot_setlinestart(graph, 0, 0);

    // todo: maths to make this an efficient choice (for the value of tstep)...
    tstep = 0.001;
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

	if(imgwidth == WITHGRAPHWIDTH)
	  plot_addlinesegment(graph, 
			      (unsigned int) calcx, 
			      (unsigned int) calcy);
	
	printf("%f ", calcx);
	for(j = (unsigned int) calcy; j < GRADIENTSIDELENGTH; j++)
	  {
	    intensity[j] = calcx / GRADIENTSIDELENGTH * 255;
	  }
      }

    if(imgwidth == WITHGRAPHWIDTH){
      plot_strokeline(graph);
      plot_endline(graph);
    }

    for(i = 0; i < GRADIENTSIDELENGTH + 1; i++)
      {
	plot_setlinecolor(graph, 
			  (unsigned int) (255.0 - intensity[i]), 
			  0,
			  (unsigned int) intensity[i]);
	plot_setlinestart(graph, imgwidth - GRADIENTSIDELENGTH, i);
	plot_addlinesegment(graph, imgwidth, i);
	plot_strokeline(graph);
	plot_endline(graph);
      }
  }

  // Write the finished raster to a PNG file
  raster = plot_getraster(graph);

  if((image = fopen("output.png", "wb")) == NULL){
    fprintf(stderr, "Could not open the output image\n");
    exit(42);
  }

  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
                                NULL)) == NULL){
    fprintf(stderr, "Could not create write structure for PNG (out of memory?)");
    exit(42);
  }

  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL){
    fprintf(stderr,
            "Could not create PNG info structure for writing (out of memory?)");
    exit(42);
  }

  if (setjmp (png_jmpbuf (png))){
    fprintf(stderr, "Could not set the PNG jump value for writing");
    exit(42);
  }

  // This is needed before IO will work (unless you define callbacks)
  png_init_io(png, image);

  // Define important stuff about the image
  info->channels = 3;
  info->pixel_depth = 24;
  png_set_IHDR (png, info, imgwidth, GRADIENTSIDELENGTH, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (imgwidth * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < GRADIENTSIDELENGTH; i++){
    row_pointers[i] = raster + (i * imgwidth * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
