// This code wont compile, but it provides an example of how to use the libpng
// callbacks to get to image data in other places than files...
#include <png.h>
#include <pthread.h>

#define panda_true 1
#define panda_false 0

// This mutex keeps us thread safe (for the globals)
pthread_mutex_t convMutex = PTHREAD_MUTEX_INITIALIZER;

// Function prototypes
void libpngDummyWriteProc (png_structp png, png_bytep data, png_uint_32 len);
void libpngDummyFlushProc (png_structp png);

// We need thse globals because of the callbacks
char *globalImageBuffer;
unsigned long globalImageBufferOffset;
char globalIsIDAT;

///////////////////////////////////////////////////////////////////////////////
// Setup libpng
if ((png =
     png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
			      NULL)) == NULL){
  fprintf(stderr,
	  "Could not create write structure for PNG (out of memory?)");
  exit(42);
}

if ((info = png_create_info_struct (png)) == NULL){
  fprintf(stderr, 
	  "Could not create PNG info structure for writing (out of memory?)");
  exit(42);
}

if (setjmp (png_jmpbuf (png))){
  fprintf(stderr, "Could not set the PNG jump value for writing");
  exit(42);
}

// If this call is done before png_create_write_struct, then everything 
// seg faults...
pthread_mutex_lock (&amp;convMutex);

png_set_write_fn (png, NULL, (png_rw_ptr) libpngDummyWriteProc,
		  (png_flush_ptr) libpngDummyFlushProc);
globalIsIDAT = panda_false;
globalImageBuffer = NULL;
globalImageBufferOffset = 0;

png_set_IHDR (png, info, width, height, bitdepth, outColourType,
	      PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
	      PNG_FILTER_TYPE_DEFAULT);
png_write_info (png, info);

png_write_image (png, row_pointers);
png_write_end (png, info);
png_destroy_write_struct (&amp;png, &amp;info);

///////////////////////////////////////////////////////////////////////////////
// When we get back to here we have the image data

  pthread_mutex_unlock (&amp;convMutex);

///////////////////////////////////////////////////////////////////////////////
// Callbacks
void
libpngDummyWriteProc (png_structp png, png_bytep data, png_uint_32 len)
{
  char tempString[5];

  // Copy the first 4 bytes into a string
  tempString[0] = data[0];
  tempString[1] = data[1];
  tempString[2] = data[2];
  tempString[3] = data[3];
  tempString[4] = '\0';

  // If we know this is an IDAT, then copy the compressed image information
  if (globalIsIDAT == panda_true)
    {
      // Have we done anything yet?
      if (globalImageBuffer == NULL)
	globalImageBuffer = (char *) panda_xmalloc (len * sizeof (char));

      // Otherwise, we need to grow the memory buffer
      else
	{
	  if ((globalImageBuffer = (char *) realloc (globalImageBuffer,
						     (len * sizeof (char)) +
						     globalImageBufferOffset))
	      == NULL){
	    fprintf(stderr,
			 "Could not grow the png conversion memory buffer.");
	    exit(42);
	  }
	}

      // Now move the image data into the buffer
      memcpy (globalImageBuffer + globalImageBufferOffset, data, len);
      globalImageBufferOffset += len;
      
      globalIsIDAT = panda_false;
    }
  else if ((len == 4) &amp;&amp; (strcmp (tempString, "IDAT") == 0))
    globalIsIDAT = panda_true;
  else
    globalIsIDAT = panda_false;
}

void
libpngDummyFlushProc (png_structp png)
{
}
