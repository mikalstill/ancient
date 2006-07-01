#include <stdlib.h>
#include <tiffio.h>
#include <stdio.h>

// NB: This code _assumes_ that BPS is always 8...

int main(int argc, char *argv[]){
  TIFF *input, *output;
  uint16 compression, planar, photo, bps, spp;
  uint32 width, height;
  uint32 *raster;
  int w, h, inset;
  char *raster2;

  // Check the command line arguements
  if(argc != 3){
    printf("Usage: %s <inputfilename> <outputfilename>\n", argv[0]);
    exit(1);
  }
   
  // Open the input image
  if((input = TIFFOpen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open input image\n");
    exit(42);
  }

  // Open the output image
  if((output = TIFFOpen(argv[2], "w")) == NULL){
    fprintf(stderr, "Could not open output image\n");
    exit(42);
  }

  // Get the tiff tags for the input file
  TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(input, TIFFTAG_COMPRESSION, &compression);
  TIFFGetField(input, TIFFTAG_PLANARCONFIG, &planar);
  TIFFGetField(input, TIFFTAG_PHOTOMETRIC, &photo);
  TIFFGetField(input, TIFFTAG_BITSPERSAMPLE, &bps);
  TIFFGetField(input, TIFFTAG_SAMPLESPERPIXEL, &spp);  

  // Read the image into the memory buffer
  printf("%dx%d image\n", width, height);
  printf("BPS = %d, SPP = %d\n", bps, spp);
  printf("Planar = %d\n", planar);
  if((raster = (uint32 *) malloc((width * height + 1) * 4)) == NULL){
    printf("Malloc for raster failed\n");
    exit(42);
  }
  
  if(TIFFReadRGBAStrip(input, 0, raster) == 0){
    fprintf(stderr, "Could not read image\n");
    exit(42);
  }

  if((raster2 = (char *) malloc((width * height + 1 ) * 3)) == NULL){
    printf("Malloc for converted raster failed\n");
    exit(42);
  }

  // Convert the raster
  for(w = 0; w < width; w++){
    for(h = 0; h < height; h++){
      raster2[((height - h - 1) * width + w) * 3] = 
	TIFFGetR(raster[h * width + w]);
      raster2[((height - h - 1) * width + w) * 3 + 1] = 
	TIFFGetG(raster[h * width + w]);
      raster2[((height - h - 1) * width + w) * 3 + 2] = 
	TIFFGetB(raster[h * width + w]);
    }
  }

  // Write the tiff tags to the file
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(output, TIFFTAG_COMPRESSION, compression);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, planar);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  // Actually write the image
  if(TIFFWriteEncodedStrip(output, 0, raster2, width * height * 3) == 0){
    fprintf(stderr, "Could not write image\n");
    exit(42);
  }

  TIFFClose(input);
  TIFFClose(output);
}
