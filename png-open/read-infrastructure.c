#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <unistd.h>

int main(int argc, char *argv[]){
  FILE *image;
  unsigned long imageBufSize, width, height, runlen;
  unsigned char signature;
  int bitdepth, colourtype;
  png_uint_32 i, j, rowbytes;
  png_structp png;
  png_infop info;
  unsigned char sig[8];
  png_bytepp row_pointers = NULL;
  char *bitmap;

  // Open the file
  if ((image = fopen (argv[1], "rb")) == NULL){
    fprintf(stderr, "Could not open the specified PNG file.");
    exit(0);
  }

  // Check that it really is a PNG file
  fread(sig, 1, 8, image);
  if(!png_check_sig(sig, 8)){
    printf("  This file is not a valid PNG file\n");
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

  ///////////////////////////////////////////////////////////////////////////
  // Start displaying information
  //
  // Can't fight the moonlight...
  ///////////////////////////////////////////////////////////////////////////
  
  printf("  Image Width: %d Image Length: %d\n", width, height);
  printf("  Bits/Sample: %d\n", bitdepth);
  printf("  Samples/Pixel: %d\n", info->channels);
  printf("  Pixel Depth: %d\n", info->pixel_depth);  // Does this add value?

  // Photometric interp packs a lot of information
  printf("  Colour Type (Photometric Interpretation): ");

  switch(colourtype){
  case PNG_COLOR_TYPE_GRAY:
    printf("GRAYSCALE ");
    break;

  case PNG_COLOR_TYPE_PALETTE:
    printf("PALETTED COLOUR ");
    if(info->num_trans > 0) printf("with alpha ");
    printf("(%d colours, %d transparent) ", 
	   info->num_palette, info->num_trans);
    break;

  case PNG_COLOR_TYPE_RGB:
    printf("RGB ");
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA:
    printf("RGB with alpha channel ");
    break;

  case PNG_COLOR_TYPE_GRAY_ALPHA:
    printf("GRAYSCALE with alpha channel ");
    break;

  default:
    printf("Unknown photometric interpretation!");
    break;
  }
  printf("\n");

  printf("  Image filter: ");
  switch(info->filter_type){
  case PNG_FILTER_TYPE_BASE:
    printf("Single row per byte filter ");
    break;

  case PNG_INTRAPIXEL_DIFFERENCING:
    printf("Intrapixel differencing (MNG only) ");
    break;

  default:
    printf("Unknown filter! ");
    break;
  }
  printf("\n");

  printf("  Interlacing: ");
  switch(info->interlace_type){
  case PNG_INTERLACE_NONE:
    printf("No interlacing ");
    break;

  case PNG_INTERLACE_ADAM7:
    printf("Adam7 interlacing ");
    break;

  default:
    printf("Unknown interlacing ");
    break;
  }
  printf("\n");

  printf("  Compression Scheme: ");
  switch(info->compression_type){
  case PNG_COMPRESSION_TYPE_BASE:
    printf("Deflate method 8, 32k window");
    break;

  default:
    printf("Unknown compression scheme!");
    break;
  }
  printf("\n");

  printf("  Resolution: %d, %d ", 
	 info->x_pixels_per_unit, info->y_pixels_per_unit);
  switch(info->phys_unit_type){
  case PNG_RESOLUTION_UNKNOWN:
    printf("(unit unknown)");
    break;

  case PNG_RESOLUTION_METER:
    printf("(pixels per meter)");
    break;

  default:
    printf("(Unknown value for unit stored)");
    break;
  }
  printf("\n");

  // FillOrder is always msb-to-lsb, big endian
  printf("  FillOrder: msb-to-lsb\n  Byte Order: Network (Big Endian)\n");

  // Text comments
  printf("  Number of text strings: %d of %d\n", 
	 info->num_text, info->max_text);

  for(i = 0; i < info->num_text; i++){
    printf("    %s ", info->text[i].key);

    switch(info->text[1].compression){
    case -1:
      printf("(tEXt uncompressed)");
      break;

    case 0:
      printf("(xTXt deflate compressed)");
      break;

    case 1:
      printf("(iTXt uncompressed)");
      break;

    case 2:
      printf("(iTXt deflate compressed)");
      break;

    default:
      printf("(unknown compression)");
      break;
    }

    printf(": ");
    j = 0;
    while(info->text[i].text[j] != '\0'){
      if(info->text[i].text[j] == '\n') printf("\\n");
      else fputc(info->text[i].text[j], stdout);

      j++;
    }

    printf("\n");
  }

  // This cleans things up for us in the PNG library
  fclose(image);
  png_destroy_read_struct(&png, &info, NULL);
}
