#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>
#include <string.h>

void usage (char *, int);

int
main (int argc, char *argv[])
{
  TIFF *input, *output;
  uint32 width, height;
  tsize_t stripSize, stripNumber;
  unsigned long x, y;
  char *inputFilename = NULL, *outputFilename = NULL, *raster, *roff, optchar;
  int count = 4, i;

  /////////////////////////////////////////////////////////////////////////////
  // Parse the command line options
  while ((optchar = getopt (argc, argv, "i:o:c:")) != -1)
    {
      switch (optchar)
	{
	case 'i':
	  inputFilename = (char *) strdup (optarg);
	  break;

	case 'o':
	  outputFilename = (char *) strdup (optarg);
	  break;

	case 'c':
	  count = atoi(optarg);
	  break;

	default:
	  usage(argv[0], 0);
	  break;
	}
    }

  // Open the input TIFF image
  if ((inputFilename == NULL) ||
      (input = TIFFOpen (inputFilename, "r")) == NULL)
    {
      fprintf (stderr, "Could not open incoming input %s\n", inputFilename);
      usage (argv[0], 42);
    }

  // Open the output TIFF input
  if ((outputFilename == NULL) ||
      (output = TIFFOpen (outputFilename, "w")) == NULL)
    {
      fprintf (stderr, "Could not open outgoing input %s\n", outputFilename);
      usage (argv[0], 42);
    }

  // Find the width and height of the input
  TIFFGetField (input, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField (input, TIFFTAG_IMAGELENGTH, &height);

  /////////////////////////////////////////////////////////////////////////////
  // Grab some memory
  if ((raster = (char *) malloc (sizeof (char) * width * height * 3)) == NULL)
    {
      fprintf (stderr, "Could not allocate enough memory for input raster\n");
      exit (42);
    }

  /////////////////////////////////////////////////////////////////////////////
  // Read the input into the memory buffer
  // todo: I couldn't use TIFFReadRGBAStrip here, because it gets confused
  stripSize = TIFFStripSize (input);
  roff = raster;
  for (stripNumber = 0; stripNumber < TIFFNumberOfStrips (input);
       stripNumber++)
    {
      roff += TIFFReadEncodedStrip (input, stripNumber, roff, stripSize);
    }

  /////////////////////////////////////////////////////////////////////////////
  // Write the image buffer to the file, we do this c times
  for(i = 0; i < count; i++){
    printf(".");
    fflush(stdout);

    // todo: We need to copy tags from the input image to the output image
    TIFFSetField (output, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField (output, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField (output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
    TIFFSetField (output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField (output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField (output, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField (output, TIFFTAG_SAMPLESPERPIXEL, 3);
    // todo: balance this off with having 8 kb per strip...
    TIFFSetField (output, TIFFTAG_ROWSPERSTRIP, 100000);
    

    if (TIFFWriteEncodedStrip (output, 0, raster,
			       width * height * 3 * sizeof (char)) == 0)
      {
	fprintf (stderr, "Could not write the output image\n");
	exit (42);
      }

    // Flush this subfile and move onto the next one
    if(TIFFWriteDirectory(output) == 0){
      fprintf(stderr, "Error writing subfile %d\n", i);
      exit(44);
    }
  }
  printf("\n");
    
  // Cleanup
  TIFFClose (input);
  TIFFClose (output);
  free (raster);
}

void
usage (char *cmd, int exitamt)
{
  fprintf (stderr, "Bad command line arguements...\n\n");
  fprintf (stderr, "Usage: %s -i <inputfile> -o <outputfile> -c <count>\n", 
	   cmd);
  exit (exitamt);
}
