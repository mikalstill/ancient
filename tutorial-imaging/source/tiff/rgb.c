#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>

void usage(char *, int);

int main(int argc, char *argv[]){
  TIFF *input, *output;
  uint32 width, height, offset, xs = 0, ys = 0, xe = 10, ye = 10 ;
  tsize_t stripSize, stripNumber;
  unsigned long inputsize, x, y;
  char *inputFilename = NULL, *outputFilename = NULL,  *raster, *roff, *rout, 
    optchar;

  // Parse the command line options
  while((optchar = getopt(argc, argv, "i:o:x:y:w:l:")) != -1){
    switch(optchar){
    case 'i':
      inputFilename = (char *) strdup(optarg);
      break;

    case 'o':
      outputFilename = (char *) strdup(optarg);
      break;

    case 'x':
      xs = atoi(optarg);
      break;

    case 'y':
      ys = atoi(optarg);
      break;

    case 'w':
      xe = xs + atoi(optarg);
      break;

    case 'l':
      ye = ys + atoi(optarg);
      break;

    default:
      usage(argv[0], 0);
      break;
    }
  }

  // Open the input TIFF image
  if((inputFilename == NULL) ||
     (input = TIFFOpen(inputFilename, "r")) == NULL){
    fprintf(stderr, "Could not open incoming input %s\n", inputFilename);    
    usage(argv[0], 42);
  }

  // Open the output TIFF input
  if((outputFilename == NULL) ||
     (output = TIFFOpen(outputFilename, "w")) == NULL){
    fprintf(stderr, "Could not open outgoing input %s\n", outputFilename);
    usage(argv[0], 42);
  }
  
  // Find the width and height of the input
  TIFFGetField(input, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(input, TIFFTAG_IMAGELENGTH, &height);
  inputsize = height * width + 1;
  
  // Sanity check some of our arguements
  if(xe > width){
    fprintf(stderr, "You choice of starting x position, or width, results in the enlargement falling off the edge of the input image\n");
    usage(argv[0], 43);
  }
  if(ye > height){
    fprintf(stderr, "You choice of starting y position, or length, results in the enlargement falling off the end of the input image\n");
    usage(argv[0], 43);
  }

  // Grab some memory
  if((raster = (char *) malloc(sizeof(char) * inputsize * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory for input raster\n");
    exit(42);
  }

  // todo: crap assumption about the data being 8 bps, 3 spp
  if((rout = (char *) malloc(sizeof(char) * inputsize * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory for output raster\n");
    exit(42);
  }

  // todo: We need to copy tags from the input image to the output image
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  // todo: balance this off with having 8 kb per strip...
  TIFFSetField(output, TIFFTAG_ROWSPERSTRIP, 100000);
  
  // Read the input into the memory buffer
  // todo: I couldn't use TIFFReadRGBAStrip here, because it gets confused
  stripSize = TIFFStripSize(input);
  roff = raster;
  for(stripNumber = 0; stripNumber < TIFFNumberOfStrips(input); stripNumber++){
    roff += TIFFReadEncodedStrip(input, stripNumber, roff, stripSize);
  }

  // Here I fix the reversal of the input (vertically) and show you
  // how to get the color values from each pixel
  offset = 0;
  for(y = height - 1; y != -1; y--){
    for(x = 0; x < width; x++){
      rout[offset] = raster[offset++]; // Red
      rout[offset] = raster[offset++]; // Green
      rout[offset] = raster[offset++]; // Blue
    }
  }

  // Write the image buffer to the file
  printf("Writing %d bytes\n", width * height * 3 * sizeof(char));
  if(TIFFWriteEncodedStrip(output, 0, rout, 
			   width * height * 3 * sizeof(char)) == 0){
    fprintf(stderr, "Could not write the output image\n");
    exit(42);
  }

  // Cleanup
  TIFFClose(input);
  TIFFClose(output);
  free(raster);
  free(rout);
}

void usage(char *cmd, int exitamt){
  fprintf(stderr, "Bad command line arguements...\n\n");
  fprintf(stderr, "Usage: %s -i <inputfile> -o <outputfile>\n", cmd);
  exit(exitamt);
}
