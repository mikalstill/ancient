#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>
#include <string.h>

void usage (char *, int);

int
main (int argc, char *argv[])
{
  TIFF *input, *output;
  uint32 width, height, offset, offset2, xs = 0, ys = 0, xe = -1, ye = -1;
  tsize_t stripSize, stripNumber;
  unsigned long x, y;
  char *inputFilename = NULL, *outputFilename = NULL, *raster, *roff,
    *enlarged, *rout, optchar;
  int xrep, yrep;
  float m;

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

	}
    }

  // Make sure we have reasonable defaults
  if (xe == -1)
    xe = xs + 10;

  if (ye == -1)
    ye = ys + 10;

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

  // Sanity check some of our arguements
  if (xe > width)
    {
      fprintf (stderr,
	       "You choice of starting x position, or width, results in the enlargement falling off the edge of the input image\n");
      usage (argv[0], 43);
    }

  if (ye > height)
    {
      fprintf (stderr,
	       "You choice of starting y position, or length, results in the enlargement falling off the end of the input image\n");
      usage (argv[0], 43);
    }

  printf ("Enlarging a %d by %d portion of the image\n", xe - xs, ye - ys);

  /////////////////////////////////////////////////////////////////////////////
  // Grab some memory
  if ((raster = (char *) malloc (sizeof (char) * width * height * 3)) == NULL)
    {
      fprintf (stderr, "Could not allocate enough memory for input raster\n");
      exit (42);
    }

  // todo: crap assumption about the data being 8 bps, 3 spp
  if ((enlarged = (char *) malloc (sizeof (char) * (xe - xs) * (ye - ys) * 3 *
				   121)) == NULL)
    {
      fprintf (stderr,
	       "Could not allocate enough memory for enlarged raster\n");
      exit (42);
    }

  if ((rout = (char *) malloc (sizeof (char) * (width + 200) * height * 3))
      == NULL)
    {
      fprintf (stderr,
	       "Could not allocate enough memory for output raster\n");
      exit (42);
    }

  // todo: We need to copy tags from the input image to the output image
  TIFFSetField (output, TIFFTAG_IMAGEWIDTH, width + 200);
  TIFFSetField (output, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField (output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField (output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField (output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField (output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField (output, TIFFTAG_SAMPLESPERPIXEL, 3);
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
  // Actually do the enlargement of the portion of the image specified
  offset = offset2 = 0;
  for (y = ys; y < ye; y++)
    {
      for (yrep = 0; yrep < 10; yrep++)
	{
	  for (x = xs; x < xe; x++)
	    {
	      offset = (x + (y * width)) * 3;
	      for (xrep = 0; xrep < 10; xrep++)
		{
		  memcpy (enlarged + offset2, raster + offset, 3);
		  offset2 += 3;
		}

	      // The white border to the left of the pixel
	      memset (enlarged + offset2, 255, 3);
	      offset2 += 3;
	    }
	}

      // The white line at the bottom of these pixels
      memset (enlarged + offset2, 255, (ye - ys) * 3 * 11);
      offset2 += (ye - ys) * 3 * 11;
    }

  /////////////////////////////////////////////////////////////////////////////
  // Now we assemble the two parts of the image together into a big output
  // raster
  memset (rout, 255, sizeof (char) * (width + 200) * height * 3);

  // The original image
  offset = 0;
  offset2 = 0;
  for (y = 0; y < height; y++)
    {
      memcpy (rout + offset2, raster + offset, width * 3);
      offset += width * 3;
      offset2 += (width + 200) * 3;
    }

  // Box the bit that was enlarged in the original image, can't use memset here
  // Top line
  offset = (((width + 200) * (ys - 1)) + xs - 1) * 3;
  for (x = 0; x < xe - xs + 2; x++)
    {
      rout[offset++] = 255;
      rout[offset++] = 0;
      rout[offset++] = 0;
    }

  // Bottom line
  offset = (((width + 200) * ye) + xs - 1) * 3;
  for (x = 0; x < xe - xs + 2; x++)
    {
      rout[offset++] = 255;
      rout[offset++] = 0;
      rout[offset++] = 0;
    }

  // Vertical lines
  offset = (((width + 200) * ys) + xs - 1) * 3;
  for (y = 0; y < ye - ys + 1; y++)
    {
      rout[offset] = 255;
      rout[offset + 1] = 0;
      rout[offset + 2] = 0;

      rout[offset + ((ye - ys + 1) * 3)] = 255;
      rout[offset + ((ye - ys + 1) * 3) + 1] = 0;
      rout[offset + ((ye - ys + 1) * 3) + 2] = 0;

      offset += (width + 200) * 3;
    }

  /////////////////////////////////////////////////////////////////////////////
  // Box the enlarged portion of the image

  /////////////////////////////////////////////////////////////////////////////
  // Draw the two diagonal lines between the original and the enlarged
  //    this bit is based on the premis that y = mx + b
  //    and m = (y2 - y1) / (x2 - x1) and that geometry hasn't significantly
  //    changed since my high school days
  //
  //    if we assume that the first point is the origin, then the maths is
  //    even easier
  // 
  //    which I think is probably a fairly safe set of assumptions at this
  //    stage...
  // todo
  //  printf("ye = %d, xe = %d\n", ye, xe);
  //  m = (ye - ys) / (xe - xs);
  //  for(x = 0; x < (xe - xs); x++){
  //    y = m * x;
  //    
  //    printf("%d, %d (%f = %f)\n", x, y, m, ye / xe);
  //  }

  /////////////////////////////////////////////////////////////////////////////
  // Copy the enlarged portion across

  offset = 0;
  // todo: I'm too tired to understand the placement of this in maths land
  offset2 = (((((height / 2) - ((ye - ys) * 11 / 2)) *
	       (width + 200)) + width + 50) * 3);
  for (y = 0; y < (ye - ys) * 11; y++)
    {
      memcpy (rout + offset2, enlarged + offset, (ye - ys) * 11 * 3);
      offset += (ye - ys) * 11 * 3;
      offset2 += (width + 200) * 3;
    }

  /////////////////////////////////////////////////////////////////////////////
  // Write the image buffer to the file
  if (TIFFWriteEncodedStrip (output, 0, rout,
			     (width + 200) * height * 3 * sizeof (char)) == 0)
    {
      fprintf (stderr, "Could not write the output image\n");
      exit (42);
    }

  // Cleanup
  TIFFClose (input);
  TIFFClose (output);
  free (raster);
  free (enlarged);
}

void
usage (char *cmd, int exitamt)
{
  fprintf (stderr, "Bad command line arguements...\n\n");
  fprintf (stderr, "Usage: %s -i <inputfile> -o <outputfile>\n", cmd);
  exit (exitamt);
}
