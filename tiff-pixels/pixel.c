#include <tiffio.h>
#include <unistd.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
  uint32 count;
  TIFF *image;
  int optchar;
  uint32 width, height;
  uint32 *raster;
  char *inputName == NULL;

  while ((optchar = getopt (argc, argv, "i:")) != -1)
    {
      switch (optchar)
	{
	case '?':
	  printf ("Unknown command line option...\n");
	  printf ("Try: %s -i <inputimage> -o <outputimage>\n\n", argv[0]);
	  exit (0);
	  break;

	case 'i':
	  inputName = (char *) strdup(optarg);
	  break;
	}
    }

  // This is a gradient image
  if ((image = TIFFOpen (inputName, "r")) == NULL)
    {
      fprintf (stderr, "Could not open gradient file\n");
      exit (0);
    }

  // Find the width and height of the image
  TIFFGetField (image, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField (image, TIFFTAG_IMAGELENGTH, &height);
  printf ("Gradient file is %d by %d\n", width, height);

  if ((raster = (uint32 *) malloc (sizeof (uint32) * width * height)) == NULL)
    {
      fprintf (stderr, "Could not allocate enough memory\n");
      exit (42);
    }

  // Read the image into the memory buffer
  if (TIFFReadRGBAStrip (image, 0, raster) == 0)
    {
      fprintf (stderr, "Could not read image\n");
      exit (42);
    }


  TIFFClose (image);
}
