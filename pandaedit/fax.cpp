#include "decompressor.h"
#include "fax.h"
#include "utility.h"
#include "verbosity.h"

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

unsigned long globalImageBufferOffset;
unsigned long globalImageBufferLength;

typedef union
{
  long l;
  short s;
} voro;

typedef struct
{
  short tag;
  short type;
  long count;
  voro valueOrOffset;
}
fifd;

typedef struct
{
  // Header (8 bytes)
  char endian[2];
  unsigned char version;
  int ifdOffset;

  // IFD (144 + 2 bytes)
  short ifdEntries;
  fifd entries[12]; // (12 bytes each)
}
ftiff;

typedef union interal_faketiff
{
  char d[154];
  ftiff t;
}
faketiff;
faketiff globalImageBuffer;

void
fax::reinit ()
{
}

char *
fax::decompress (char *input, unsigned long length, unsigned long &newlength)
{
  TIFF *stream;
  int i;

  // Open the image using the TIFF client routines defined below
  pthread_mutex_lock (&convMutex);
  globalImageBufferOffset = 0;
  globalImageBufferLength = sizeof(globalImageBuffer);
  memset(&globalImageBuffer, 0, sizeof(globalImageBuffer));

  ////////////////////
  // The TIFF header
  globalImageBuffer.t.endian[0] = 'I';
  globalImageBuffer.t.endian[1] = 'I';
  globalImageBuffer.t.version = 42;
  globalImageBuffer.t.ifdOffset = 8;

  ////////////////////
  // The TIFF IFD
  int entries = 0;
  
  // Image width (long, 1 immediate entry)
  globalImageBuffer.t.entries[entries].tag = 256;
  globalImageBuffer.t.entries[entries].type = 4;
  globalImageBuffer.t.entries[entries].count = 1;
  globalImageBuffer.t.entries[entries].valueOrOffset.l = m_width;
  entries++;

  // Image length (long, 1 immediate entry)
  globalImageBuffer.t.entries[entries].tag = 257;
  globalImageBuffer.t.entries[entries].type = 4;
  globalImageBuffer.t.entries[entries].count = 1;
  globalImageBuffer.t.entries[entries].valueOrOffset.l = m_length;
  entries++;

  // Compression (short, 1 immediate entry)
  globalImageBuffer.t.entries[entries].tag = 259;
  globalImageBuffer.t.entries[entries].type = 3;
  globalImageBuffer.t.entries[entries].count = 1;
  switch(m_k){
  case 0:
    globalImageBuffer.t.entries[entries].valueOrOffset.s = 2; // G2 fax
    break;

  case 1:
    globalImageBuffer.t.entries[entries].valueOrOffset.s = 3; // G4 fax
    break;

  default:
    debug(dlError, "Unknown fax compression level");
    globalImageBuffer.t.entries[entries].valueOrOffset.s = 0;
    break;
  }
  entries++;

  // Photometric interp (short, 1 immediate entry)
  globalImageBuffer.t.entries[entries].tag = 256;
  globalImageBuffer.t.entries[entries].type = 4;
  globalImageBuffer.t.entries[entries].count = 1;
  globalImageBuffer.t.entries[entries].valueOrOffset.s = m_width;
  entries++;

  globalImageBuffer.t.ifdEntries = entries;

  // Now fake libtiff out
  if ((stream =
       TIFFClientOpen ("dummy", "r", (thandle_t) - 1, libtiffDummyReadProc,
		       libtiffDummyWriteProc, libtiffDummySeekProc,
		       libtiffDummyCloseProc, libtiffDummySizeProc,
		       libtiffDummyMapProc, libtiffDummyUnmapProc)) == NULL)
    {
      newlength = 0;
      return NULL;
    }

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
  	string("TIFFIO Read fd = ") + toString((int) fd) + string(" amount = ") + toString(size) +
  	string(" offset = ") + toString((long) globalImageBufferOffset));

  tsize_t realsize;
  if(globalImageBufferOffset + size < globalImageBufferLength)
    realsize = size;
  else{
    realsize = globalImageBufferLength - globalImageBufferOffset;
    debug(dlTrace, string("Read truncated to ") + toString((long) realsize) + string(" bytes"));
  }

  memcpy (buf, globalImageBuffer.d + globalImageBufferOffset, realsize);
  globalImageBufferOffset += realsize;
  return realsize;
}

static tsize_t
libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // This always "fails"
  debug(dlTrace,
	string("TIFFIO Write fd = ") + toString((int) fd) + string(" amount = ") + toString(size) +
	string(" offset = ") + toString((long) globalImageBufferOffset));
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
	toString((long) globalImageBufferOffset));
  debug(dlTrace, "Emulate map fail");
  return 0;
}

static void
libtiffDummyUnmapProc (thandle_t fd, tdata_t buf, toff_t size)
{
  // This should never be called, as map failed
  debug(dlTrace,
	string("TIFFIO Unmap fd = ") + toString((int) fd) + string(" amount = ") + 
	toString((long) size) + string(" offset = ") + toString((long) globalImageBufferOffset));
}

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  // I don't really know what i is used for here...
  debug(dlTrace,
	string("TIFFIO Seek fd = ") + toString((int) fd) + string(" target = ") + 
	toString((long) off) + string(" i = ") + toString(i) + string(" offset = ") + 
	toString((long) globalImageBufferOffset));

  globalImageBufferOffset = off;

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
