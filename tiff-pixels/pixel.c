#include <tiffio.h>

int
main (int argc, char *argv[])
{
  uint32 count;
  TIFF *image;

  // The command line packs information about the data file to use and the x and y coordinates
  while ((optchar = getopt (argc, argv, "d:x:y:g:vm:")) != -1)
    {
      switch (optchar)
	{
	case '?':
	  printf ("Unknown command line option...\n");
	  printf ("Try: %s -d <datafile> -x <width> -y <height>\n\n",
		  argv[0]);
	  exit (0);
	  break;

	case 'd':
	  dataFile = optarg;
	  break;

	case 'm':
	  dataFile2 = optarg;
	  break;

	case 'x':
	  boardx = atoi (optarg);
	  break;

	case 'y':
	  boardy = atoi (optarg);
	  break;

	case 'g':
	  gradFile = optarg;
	  break;

	case 'v':
	  verbose = 1;
	  break;
	}
    }

  // This is a gradient image
  if ((image = TIFFOpen (gradFile, "r")) == NULL)
    {
      fprintf (stderr, "Could not open gradient file\n");
      exit (0);
    }

  // Find the width and height of the image
  TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &gWidth);
  TIFFGetField (image, TIFFTAG_IMAGELENGTH, &gHeight);
  printf ("Gradient file is %d by %d\n", gWidth, gHeight);

  if ((gradRaster = (uint32 *) malloc (sizeof (uint32) * gWidth * gHeight)) == NULL)
    {
      fprintf (stderr, "Could not allocate enough memory\n");
      exit (42);
    }

  // Read the image into the memory buffer
  if (TIFFReadRGBAStrip (image, 0, gradRaster) == 0)
    {
      fprintf (stderr, "Could not read image\n");
      exit (42);
    }

  // Populate the gradient thingie
  for (count = 0; count < gHeight; count++)
    {
      gradient[256 - count] = gradRaster[gWidth * count];
    }
  TIFFClose (image);

}
