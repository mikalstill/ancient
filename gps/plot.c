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
  char *raster;

  unsigned long mapptx[10000];
  unsigned long mappty[10000];
  unsigned long speed[10000];
  unsigned long minx = -1, maxx = 0, miny = -1, maxy = 0, rangex, rangey;
  int datapts = 0;
  char line[1000];

  // Read the data points from stdin
  while(fgets(&line, 1000, stdin) != NULL){
    mapptx[datapts] = atol(strtok(line, " ")) / 100;
    mappty[datapts] = atol(strtok(NULL, " ")) / 100;
    speed[datapts] = atol(strtok(NULL, " ")) / 1000;

    if(mapptx[datapts] > maxx) maxx = mapptx[datapts];
    if(mapptx[datapts] < minx) minx = mapptx[datapts];
    if(mappty[datapts] > maxy) maxy = mappty[datapts];
    if(mappty[datapts] < miny) miny = mappty[datapts];
    
    datapts++;
  }

  rangex = maxx - minx;
  rangey = maxy - miny;

  printf("Plot extents:\n");
  printf("  %d - %d (range %d)\n", minx, maxx, rangex);
  printf("  %d - %d (range %d)\n", miny, maxy, rangey);

  rangex += 20;
  rangey += 20;
  minx -= 10;
  miny -= 10;

  if((graph = plot_newplot(rangex, rangey)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  plot_setfontcolor(graph, 26, 22, 249);
  plot_setfont(graph, "n019004l.pfb", 18);
  plot_settextlocation(graph, mapptx[0] - minx - 5, mappty[0] - miny - 5);
  plot_writestring(graph, "S");

  plot_setlinecolor(graph, 0, 0, 255);
  plot_setlinestart(graph, mapptx[0] - minx, mappty[0] - miny);
  printf("[%d, %d]", mapptx[0] - minx, mappty[0] - miny);
  for(i = 1; i < datapts; i++){
    plot_addlinesegment(graph, mapptx[i] - minx, mappty[i] - miny);
    printf("[%d, %d]", mapptx[i] - minx, mappty[i] - miny);
  }
  plot_strokeline(graph);
  plot_endline(graph);

  plot_settextlocation(graph, mapptx[i - 1] - minx - 5, 
		       mappty[i - 1] - miny - 5);
  plot_writestring(graph, "E");
  
  raster = plot_getraster(graph);

  if((image = fopen("map.png", "wb")) == NULL){
    fprintf(stderr, "Could not open map.png\n");
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
  png_set_IHDR (png, info, rangex, rangey, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (rangey * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < rangey; i++){
    row_pointers[i] = raster + (i * rangex * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
