#include <stdio.h>
#include <tiffio.h>

int main(int argc, char *argv[]){
  TIFF *image, *output;
  uint16 photo, bps, spp, fillorder;
  uint32 width, height, *raster;
  tsize_t stripSize;
  unsigned long imagesize, c, d;
  char *raster2;

  // Open the TIFF image
  if((image = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open incoming image\n");
    exit(42);
  }

  // Open the output image
  if((output = TIFFOpen(argv[2], "w")) == NULL){
    fprintf(stderr, "Could not open outgoing image\n");
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

  if((raster2 = (char *) malloc(sizeof(char) * imagesize * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  // Read the image into the memory buffer
  if(TIFFReadRGBAStrip(image, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  d = 0;
  for(c = 0; c < imagesize; c++){
    raster2[d++] = TIFFGetR(raster[c]);
    raster2[d++] = TIFFGetG(raster[c]);
    raster2[d++] = TIFFGetB(raster[c]);
  }

  // Recompress it straight away -- set the tags we require
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_JPEG);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);
  TIFFSetField(output, TIFFTAG_JPEGQUALITY, 25);

  // Actually write the image
  if(TIFFWriteEncodedStrip(output, 0, raster2, imagesize * 3) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  TIFFClose(output);
  TIFFClose(image);
}
