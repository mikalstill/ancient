/* sunraster.c:
 *
 * sun rasterfile image type
 *
 * jim frost 09.27.89
 *
 * Copyright 1989, 1991 Jim Frost.
 * See included file "copyright.h" for complete copyright information.
 */

#include "copyright.h"
#include "image.h"
#include "sunraster.h"

/* SUPPRESS 558 */
/* SUPPRESS 560 */

static void babble(name, header)
     char           *name;
     struct rheader *header;
{
  printf("%s is a", name);
  switch (memToVal(header->type, 4)) {
  case ROLD:
    printf("n old-style");
    break;
  case RSTANDARD:
    printf(" standard");
    break;
  case RRLENCODED:
    printf(" run-length encoded");
    break;
  case RRGB:
    printf(" RGB"); /* RGB format instead of BGR */
    break;
  case RTIFF:
    printf(" TIFF");
    break;
  case RIFF:
    printf(" RIFF");
    break;
  default:
    printf(" unknown-type");
  }
  printf(" %dx%d", memToVal(header->width, 4), memToVal(header->height, 4));

  switch (memToVal(header->depth, 4)) {
  case 1:
    printf(" monochrome");
    break;
  case 8:
    printf(" 8 plane %s",
	   memToVal(header->maplen, 4) > 0 ? "color" : "greyscale");
    break;
  case 24:
    printf(" 24 plane color");
    break;

  case 32:
    /* isn't it nice how the sunraster.h file doesn't bother to mention that
     * 32-bit depths are allowed?
     */
    printf(" 32 plane color");
    break;
  }
  printf(" Sun rasterfile\n");
}

int sunRasterIdent(fullname, name)
     char *fullname, *name;
{ ZFILE          *zf;
  struct rheader  header;
  int             r;

  if (! (zf= zopen(fullname))) {
    perror("sunRasterIdent");
    return(0);
  }
  switch (zread(zf, (byte *)&header, sizeof(struct rheader))) {
  case -1:
    perror("sunRasterIdent");
    r= 0;
    break;

  case sizeof(struct rheader):
    if (memToVal(header.magic, 4) != RMAGICNUMBER) {
      r= 0;
      break;
    }
    babble(name, &header);
    r= 1;
    break;

  default:
    r= 0;
    break;
  }
  zclose(zf);
  return(r);
}

/* read either rl-encoded or normal image data
 */

static void sunread(zf, buf, len, enc)
     ZFILE        *zf;
     byte         *buf;
     unsigned int  len;
     unsigned int  enc;  /* true if encoded file */
{ static byte repchar, remaining= 0;

  /* rl-encoded read
   */

  if (enc) {
    while (len--)
      if (remaining) {
	remaining--;
	*(buf++)= repchar;
      }
      else {
	if (zread(zf, &repchar, 1) != 1) {
	  printf("sunRasterLoad: Bad read on image data\n");
	  exit(1);
	}
	if (repchar == RESC) {
	  if (zread(zf, &remaining, 1) != 1) {
	    printf("sunRasterLoad: Bad read on image data\n");
	    exit(1);
	  }
	  if (remaining == 0)
	    *(buf++)= RESC;
	  else {
	    if (zread(zf, &repchar, 1) != 1) {
	      printf("sunRasterLoad: Bad read on image data\n");
	      exit(1);
	    }
	    *(buf++)= repchar;
	  }
	}
	else
	  *(buf++)= repchar;
      }
  }

  /* normal read
   */

  else {
    if (zread(zf, buf, len) < len) {
      printf("sunRasterLoad: Bad read on image data\n");
      exit(1);
    }
  }
}

