/* An example of how to read a jpeg file using libjpeg. This example converts
   the file to a TIFF file to prove that we can read the image data
   successfully */

#include <stdio.h>
#include <jpeglib.h>
#include <tiffio.h>

int main(int argc, char *argv[])
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * infile;
  char *img, *upto;
  JSAMPARRAY buffer;

  TIFF *output;

  /* Open a JPEG file and grab the contents */
  if ((infile = fopen("input.jpg", "rb")) == NULL) {
    fprintf(stderr, "Cannot open input image\n");
    exit(1);
  }

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

  // Actually write the image
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
}
