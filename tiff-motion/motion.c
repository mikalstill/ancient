#include <tiffio.h>
#include <stdio.h>

int main(int argc, char *argv[]){
  TIFF *output;
  uint32 width, height;
  unsigned char *raster;
  char filename[100];
  int x, y, count, xdelta, ydelta;

  // We need to know the width and the height before we can malloc
  width = 84;
  height = 42;
  x = 5;
  y = 20;
  xdelta = ydelta = 1;

  if((raster = (char *) malloc(sizeof(char) * width * height * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }
  memset(raster, 0, width * height * 3);

  for(count = 0; count < atoi(argv[1]); count++){
    // memset(raster, 0, width * height * 3);
    
    // Track the object
    raster[(y * width + x) * 3] = 255;
    raster[(y * width + x) * 3 + 1] = 255;
    raster[(y * width + x) * 3 + 2] = 255;
    
    x += xdelta;
    y += ydelta;

    if((x == (width - 1)) || (x == 1))
      xdelta = -xdelta;
    if((y == (height - 1)) || (y == 1))
      ydelta = -ydelta;

    // Open the output image
    snprintf(filename, 100, "output-%08d.tif", count);
    if((output = TIFFOpen(filename, "w")) == NULL){
      fprintf(stderr, "Could not open outgoing image\n");
      exit(42);
    }
    
    // Write the tiff tags to the file
    TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
    TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);
    
    // Actually write the image
    if(TIFFWriteEncodedStrip(output, 0, raster, width * height * 3) == 0){
      fprintf(stderr, "Could not write image\n");
      exit(42);
    }
    
    TIFFClose(output);
  }
}
