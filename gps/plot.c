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
#include <unistd.h>

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

void usage(){
  printf ("Try: -x <minx> -y <miny> -X <maxx> -Y <maxy> -w <mapwidth> -h <mapheight>\n");
  exit (0);
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

  unsigned long minx = 0, maxx = 0, miny = 0, maxy = 0, 
    mapx = 0, mapy = 0, x, y, prevx = 0, prevy = 0, s;
  int datapts = 0;
  char line[1000];
  int speed, optchar;
  float scalex, scaley;

  // Command line options
  while ((optchar = getopt (argc, argv, "x:y:X:Y:w:h:")) != -1){
    switch (optchar){
    case 'x':
      minx = atol(optarg);
      break;

    case 'y':
      miny = atol(optarg);
      break;
      
    case 'X':
      maxx = atol(optarg);
      break;

    case 'Y':
      maxy = atol(optarg);
      break;
      
    case 'w':
      mapx = atol(optarg);
      break;

    case 'h':
      mapy = atol(optarg);
      break;

    default:
    case '?':
      usage();
      break;
    }
  }
  
  if(minx == 0) usage();
  if(miny == 0) usage();
  if(maxx == 0) usage();
  if(maxy == 0) usage();
  if(mapx == 0) usage();
  if(mapy == 0) usage();

  // Determine scale
  minx -= 5000;
  maxx += 5000;
  miny -= 5000;
  maxy += 5000;
  scalex = (maxx - minx) / mapx;
  scaley = (maxy - miny) / mapy;
  scalex += 7;
  scaley += 7;
  printf("Scaling: %f and %f\n", scalex, scaley);

  if((graph = plot_newplot(mapx, mapy)) == NULL){
    fprintf(stderr, "Could not allocate a new plot\n");
    exit(1);
  }

  // Read the data points from stdin
  while(fgets(&line, 1000, stdin) != NULL){
    y = atol(strtok(line, " "));
    x = atol(strtok(NULL, " "));
    s = atol(strtok(NULL, " ")) / 100000;
    
    if((x == prevx) && (y == prevy)) continue;
    
    // Mapping stopped things isn't very interesting
    if(s == 0){
      continue;
    }

    // A gap in the data
    if((prevx != 0) && ((abs(x - prevx) > 500) ||
			(abs(y - prevy) > 500))){
      prevx = x;
      prevy = y;
      continue;
    }

    if(datapts == 0){
      plot_setlinecolor(graph, rconv(s), 0, bconv(s));
      plot_setlinestart(graph, (x - minx) / scalex, (y - miny) / scaley);
    }
    else if((rconv(s) == rconv(speed)) && 
	    ((bconv(s) == bconv(speed)))){
      plot_addlinesegment(graph, (x - minx) / scalex, (y - miny) / scaley);
    }
    else{
      plot_strokeline(graph);
      plot_endline(graph);
      plot_setlinecolor(graph, rconv(s), 0, bconv(s));
      plot_setlinestart(graph, (prevx - minx) / scalex, 
			(prevy - miny) / scaley);
      plot_addlinesegment(graph, (x - minx) / scalex, (y - miny) / scaley);
    }
    
    speed = s;
    prevx = x;
    prevy = y;
    
    datapts++;
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
  png_set_IHDR (png, info, mapx, mapy, 8, PNG_COLOR_TYPE_RGB,
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                PNG_FILTER_TYPE_DEFAULT);
  png_write_info (png, info);

  // Write the image out
  if((row_pointers = malloc (mapy * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  for(i = 0; i < mapy; i++){
    row_pointers[i] = raster + (i * mapx * 3);
  }

  png_write_image (png, row_pointers);

  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image); 
}
