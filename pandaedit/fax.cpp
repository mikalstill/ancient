#include "decompressor.h"
#include "fax.h"
#include "utility.h"
#include "verbosity.h"
#include "faketiff.h"

// todo_mikal: check for libtiff

#include <tiffio.h>
#include <jpeglib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>

pthread_mutex_t convMutex = PTHREAD_MUTEX_INITIALIZER;

static tsize_t libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size);
static tsize_t libtiffDummyWriteProc (thandle_t fd, tdata_t buf,
				      tsize_t size);
static toff_t libtiffDummySizeProc (thandle_t fd);
static int libtiffDummyMapProc (thandle_t fd, tdata_t * buf, toff_t * offset);
static void libtiffDummyUnmapProc (thandle_t fd, tdata_t buf, toff_t offset);
static toff_t libtiffDummySeekProc (thandle_t fd, toff_t off, int i);
static int libtiffDummyCloseProc (thandle_t fd);

unsigned long globalImageOffset;
unsigned long globalImageLength;
char *globalImage;
char *globalImageBuffer;
fakeTiff *globalTiff;

void
fax::reinit ()
{
}

char *
fax::decompress (char *input, unsigned long length, unsigned long &newlength)
{
  TIFF *stream;
  int i;
  short entries = 0;
  fakeTiffRemoteRational *xres, *yres;
  char *retval;
  uint16 compTest;
  TIFF *image;

  debug(dlTrace, "Decode a TIFF stream");
  debug(dlTrace, string("TIFF header length: ") + toString((int) sizeof(fakeTiff)));
  debug(dlTrace, string("TIFF compressed input data is ") + toString((long) length) + string(" bytes"));
  debug(dlTrace, string("A short is ") + toString((int) sizeof(short)));
  debug(dlTrace, string("A long is ") + toString((int) sizeof(long)));

  // Open the image using the TIFF client routines defined below
  pthread_mutex_lock (&convMutex);
  globalImageOffset = 0;

  // Setup the fake TIFF
  if(globalImage != NULL)
    free(globalImage);
  if((globalImage = (char *) malloc(length + sizeof(fakeTiff) + 
				    (sizeof(fakeTiffRemoteRational) * 2 ))) == NULL){
    debug(dlError, "Fake TIFF memory allocation error");
    goto error;
  }
  memset(globalImage, 0, sizeof(globalImage));

  ////////////////////////////////////////////////////
  // Setup the header for the fake tiff
  globalTiff = (fakeTiff *) globalImage;

  // The TIFF header
  globalTiff->endian[0] = 'I';
  globalTiff->endian[1] = 'I';
  globalTiff->version = 42;
  globalTiff->ifdOffset = 10;
  globalTiff->pad = 0;

  ////////////////////
  // The TIFF IFD -- these must be in increasing numerical order

  // Image width (long, 1 immediate entry)
  globalTiff->entries[entries].tag = 256;
  globalTiff->entries[entries].type = 4;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = m_width;
  entries++;

  // Image length (long, 1 immediate entry)
  globalTiff->entries[entries].tag = 257;
  globalTiff->entries[entries].type = 4;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = m_length;
  entries++;

  // Bits per sample (short, 1 immediate entry)
  globalTiff->entries[entries].tag = 258;
  globalTiff->entries[entries].type = 3;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.s = 1;
  entries++;

  // Compression (short, 1 immediate entry)
  globalTiff->entries[entries].tag = 259;
  globalTiff->entries[entries].type = 3;
  globalTiff->entries[entries].count = 1;
  debug(dlTrace, string("Fax compression level is: ") + toString(m_k));
  switch(m_k){
  case 0:
    globalTiff->entries[entries].valueOrOffset.s = 3; // G3 fax
    break;

  case -1:
    globalTiff->entries[entries].valueOrOffset.s = 4; // G4 fax
    break;

  default:
    debug(dlError, string("Unknown fax compression level: ") + toString(m_k));
    globalTiff->entries[entries].valueOrOffset.s = 0;
    goto error;
  }
  entries++;

  // Photometric interp (short, 1 immediate entry)
  globalTiff->entries[entries].tag = 262;
  globalTiff->entries[entries].type = 3;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.s = 0;
  entries++;

  // Strip Offsets (long, 1 immediate entry)
  globalTiff->entries[entries].tag = 273;
  globalTiff->entries[entries].type = 4;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = sizeof(fakeTiff) + (sizeof(fakeTiffRemoteRational) * 2);
  entries++;

  // Samples per pixel (short, 1 immediate entry)
  globalTiff->entries[entries].tag = 277;
  globalTiff->entries[entries].type = 3;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.s = 1;
  entries++;

  // Rows per strip (long, 1 immediate entry)
  globalTiff->entries[entries].tag = 278;
  globalTiff->entries[entries].type = 4;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = m_length;
  entries++;

  // Strip byte counts (long, 1 immediate entry)
  globalTiff->entries[entries].tag = 279;
  globalTiff->entries[entries].type = 4;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = m_width * m_length / 8;
  if(m_width * m_length % 8 != 0)
    globalTiff->entries[entries].valueOrOffset.l++;
  entries++;

  // X resolution (rational, 1 remote entry)
  globalTiff->entries[entries].tag = 282;
  globalTiff->entries[entries].type = 5;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = sizeof(fakeTiff);
  debug(dlTrace, string("X resolution offset: ") + toString(globalTiff->entries[entries].valueOrOffset.l));
  entries++;

  // Y resolution (rational, 1 remote entry)
  globalTiff->entries[entries].tag = 283;
  globalTiff->entries[entries].type = 5;
  globalTiff->entries[entries].count = 1;
  globalTiff->entries[entries].valueOrOffset.l = sizeof(fakeTiff) + sizeof(fakeTiffRemoteRational);
  debug(dlTrace, string("Y resolution offset: ") + toString(globalTiff->entries[entries].valueOrOffset.l));
  entries++;

  // And now write out the number of entries
  debug(dlTrace, string("The fake TIFF IFD contains ") + toString(entries) + 
	string(" entries"));
  globalTiff->ifdEntries = entries;

  // And now the two remote rationals
  xres = (fakeTiffRemoteRational *) globalImage + sizeof(fakeTiff);
  xres->n = 300;
  xres->d = 1;

  yres = (fakeTiffRemoteRational *) globalImage + sizeof(fakeTiff) + sizeof(fakeTiffRemoteRational);
  yres->n = 300;
  yres->d = 1;

  //////////////////////////////////
  // Now remember the length, and copy the image data
  globalImageLength = sizeof(fakeTiff) + sizeof(fakeTiffRemoteRational) * 2 + length;
  globalImageBuffer = globalImage + sizeof(fakeTiff) + (sizeof(fakeTiffRemoteRational) * 2);
  memcpy(globalImageBuffer, input, length);

  // Now fake libtiff out
  if ((stream =
       TIFFClientOpen ("dummy", "r", (thandle_t) - 1, libtiffDummyReadProc,
		       libtiffDummyWriteProc, libtiffDummySeekProc,
		       libtiffDummyCloseProc, libtiffDummySizeProc,
		       libtiffDummyMapProc, libtiffDummyUnmapProc)) == NULL)
    {
      debug(dlError, "Could not open fake TIFF image");
      goto error;
    }

  // We should determine the compression which we faked (just to be sure)
  TIFFGetField(stream, TIFFTAG_COMPRESSION, &compTest);
  debug(dlTrace, string("Fake TIFF uses compression scheme: ") + toString((int) compTest));

  // Well, after all that, we can now get the decompressed image stream
  newlength = TIFFStripSize(stream);
  debug(dlTrace, string("The TIFF width is ") + toString(m_width));
  debug(dlTrace, string("The TIFF height is ") + toString(m_length));
  debug(dlTrace, string("The TIFF strip size is ") + toString((long) newlength));
  if((retval = (char *) malloc(newlength)) == NULL){
    debug(dlError, "Failed to allocate enough memory for decompressed TIFF");
    goto error;
  }

  if((newlength = TIFFReadEncodedStrip(stream, 0, retval, newlength)) < 0){
    debug(dlError, "Failled to read encoded image data from fake TIFF");
    goto error;
  }
  debug(dlTrace, string("New length of stream is ") + toString((long) newlength));

  ///////////////////////////////////////
  // Temporarily write it out to a file as well so that I can visually inspect it...
  // Open the TIFF file
  if((image = TIFFOpen("output.tif", "w")) == NULL){
    printf("Could not open output.tif for writing\n");
    exit(42);
  }

  // We need to set some values for basic tags before we can add any data
  TIFFSetField(image, TIFFTAG_IMAGEWIDTH, m_width);
  TIFFSetField(image, TIFFTAG_IMAGELENGTH, m_length);
  TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 1);
  TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, m_length);

  TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_CCITTFAX4);
  TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISWHITE);
  TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
  TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

  TIFFSetField(image, TIFFTAG_XRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_YRESOLUTION, 300.0);
  TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);

  TIFFWriteEncodedStrip(image, 0, retval, newlength);
  TIFFClose(image);

  // Cleanup
  free(globalImage);
  globalImage = NULL;
  pthread_mutex_unlock(&convMutex);
  return retval;

 error:
  free(globalImage);
  globalImage = NULL;
  pthread_mutex_unlock (&convMutex);
  newlength = 0;
  return NULL;
}

