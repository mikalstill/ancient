#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  FILE *image;
  unsigned long width, height;
  int bitdepth, colourtype;
  png_uint_32 i, j;
  png_structp png;
  png_infop info;
  unsigned char sig[8];

  // Open the file
  if ((image = fopen (argv[1], "rb")) == NULL){
    fprintf(stderr, "Could not open the specified PNG file.");
    exit(0);
  }

  // Check that it really is a PNG file
  fread(sig, 1, 8, image);
  if(!png_check_sig(sig, 8)){
    printf("This file is not a valid PNG file\n");
    fclose(image);
    return;
  }

  // Start decompressing
  if((png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
				   NULL, NULL)) == NULL){
    fprintf(stderr, "Could not create a PNG read structure (out of memory?)");
    exit(0);
  }

  if((info = png_create_info_struct(png)) == NULL){
    fprintf(stderr, "Could not create PNG info structure (out of memory?)");
    exit(0);
  }

  // If pnginfo_error did not exit, we would have to call 
  // png_destroy_read_struct
  if(setjmp(png_jmpbuf(png))){
    fprintf(stderr, "Could not set PNG jump value");
    exit(0);
  }

  // Get ready for IO and tell the API we have already read the image signature
  png_init_io(png, image);
  png_set_sig_bytes(png, 8);
  png_read_info(png, info);
  png_get_IHDR(png, info, &width, &height, &bitdepth, &colourtype, NULL, 
	       NULL, NULL);

  // This cleans things up for us in the PNG library
  fclose(image);
  png_destroy_read_struct(&png, &info, NULL);
}
