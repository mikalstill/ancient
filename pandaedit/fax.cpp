#include "decompressor.h"
#include "fax.h"

// todo_mikal: check for libtiff

#include <tiffio.h>
#include <jpeglib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
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

typedef struct
{
  // Header (8 bytes)
  char endian[2];
  unsigned char version;
  int ifdOffset;

  // IFD
  short ifdEntries;
}
ftiff;

typedef union interal_faketiff
{
  char d[8];
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
  newlength = 0;
  return NULL;

  TIFF *stream;
  int i;

  // Open the image using the TIFF client routines defined below
  pthread_mutex_lock (&convMutex);
  globalImageBufferOffset = 0;

  // The TIFF header
  globalImageBuffer.t.endian[0] = 'I';
  globalImageBuffer.t.endian[1] = 'I';
  globalImageBuffer.t.version = 42;
  globalImageBuffer.t.ifdOffset = 8;

  // The TIFF IFD
  globalImageBuffer.t.ifdEntries = 12;

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







/*****************************************************************************
  The following are dummy functions for libtiff that will allow us to do on-
  the-fly conversion of tiff images to the small subset of tiff that the
  PDF specification allows for...
*****************************************************************************/

static tsize_t
libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  printf
    ("DEBUG: TIFFIO Read fd = %d, buf = 0x%08x, amount = %d, data = 0x%08x, offset = %d\n",
     fd, buf, size, globalImageBuffer.d, globalImageBufferOffset);
  memcpy (buf, globalImageBuffer.d + globalImageBufferOffset, size);
  globalImageBufferOffset += size;
  printf ("DEBUG: TIFFIO Read finished\n");
  return size;
}

static tsize_t
libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // This always "fails"
  printf ("DEBUG: TIFFIO Write fd = %d, amount = %d\n", fd, size);
  return 0;
}

static toff_t
libtiffDummySizeProc (thandle_t fd)
{
  printf ("DEBUG: TIFFIO Size fd = %d\n", fd);
  return 0;
}

static int
libtiffDummyMapProc (thandle_t fd, tdata_t * buf, toff_t * size)
{
  // This always "fails"
  printf ("DEBUG: TIFFIO Map fd = %d, amount = %d\n", fd, size);
  return 0;
}

static void
libtiffDummyUnmapProc (thandle_t fd, tdata_t buf, toff_t size)
{
  // This should never be called, as map failed
  printf ("DEBUG: TIFFIO Unmap fd = %d, amount = %d\n", fd, size);
}

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  printf ("DEBUG: TIFFIO Seek fd = %d, offset = %d, i = %d\n", fd, off, i);
  globalImageBufferOffset = i;

  // This appears to return the location that it went to
  return off;
}

static int
libtiffDummyCloseProc (thandle_t fd)
{
  printf ("DEBUG: TIFFIO Close fd = %d\n", fd);

  // Return a zero meaning all is well
  return 0;
}
