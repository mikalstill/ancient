#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  FILE *image;
  unsigned long width, height;
  int bitdepth, colourtype;
  png_uint_32 i, j, rowbytes;
  png_structp png;
  png_infop info;
  png_bytepp row_pointers = NULL;
  unsigned char sig[8];
  char *raster;

  // Open the file
  if ((image = fopen (argv[1], "rb")) == NULL){
    fprintf(stderr, "Could not open the specified PNG file.\n");
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
  printf("Input image is %d by %d pixels\n", width, height);

  if (colourtype == PNG_COLOR_TYPE_PALETTE)
    png_set_expand (png);
  //  if(colourtype & PNG_COLOR_MASK_ALPHA)
  png_set_strip_alpha (png);
  png_read_update_info (png, info);

  rowbytes = png_get_rowbytes (png, info);
  printf("Using %d bytes per row\n", rowbytes);
  if((row_pointers = malloc (height * sizeof (png_bytep))) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  // Space for the bitmap
  if((raster = (unsigned char *) malloc ((rowbytes * height) + 1)) == NULL){
    fprintf(stderr, "Could not allocate memory\n");
    exit(42);
  }

  // Get the image bitmap
  for (i = 0; i < height; ++i)
    row_pointers[i] = raster + (i * rowbytes);
  png_read_image (png, row_pointers);
  png_read_end (png, NULL);
  fclose (image);


  // This cleans things up for us in the PNG library
  png_destroy_read_struct (&png, &info, NULL);

  /////////////////////////////////////////////////////////////////////////////
  // Now process the bitmap
  /////////////////////////////////////////////////////////////////////////////

  for(i = 0; i < width * height; i++){
    // Transparent is white
    if(((unsigned char) raster[i * 3] == 255) && 
       ((unsigned char) raster[i * 3 + 1] == 0) && 
       ((unsigned char) raster[i * 3 + 2] == 255)){
      raster[i * 3] = 255;
      raster[i * 3 + 1] = 255;
      raster[i * 3 + 2] = 255;
    }

    // "Black"
    else if(((unsigned char) raster[i * 3] < 100) && 
       ((unsigned char) raster[i * 3 + 1] < 100) && 
       ((unsigned char) raster[i * 3 + 2] < 100)){
      raster[i * 3] = 0;
      raster[i * 3 + 1] = 0;
      raster[i * 3 + 2] = 255;
    }
    
    // "White"
    else if(((unsigned char) raster[i * 3] > 150) && 
       ((unsigned char) raster[i * 3 + 1] > 150) && 
       ((unsigned char) raster[i * 3 + 2] > 150)){
      raster[i * 3] = 249;
      raster[i * 3 + 1] = 221;
      raster[i * 3 + 2] = 9;
    }

    // "Yellow"
    else if(((unsigned char) raster[i * 3] > 100) && 
       ((unsigned char) raster[i * 3 + 1] > 65) && 
       ((unsigned char) raster[i * 3 + 2] < 75)){
      raster[i * 3] = 255;
      raster[i * 3 + 1] = 0;
      raster[i * 3 + 2] = 0;
    }

    // Levels of gray that I have mapped to "Black"
    else if((abs((unsigned char) raster[i * 3] -
		 (unsigned char) raster[i * 3 + 1]) < 5) &&
	    (abs((unsigned char) raster[i * 3 + 1] - 
		 (unsigned char) raster[i * 3 + 2]) < 5)){
      raster[i * 3] = 0;
      raster[i * 3 + 1] = 0;
      raster[i * 3 + 2] = 255;
    }

    else{
      printf("Unknown color mapping %d,%d,%d\n", 
	     (unsigned char) raster[i * 3],
	     (unsigned char) raster[i * 3 + 1],
	     (unsigned char) raster[i * 3 + 2]);
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Now write the image out again
  /////////////////////////////////////////////////////////////////////////////
  if((image = fopen("output.png", "wb")) == NULL){
    fprintf(stderr, "Could not open the output image\n");
    exit(42);
  }
  
  // Get ready for writing
  if ((png =
       png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL,
				NULL)) == NULL){
    fprintf(stderr, "Could not create write structure for PNG (out of memory?)");
    exit(42);
  }
  
  // Get ready to specify important stuff about the image
  if ((info = png_create_info_struct (png)) == NULL){
    fprintf(stderr,
	    "Could not create PNG info structure for writing (out of memory?)");
    exit(42);
  }
  
  if (setjmp (png_jmpbuf (png))){
    fprintf(stderr, "Could not set the PNG jump value for writing");
    exit(42);
  }
  
  // This is needed before IO will work (unless you define callbacks)
  png_init_io(png, image);
  
  // Define important stuff about the image
  png_set_IHDR (png, info, width, height, bitdepth, PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
  printf("Writing an image of bitdepth %d\n", bitdepth);
  png_write_info (png, info);
  
  // Write the image out
  png_write_image (png, row_pointers);
  
  // Cleanup
  png_write_end (png, info);
  png_destroy_write_struct (&png, &info);
  fclose(image);
}
