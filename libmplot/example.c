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
#include <png.h>
#include <libmplot.h>

char *readimage(char *filename, unsigned long *width, unsigned long *height, 
		      int *bitdepth, int *channels);

char *inflateraster(char *input, unsigned long width, unsigned long height, 
                    int bitdepth, int targetbitdepth, 
                    int channels, int targetchannels);

int
main (int argc, char *argv[])
{
  FILE *image;
  unsigned int i;
  unsigned long gnux, gnuy;
  png_uint_32 rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  plot_state *graph;
  char *raster, *gnu, *newgnu;
  int gnubitdepth, gnuchannels;

  if((graph = plot_newplot(300, 200)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // Get another raster to use for testing
  if((gnu = readimage("gnu.png", &gnux, &gnuy, &gnubitdepth, &gnuchannels)) == -1){
    fprintf(stderr, "Cannot read the gnu.png test image\n");
    exit(42);
  }
  if((newgnu = inflateraster(gnu, gnux, gnuy, gnubitdepth, 8, gnuchannels, 3)) == -1){
    fprintf(stderr, "Cannot inflate the raster of the gnu image\n");
    exit(42);
  }
  
  // Shrink
  plot_overlayraster(graph, newgnu, 10, 10, 90, 90, gnux, gnuy);
  plot_setlinecolor(graph, 255, 0, 0);
  plot_rectangle(graph, 10, 10, 90, 90);
  plot_strokeline(graph);
  plot_endline(graph);

  // Expand
  plot_overlayraster(graph, newgnu, 100, 100, 900, 900, gnux, gnuy);
  plot_setlinecolor(graph, 255, 0, 0);
  plot_rectangle(graph, 100, 100, 900, 900);
  plot_strokeline(graph);
  plot_endline(graph);

  // A red line from 10, 10 to 20, 10
  plot_setlinecolor(graph, 255, 0, 0);
  plot_setlinestart(graph, 10, 10);
  plot_addlinesegment(graph, 20, 10);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinecolor(graph, 0, 255, 0);
  plot_rectangle(graph, 20, 20, 30, 40);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinecolor(graph, 0, 0, 255);
  plot_setlinestart(graph, 10, 10);
  plot_addlinesegment(graph, 20, 20);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinestart(graph, 20, 10);
  plot_addlinesegment(graph, 30, 30);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinecolor(graph, 255, 0, 0);
  plot_setlinestart(graph, 30, 30);
  plot_addlinesegment(graph, 50, 40);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinestart(graph, 30, 30);
  plot_addlinesegment(graph, 25, 25);
  plot_strokeline(graph);
  plot_endline(graph);

  plot_setlinestart(graph, 98, 100);
  plot_addlinesegment(graph, 102, 100);
  plot_strokeline(graph);
  plot_endline(graph);
  plot_setlinestart(graph, 100, 98);
  plot_addlinesegment(graph, 100, 102);
  plot_strokeline(graph);
  plot_endline(graph);
  
  plot_setlinecolor(graph, 0, 0, 0);
  plot_circle(graph, 100, 100, 50);
  plot_strokeline(graph);
  plot_endline(graph);
  
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
  png_set_IHDR (png, info, 300, 200, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (200 * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < 200; i++){
    row_pointers[i] = raster + (i * 300 * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
