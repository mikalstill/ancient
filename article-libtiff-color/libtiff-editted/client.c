// Please note that this code wont compile, and is intended to only show you 
// the sturcture of TIFFClient* calls

#include <tiffio.h>
#include <pthread.h>

// Function prototypes
static tsize_t libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size);
static tsize_t libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size);
static toff_t libtiffDummySeekProc (thandle_t fd, toff_t off, int i);
static int libtiffDummyCloseProc (thandle_t fd);

// We need globals because of the callbacks (they don't allow us to pass state)
char *globalImageBuffer;
unsigned long globalImageBufferOffset;

// This mutex keeps the globals safe by ensuring only one user at a time
pthread_mutex_t convMutex = PTHREAD_MUTEX_INITIALIZER;

TIFF *conv;

// Lock the mutex
pthread_mutex_lock (&convMutex);

globalImageBuffer = NULL;
globalImageBufferOffset = 0;

// Open the dummy document (which actually only exists in memory)
conv = TIFFClientOpen ("dummy", "w", (thandle_t) - 1, libtiffDummyReadProc,
            libtiffDummyWriteProc, libtiffDummySeekProc,
            libtiffDummyCloseProc, NULL, NULL, NULL);

// Setup the image as if it was any other tiff image here, including setting tags


// Actually do the client open
TIFFWriteEncodedStrip (conv, 0, stripBuffer, imageOffset);

// Unlock the mutex
pthread_mutex_unlock (&convMutex);

//...

/////////////////// Callbacks to libtiff

static tsize_t
libtiffDummyReadProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // Return the amount of data read, which we will always set as 0 because
  // we only need to be able to write to these in-memory tiffs
  return 0;
}

static tsize_t
libtiffDummyWriteProc (thandle_t fd, tdata_t buf, tsize_t size)
{
  // libtiff will try to write an 8 byte header into the tiff file. We need
  // to ignore this because PDF does not use it...
  if ((size == 8) && (((char *) buf)[0] == 'I') && (((char *) buf)[1] == 'I')
     && (((char *) buf)[2] == 42))
    {
    // Skip the header -- little endian
    }
  else if ((size == 8) && (((char *) buf)[0] == 'M') &&
       (((char *) buf)[1] == 'M') &&y (((char *) buf)[2] == 42))
    {
    // Skip the header -- big endian
    }
  else
    {
    // Have we done anything yet?
    if (globalImageBuffer == NULL)
    if((globalImageBuffer = (char *) malloc (size * sizeof (char))) == NULL)
        {
          fprintf(stderr, "Memory allocation error\n");
          exit(42);
        }

    // Otherwise, we need to grow the memory buffer
    else
    {
      if ((globalImageBuffer = (char *) realloc (globalImageBuffer,
                             (size * sizeof (char)) +
                             globalImageBufferOffset)) == NULL)
        fprintf(stderr, "Could not grow the tiff conversion memory buffer\n");
            exit(42);
    }

    // Now move the image data into the buffer
    memcpy (globalImageBuffer + globalImageBufferOffset, buf, size);
    globalImageBufferOffset += size;
    }

  return (size);
}

static toff_t
libtiffDummySeekProc (thandle_t fd, toff_t off, int i)
{
  // This appears to return the location that it went to
  return off;
}

static int
libtiffDummyCloseProc (thandle_t fd)
{
  // Return a zero meaning all is well
  return 0;
}