Image *sunRasterLoad(fullname, name, verbose)
     char         *fullname, *name;
     unsigned int  verbose;
{ ZFILE          *zf;
  struct rheader  header;
  unsigned int    mapsize;
  byte           *map;
  byte           *mapred, *mapgreen, *mapblue;
  unsigned int    depth;
  unsigned int    linelen;   /* length of raster line in bytes */
  unsigned int    fill;      /* # of fill bytes per raster line */
  unsigned int    enc;
  unsigned int    rgb_format;
  byte            fillchar;
  Image          *image;
  byte           *lineptr;
  unsigned int    x, y;

  if (! (zf= zopen(fullname))) {
    perror("sunRasterLoad");
    return(NULL);
  }
  switch (zread(zf, (byte *)&header, sizeof(struct rheader))) {
  case -1:
    perror("sunRasterLoad");
    zclose(zf);
    exit(1);

  case sizeof(struct rheader):
    if (memToVal(header.magic, 4) != RMAGICNUMBER) {
      zclose(zf);
      return(NULL);
    }
    if (verbose)
      babble(name, &header);
    break;

  default:
    zclose(zf);
    return(NULL);
  }

  znocache(zf); /* turn off caching; we don't need it anymore */

  /* get an image to put the data in
   */

  depth= memToVal(header.depth, 4);
  switch(depth) {
  case 1:
    image= newBitImage(memToVal(header.width, 4),
		       memToVal(header.height, 4));
    break;
  case 8:
    image= newRGBImage(memToVal(header.width, 4),
		       memToVal(header.height, 4),
		       memToVal(header.depth, 4));
    break;
  case 24:
  case 32:
    image= newTrueImage(memToVal(header.width, 4),
		       memToVal(header.height, 4));
    break;
  default:
    printf("sunRasterLoad: Bad depth %d (only 1, 8, 24 are valid)\n", depth);
    exit(1);
  }

  /* figure out which format it is
   */

  enc= 0;
  rgb_format= 0;
  switch (memToVal(header.type, 4)) {
  case ROLD:
  case RSTANDARD:
    break;
  case RRLENCODED:
    enc= 1;
    break;
  case RRGB:
    rgb_format= 1;
    break;
  case RTIFF: /* sorry, don't even know what these are */
  case RIFF:
  default:
    fprintf(stderr, "%s: Unsupported Sun Rasterfile image type (sorry)\n");
    return(NULL);
  }

  /* set up the colormap
   */

  if (depth == 1)
    linelen= (image->width / 8) + (image->width % 8 ? 1 : 0);
  else
    linelen= image->width * image->pixlen;
  fill= (linelen % 2 ? 1 : 0);
  /*
   *  Handle color...
   */
  if (mapsize= memToVal(header.maplen, 4)) {
    map= lmalloc(mapsize);
    if (zread(zf, map, mapsize) < mapsize) {
      printf("sunRasterLoad: Bad read on colormap\n");
      exit(1);
    }
    mapsize /= 3;
    mapred= map;
    mapgreen= mapred + mapsize;
    mapblue= mapgreen + mapsize;
    if (image->rgb.size == 0)
	newRGBMapData(&image->rgb, mapsize);
    else if (mapsize > image->rgb.size)
	mapsize= image->rgb.size;
    for (y= 0; y < mapsize; y++) {
      *(image->rgb.red + y)= (*(mapred++) << 8);
      *(image->rgb.green + y)= (*(mapgreen++) << 8);
      *(image->rgb.blue + y)= (*(mapblue++) << 8);
    }
    lfree(map);
    image->rgb.used= mapsize;
  }

  /*
   *  Handle 8-bit greyscale via a simple ramp function...
   */
  else if (depth == 8) {
    mapsize = 256*3;
    map= lmalloc(mapsize);
    for (y = 0; y < 256; y += 1) {
      map[y] = map[256+y] = map[2*256+y] = y;
    }
    mapsize /= 3;
    mapred= map;
    mapgreen= mapred + mapsize;
    mapblue= mapgreen + mapsize;
    if (image->rgb.size == 0)
	newRGBMapData(&image->rgb, mapsize);
    for (y= 0; y < mapsize; y++) {
      *(image->rgb.red + y)= (*(mapred++) << 8);
      *(image->rgb.green + y)= (*(mapgreen++) << 8);
      *(image->rgb.blue + y)= (*(mapblue++) << 8);
    }
    lfree(map);
    image->rgb.used= mapsize;
  }
  /* 24-bit and 32-bit handle themselves.  currently we don't support
   * a colormap for them.
   */

  lineptr= image->data;

  /* if it's a 32-bit image, we read the line and then strip off the
   * top byte of each pixel to get truecolor format
   */

  if (depth >= 24) {
    byte *buf, *bp;

    buf= lmalloc(image->width * (depth == 24 ? 3 : 4));
    for (y= 0; y < image->height; y++) {
      sunread(zf, buf, image->width * (depth == 24 ? 3 : 4), enc);
      bp= buf;
      if (depth == 24) {
	if (rgb_format)
	  for (x= 0; x < image->width; x++) {
	    *(lineptr++)= *(bp++); /* red */
	    *(lineptr++)= *(bp++); /* green */
	    *(lineptr++)= *(bp++); /* blue */
	  }
	else
	  for (x= 0; x < image->width; x++) {
	    *(lineptr++)= *(bp + 2); /* red */
	    *(lineptr++)= *(bp + 1); /* green */
	    *(lineptr++)= *bp;       /* blue */
	    bp += 3;
	  }
      }
      else {
	if (rgb_format)
	  for (x= 0; x < image->width; x++) {
	    bp++;                  /* skip byte */
	    *(lineptr++)= *(bp++); /* red */
	    *(lineptr++)= *(bp++); /* green */
	    *(lineptr++)= *(bp++); /* blue */
	  }
	else
	  for (x= 0; x < image->width; x++) {
	    *(lineptr++)= *(bp + 3); /* red */
	    *(lineptr++)= *(bp + 2); /* green */
	    *(lineptr++)= *(bp + 1); /* blue */
	    bp += 4;
	  }
      }
      if (fill)
	sunread(zf, &fillchar, fill, enc);
    }
    lfree(buf);
  }
  else {
    for (y= 0; y < image->height; y++) {
      sunread(zf, lineptr, linelen, enc);
      lineptr += linelen;
      if (fill)
	sunread(zf, &fillchar, fill, enc);
    }
  }
  zclose(zf);
  image->title= dupString(name);
  return(image);
}