void
fax::setWidth(long width){
  m_width = width;
}

void
fax::setLength(long length){
  m_length = length;
}

void
fax::setK(int k){
  m_k = k;
}




  





/*****************************************************************************
  The following are dummy functions for libtiff that will allow us to do on-
  the-fly conversion of tiff images to the small subset of tiff that the
  PDF specification allows for...
*****************************************************************************/

static tsize_t
libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  debug(dlTrace,
  	string("TIFFIO Read fd = ") + toString((int) fd) + string(" amount = ") + 
	toString(size) + string(" offset = ") + toString((long) globalImageOffset));

  tsize_t realsize;
  if(globalImageOffset + size < globalImageLength)
    realsize = size;
  else{
    realsize = globalImageLength - globalImageOffset;
    debug(dlTrace, string("Read truncated to ") + toString((long) realsize) + string(" bytes"));
  }

  memcpy (buf, globalImage + globalImageOffset, realsize);
  globalImageOffset += realsize;

  debug(dlTrace, string("Image data read: ") + binaryToString(buf, realsize));
  debug(dlTrace, string("Read ") + toString(realsize) + string(" bytes"));
  debug(dlTrace, string("Offset = ") + toString((long) globalImageOffset));
  return size;
}

static tsize_t
libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // This always "fails"
  debug(dlTrace,
	string("TIFFIO Write fd = ") + toString((int) fd) + string(" amount = ") + toString(size) +
	string(" offset = ") + toString((long) globalImageOffset));
  debug(dlTrace, "Emulate write fail");
  return 0;
}

