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

// The speed range is considered to be 0 - 150 kmh
int rconv(long speed){
  // The top half of the speed range are reds
  return 105 + speed;
}

int bconv(long speed){
  // The bottom half are blues
  return 255 - speed;
}


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

  unsigned long mapptx[100000];
  unsigned long mappty[100000];
  unsigned long speeds[100000];
  unsigned long minx = -1, maxx = 0, miny = -1, maxy = 0, rangex, rangey;
  unsigned long x, y, s;
  int datapts = 0;
  char line[1000];
  int speed;

  // Read the data points from stdin
  while(fgets(&line, 1000, stdin) != NULL){
    y = atol(strtok(line, " ")) / 250;
    x = atol(strtok(NULL, " ")) / 250;
    s = atol(strtok(NULL, " ")) / 100000;

    // If we're at warp speed, then don't believe the point
    if((datapts == 0) || 
       ((abs(x - mapptx[datapts - 1]) < 15) &&
	(abs(y - mappty[datapts - 1]) < 15))){
      mapptx[datapts] = x;
      mappty[datapts] = y;
      speeds[datapts] = s;

      if(mapptx[datapts] > maxx) maxx = mapptx[datapts];
      if(mapptx[datapts] < minx) minx = mapptx[datapts];
      if(mappty[datapts] > maxy) maxy = mappty[datapts];
      if(mappty[datapts] < miny) miny = mappty[datapts];
    
      datapts++;
    }
  }

  rangex = maxx - minx;
  rangey = maxy - miny;

  printf("Plot extents:\n");
  printf("  %d - %d (range %d)\n", minx, maxx, rangex);
  printf("  %d - %d (range %d)\n", miny, maxy, rangey);

  rangex += 20;
  rangey += 20;

  printf("  %d pixels\n", rangex * rangey);
  printf("  150 kmh = %d %d %d\n", rconv(150), 0, bconv(150));
  printf("    0 kmh = %d %d %d\n", rconv(0), 0, bconv(0));

  minx -= 10;
  miny -= 10;

  if((graph = plot_newplot(rangex, rangey)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  plot_setlinecolor(graph, rconv(speeds[i]), 0, bconv(speeds[i]));
  plot_setlinestart(graph, mapptx[0] - minx, mappty[0] - miny);
  speed = speeds[1];

  for(i = 1; i < datapts; i++){
    if(speeds[i] == speed)
      plot_addlinesegment(graph, mapptx[i] - minx, mappty[i] - miny);
    else{
      plot_strokeline(graph);
      plot_endline(graph);
      plot_setlinecolor(graph, rconv(speeds[i]), 0, bconv(speeds[i]));
      plot_setlinestart(graph, mapptx[i - 1] - minx, mappty[i - 1] - miny);
      plot_addlinesegment(graph, mapptx[i] - minx, mappty[i] - miny);
    }

    speed = speeds[i];
  }
  plot_strokeline(graph);
  plot_endline(graph);
  
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
