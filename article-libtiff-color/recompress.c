#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *image;
  uint16 photo, bps, spp, fillorder;
  uint32 width, height, *raster;
  tsize_t stripSize;
  unsigned long imagesize, c;

  // Open the TIFF image
  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Find the width and height of the image
  TIFFGetField(image, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(image, TIFFTAG_IMAGELENGTH, &height);
  imagesize = height * width + 1;
  if((raster = (uint32 *) malloc(sizeof(uint32) * imagesize)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  // Read the image into the memory buffer
  if(TIFFReadRGBAStrip(image, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  // Dump it to stdout
  for(c = 0; c < imagesize; c += 2){
    fprintf(stdout, "%c%c%c%c%c%c  ", TIFFGetR(raster[c]), TIFFGetG(raster[c]),
            TIFFGetB(raster[c]), TIFFGetR(raster[c + 1]), 
            TIFFGetG(raster[c + 1]), TIFFGetB(raster[c + 1]));
  }

  TIFFClose(image);
}
