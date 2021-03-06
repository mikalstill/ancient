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
  char *inputFilename = NULL, *outputFilename = NULL, 
    outputFilenameActual[200], *raster, *roff, optchar;
  int count;

  /////////////////////////////////////////////////////////////////////////////
  // Parse the command line options
  while ((optchar = getopt (argc, argv, "i:o:")) != -1)
    {
      switch (optchar)
	{
	case 'i':
	  inputFilename = (char *) strdup (optarg);
	  break;

	case 'o':
	  outputFilename = (char *) strdup (optarg);
	  break;

	default:
	  usage(argv[0], 0);
	  break;
	}
    }

  // Check the output parent name
  if(outputFilename == NULL){
    fprintf(stderr, \
	    "You need to specify a name for the series of output files\n");
    usage(argv[0], 42);
  }

  // Open the input TIFF image
  if ((inputFilename == NULL) ||
      (input = TIFFOpen (inputFilename, "r")) == NULL)
    {
      fprintf (stderr, "Could not open incoming input %s\n", inputFilename);
      usage (argv[0], 42);
    }

  /////////////////////////////////////////////////////////////////////////////
  // Grab a sub file from the input image and move it to a separate file. We do
  // this forever (until we break down below)...
  for(count = 0;; count++){
    // Find the width and height of the input
    TIFFGetField (input, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField (input, TIFFTAG_IMAGELENGTH, &height);

    ///////////////////////////////////////////////////////////////////////////
    // Grab some memory
    if ((raster = (char *) malloc (sizeof (char) * width * height * 3)) == 
	NULL)
      {
	fprintf (stderr, 
		 "Could not allocate enough memory for input raster\n");
	exit (42);
      }

    ///////////////////////////////////////////////////////////////////////////
    // Read the input into the memory buffer
    // todo: I couldn't use TIFFReadRGBAStrip here, because it gets confused
    stripSize = TIFFStripSize (input);
    roff = raster;
    for (stripNumber = 0; stripNumber < TIFFNumberOfStrips (input);
	 stripNumber++)
      {
	roff += TIFFReadEncodedStrip (input, stripNumber, roff, stripSize);
      }
    
    ///////////////////////////////////////////////////////////////////////////
    // Open the output TIFF
    snprintf(outputFilenameActual, 200, "%s-%d.tif", outputFilename, count);
    if ((output = TIFFOpen (outputFilenameActual, "w")) == NULL)
      {
	fprintf (stderr, "Could not open outgoing input %s\n", outputFilename);
	usage (argv[0], 42);
      }

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
    
    // Copy the subfile to a output location
    if (TIFFWriteEncodedStrip (output, 0, raster,
			       width * height * 3 * sizeof (char)) == 0)
      {
	fprintf (stderr, "Could not write the output image\n");
	exit (42);
      }

    ///////////////////////////////////////////////////////////////////////////
    // Flush this subfile and move onto the next one
    if(TIFFReadDirectory(input) == 0){
      printf(" No more subfiles");
      break;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Doing correct cleanup with a loop like this is important...
    free(raster);
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
