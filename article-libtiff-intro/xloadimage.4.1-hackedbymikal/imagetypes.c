/* imagetypes.c:
 *
 * this contains things which reference the global ImageTypes array
 *
 * jim frost 09.27.89
 *
 * Copyright 1989, 1991 Jim Frost.
 * See included file "copyright.h" for complete copyright information.
 */

#include "copyright.h"
#include "image.h"
#include "imagetypes.h"
#include "options.h"
#include <errno.h>

/* SUPPRESS 560 */

extern int errno;

/* load a named image
 */

Image *loadImage(globalopts, options, name, verbose)
     OptionSet    *globalopts;
     OptionSet    *options;
     char         *name;
     unsigned int  verbose;
{ char    fullname[BUFSIZ];
  Option *opt;
  Image  *image;
  int     a;
  unsigned long mikal_count;

  if (findImage(name, fullname) < 0) {
    if (errno == ENOENT)
      fprintf(stderr, "%s: image not found\n", name);
    else
      perror(fullname);
    return(NULL);
  }

  printf("Mikal: Starting to call loadImage()\n");

  /* see if there's a "type" option and use it if there is
   */
  if (! (opt= getOption(globalopts, TYPE)))
    opt= getOption(options, TYPE);
  if (opt) {
    for (a= 0; ImageTypes[a].loader; a++)
      if (!strncmp(ImageTypes[a].type, opt->info.type, strlen(opt->info.type))) {
	if (image= ImageTypes[a].loader(fullname, name, verbose)) {
	  zreset(NULL);

	  /* this converts a 1-bit RGB image to a bitmap prior to blitting
	   * it across.  it's a transmission efficiency hack, and has the
	   * beneficial side-effect of eliminating dithering on RGB images
	   * that don't really need it.
	   */
	  if (RGBP(image) && (image->rgb.used <= 2)) {
	      Image *new_image;
	      new_image = flatten(image);
	      if (new_image != image) {
		  freeImage(image);
		  image = new_image;
	      }
	  }

	  printf("Mikal: Returning image\n");
	  return(image);
	}
	fprintf(stderr, "%s does not look like a \"%s\" image (skipping).\n",
		fullname, opt->info.type); 
	printf("Mikal: Returning NULL\n");
	zreset(NULL);
	return(NULL);
      }
    fprintf(stderr, "\"%s\" is not a supported image type (will try to guess the type)\n", opt->info.type);

    /* fall through into the type determination loop
     */
  }

  printf("Mikal: Step two\n");

  /* try to pick out the image type
   */
  for (a= 0; ImageTypes[a].loader; a++) {
    debug(("Checking %s against loader for %s\n", fullname, ImageTypes[a].name));
    if (image= ImageTypes[a].loader(fullname, name, verbose)) {
      zreset(NULL);

      printf("Mikal: Step two complete RGBP(image) = %d, image->rgb.used = %d\n", RGBP(image), image->rgb.used);

      /* this does the 1-bit conversion as above.
       */
      //      if (RGBP(image) && (image->rgb.used <= 2)) {
      if(1 == 1){
	Image *new_image;

		printf("Mikal: One bit conversion\n");  

	  new_image = flatten(image);
	  if (new_image != image) {
	      freeImage(image);
	      image = new_image;
	  }
	}

      printf("Mikal: Dumping the image data out (width = %d):\n",
             image->width);

      for(mikal_count = 0; mikal_count < (image->height * image->width) / 8; 
          mikal_count++){
        printf("%02x ", image->data[mikal_count]);
        if((mikal_count + 1) % (image->width / 8) == 0) printf("\n");
      }

      printf("Mikal: Step two returning\n");
      return(image);
    }
  }
  
  printf("Mikal: Step three\n");

  fprintf(stderr, "%s: unknown or unsupported image type\n", fullname);
  zreset(NULL);
  return(NULL);
}

/* identify what kind of image a named image is
 */

void identifyImage(name)
     char *name;
{ char fullname[BUFSIZ];
  int  a;

  if (findImage(name, fullname) < 0) {
    if (errno == ENOENT)
      fprintf(stderr, "%s: image not found\n", name);
    else
      perror(fullname);
    return;
  }
  for (a= 0; ImageTypes[a].identifier; a++) {
    if (ImageTypes[a].identifier(fullname, name)) {
      zreset(NULL);
      return;
    }
  }
  zreset(NULL);
  fprintf(stderr, "%s: unknown or unsupported image type\n", fullname);
}

/* dump an image into an image file of the specified type
 */
void dumpImage(image, type, filename, verbose)
     Image *image;
     char *type;
     char *filename;
     int verbose;
{ int a;
  char typename[32];
  char *optptr;

  /* find options if there are any
   */
  optptr = index(type, ',');
  if (optptr) {
    strncpy(typename, type, optptr - type);
    typename[optptr - type] = '\0';
    optptr++; /* skip comma */
  }
  else
    strcpy(typename, type);

  for (a= 0; ImageTypes[a].loader; a++)
    if (!strncmp(ImageTypes[a].type, typename, strlen(typename))) {
      if (ImageTypes[a].dumper)
	ImageTypes[a].dumper(image, optptr, filename, verbose);
      else
	fprintf(stderr, "\
%s: I can read this image type but cannot write it (sorry).\n\
If you need a list of supported image types, use the -supported option.\n",
		type);
      return;
    }
  fprintf(stderr, "\
%s: unknown or unsupported image type.  If you need a list\n\
of supported image types, use the -supported option.\n", type);
}

/* tell user what image types we support
 */
void supportedImageTypes()
{ int a;

  printf("Type Name  Can Dump Description\n");
  printf("---------- -------- -----------\n");
  for (a= 0; ImageTypes[a].name; a++)
    printf("%-10s %-8s %s\n", ImageTypes[a].type, 
	   (ImageTypes[a].dumper ? "Yes" : "No"),
	   ImageTypes[a].name);
}