static toff_t
libtiffDummySizeProc (thandle_t fd)
{
  debug(dlTrace,
	string("TIFFIO Size fd = ") + toString((int) fd));

  return 0;
}

static int
libtiffDummyMapProc (thandle_t fd, tdata_t * buf, toff_t * size)
{
  // This always "fails"
  debug(dlTrace,
	string("TIFFIO Map fd = ") + toString((int) fd) + string(" amount = ") + 
	toString((long) *size) + string(" offset = ") + 
	toString((long) globalImageOffset));
  debug(dlTrace, "Emulate map fail");
  return 0;
}

static void
libtiffDummyUnmapProc (thandle_t fd, tdata_t buf, toff_t size)
{
  // This should never be called, as map failed
  debug(dlTrace,
	string("TIFFIO Unmap fd = ") + toString((int) fd) + string(" amount = ") + 
	toString((long) size) + string(" offset = ") + toString((long) globalImageOffset));
}

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  // I don't really know what i is used for here...
  debug(dlTrace,
	string("TIFFIO Seek fd = ") + toString((int) fd) + string(" target = ") + 
	toString((long) off) + string(" i = ") + toString(i) + string(" offset = ") + 
	toString((long) globalImageOffset));

  globalImageOffset = off;

  // This appears to return the location that it went to
  return off;
}

static int
libtiffDummyCloseProc (thandle_t fd)
{
  debug(dlTrace,
	string("TIFFIO Close fd = ") + toString((int) fd));

  // Return a zero meaning all is well
  return 0;
}
