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

int rconv(long speed){
  if(speed < 5) return 0;
  if(speed < 15) return 20;
  if(speed < 25) return 40;
  if(speed < 35) return 60;
  if(speed < 45) return 80;
  if(speed < 55) return 100;
  if(speed < 65) return 120;
  if(speed < 75) return 140;
  if(speed < 85) return 160;
  if(speed < 95) return 180;
  if(speed < 105) return 200;
  if(speed < 115) return 220;
  return 240;
}

int bconv(long speed){
  if(speed < 5) return 240;
  if(speed < 15) return 220;
  if(speed < 25) return 200;
  if(speed < 35) return 180;
  if(speed < 45) return 160;
  if(speed < 55) return 140;
  if(speed < 65) return 120;
  if(speed < 75) return 100;
  if(speed < 85) return 80;
  if(speed < 95) return 60;
  if(speed < 105) return 40;
  if(speed < 115) return 20;
  return 0;
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
  unsigned long minx = 596115, maxx = 597072, miny = 140587, maxy = 141115, rangex = 957, rangey = 528;
  unsigned long x, y, prevx = 0, prevy = 0, s;
  int datapts = 0;
  char line[1000];
  int speed;

  if((graph = plot_newplot(rangex, rangey)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // Read the data points from stdin
  while(fgets(&line, 1000, stdin) != NULL){
    y = atol(strtok(line, " ")) / 250;
    x = atol(strtok(NULL, " ")) / 250;
    s = atol(strtok(NULL, " ")) / 100000;
    
    if((x == prevx) && (y == prevy)) continue;
    
    if(prevx != 0){
      if(abs(x - prevx) > 20) continue;
      if(abs(y - prevy) > 20) continue;
    }

    if(datapts == 0){
      printf("S[%d %d] ", x - minx, y - miny);
      plot_setlinecolor(graph, rconv(s), 0, bconv(s));
      plot_setlinestart(graph, x - minx, y - miny);
    }
    else if((rconv(s) == rconv(speed)) && 
	    ((bconv(s) == bconv(speed)))){
      printf("[%d %d] ", x - minx, y - miny);
      plot_addlinesegment(graph, x - minx, y - miny);
    }
    else{
      printf("C[%d %d] ", x - minx, y - miny);
      plot_strokeline(graph);
      plot_endline(graph);
      plot_setlinecolor(graph, rconv(s), 0, bconv(s));
      plot_setlinestart(graph, prevx - minx, prevy - miny);
      plot_addlinesegment(graph, x - minx, y - miny);
    }
    
    speed = s;
    prevx = x;
    prevy = y;
    
    datapts++;
  }
  
  plot_strokeline(graph);
  plot_endline(graph);
  printf("\n\n");

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
