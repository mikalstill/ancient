/* An example of how to read a jpeg file using libjpeg. This example converts
   the file to a TIFF file to prove that we can read the image data
   successfully */

#include <stdio.h>
#include <jpeglib.h>
#include <tiffio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

char *file;
off_t size;

typedef struct
{
  struct jpeg_source_mgr pub;   /* public fields */

  JOCTET * buffer;              /* start of buffer */
  boolean start_of_file;        /* have we gotten any data yet? */
} *src_ptr;

static boolean
fill_input_buffer(cinfo)
    j_decompress_ptr cinfo;
{
  src_ptr src = (src_ptr) cinfo->src;

  if (size <= 0) {
    /* Insert a fake EOI marker */
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    size = 2;
  }
  else
    {
      src->buffer = file;
    }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = size;

  return TRUE;
}

static void
skip_input_data(cinfo, num_bytes)
    j_decompress_ptr cinfo;
    long num_bytes;
{
  src_ptr src = (src_ptr) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

static void
dummy_source(cinfo)
    j_decompress_ptr cinfo;
{
  /* no work necessary here */
}

int main(int argc, char *argv[])
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  char *img, *upto;
  JSAMPARRAY buffer;
  src_ptr src;

  TIFF *output;
  
  int fd;
  struct stat sb;

  /* Map the input file into memory */
  if ((fd = open ("input.jpg", O_RDONLY)) < 0)
    {
      perror("Could not open file");
      exit (43);
    }

  if(fstat(fd, &sb) < 0){
    perror("Could not stat file");
    exit(43);
  }
  size = sb.st_size;

  if ((file =
       (char *) mmap (NULL, size, PROT_READ, MAP_SHARED, fd, 0)) == -1)
    {
      perror("Could not mmap file");
      exit (43);
    }

  /* Setup the JPEG memory reading stuff */
  src = (src_ptr)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				sizeof(src_ptr));
  cinfo->src = (struct jpeg_source_mgr *) src;

  src->pub.init_source = dummy_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = dummy_source;

  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */

  /* Do JPEG things */
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, infile);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  
  printf("Input image is %d x %d at depth %d\n",
	 cinfo.output_width,
	 cinfo.output_height,
	 cinfo.output_components);
  if((img = (char *) malloc(cinfo.output_width * cinfo.output_height * 
			    cinfo.output_components)) == NULL)
    {
      perror("Couldn't allocate memory");
      exit(1);
    }
  buffer = (*cinfo.mem->alloc_sarray)
    ((j_common_ptr) &cinfo, JPOOL_IMAGE, 
     cinfo.output_width * cinfo.output_components, 1);

  upto = img;
  while (cinfo.output_scanline < cinfo.output_height)
    {
      jpeg_read_scanlines(&cinfo, buffer, 1);
      memcpy(upto, *buffer, cinfo.output_width * cinfo.output_components);
      upto += cinfo.output_width * cinfo.output_components;
    }

  /* Write that out to a TIFF file */
  if((output = TIFFOpen("output.tif", "w")) == NULL){
    fprintf(stderr, "Could not open output image\n");
    exit(42);
  }

  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, cinfo.output_width);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, cinfo.output_height);
  TIFFSetField(output, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, cinfo.output_components);

  /* Actually write the image */
  if(TIFFWriteEncodedStrip(output, 0, img, 
			   cinfo.output_width * cinfo.output_height * 
			   cinfo.output_components) == 0){
    fprintf(stderr, "Could not write image\n");
    exit(42);
  }

  /* Cleanup */
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);

  TIFFClose(output);

  /* Now clean up */
  if(munmap(file, sb.st_size) < 0){
    perror("Could not unmap memory");
    exit(43);
  }

  close(fd);
}
