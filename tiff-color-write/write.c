#include <tiffio.h>
#include <stdio.h>

unsigned char hex2decimal(int c, int d){
  unsigned char rc = 0;

  if(isdigit(c)){
    rc = c - '0';
    rc = rc << 4;
  }
  else{
    rc = c - 'a' + 10;
    rc = rc << 4;
  }

  if(isdigit(d)){
    rc += d - '0';
  }
  else{
    rc += d - 'a' + 10;
  }

  return rc;
}

int main(int argc, char *argv[]){
  TIFF *output;
  uint32 width, height;
  char *raster;
  int c, d, inset;

  // Open the output image
  if((output = TIFFOpen("output.tif", "w")) == NULL){
    fprintf(stderr, "Could not open outgoing image\n");
    exit(42);
  }

  // We need to know the width and the height before we can malloc
  width = 352;
  height = 288;

  if((raster = (char *) malloc(sizeof(char) * width * height * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  // Magical stuff for creating the image
  memset(raster, sizeof(char) * width * height * 3, 0);
  inset = 0;
  while(((c = fgetc(stdin)) != EOF) && (inset < width * height * 3 - 1)){
    if(c != '\n'){
      d = fgetc(stdin);

      raster[inset] = hex2decimal(c, d);
      raster[inset + 1] = 0;
      raster[inset + 2] = 0;
      inset += 3;
    }
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
