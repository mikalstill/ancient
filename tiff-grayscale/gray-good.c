#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>
#include <string.h>

void usage (char *, int);

int
main (int argc, char *argv[])
{
  TIFF *input, *output;
  uint32 width, height, offset, offset2;
  tsize_t stripSize, stripNumber;
  unsigned long x, y;
  char *inputFilename = NULL, *outputFilename = NULL, *raster, *roff,
    optchar;
  unsigned char *rout;

  /////////////////////////////////////////////////////////////////////////////
  // Parse the command line options
  while ((optchar = getopt (argc, argv, "i:o:x:y:w:l:")) != -1)
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
	  usage (argv[0], 0);
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

  // Open the output TIFF
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

  if ((rout = (unsigned char *) malloc (sizeof (char) * width * height)) 
      == NULL)
    {
      fprintf (stderr,
	       "Could not allocate enough memory for enlarged raster\n");
      exit (42);
    }

  // todo: We need to copy tags from the input image to the output image
  TIFFSetField (output, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField (output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField (output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField (output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField (output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
  TIFFSetField (output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField (output, TIFFTAG_SAMPLESPERPIXEL, 1);
  // todo: balance this off with having 8 kb per strip...
  TIFFSetField (output, TIFFTAG_ROWSPERSTRIP, 100000);

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
  // Convert to grayscale
  offset2 = 0;
  for(offset = 0; offset < width * height * 3; offset += 3){
    rout[offset2++] = ((unsigned char) (raster[offset]) * 0.299 + 
		       (unsigned char) (raster[offset + 1]) * 0.587 + 
		       (unsigned char) (raster[offset + 2]) * 0.114);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Write the image buffer to the file
  if (TIFFWriteEncodedStrip (output, 0, rout,
			     width * height * sizeof (char)) == 0)
    {
      fprintf (stderr, "Could not write the output image\n");
      exit (42);
    }

  // Cleanup
  TIFFClose (input);
  TIFFClose (output);
  free (raster);
  free (rout);
}

void
usage (char *cmd, int exitamt)
{
  fprintf (stderr, "Bad command line arguements...\n\n");
  fprintf (stderr, "Usage: %s -i <inputfile> -o <outputfile>\n", cmd);
  exit (exitamt);
}
