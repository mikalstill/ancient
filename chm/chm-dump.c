#include <stdio.h>
#include "fileutil.h"

// DWORD = int

int main(int argc, char *argv[]){
  FILE *input;
  int count, maxchunk, chunksize, waste, read, density;
  long long offset, length;
  
  if(argc < 2){
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(43);
  }

  if((input = fopen(argv[1], "r")) == NULL){
    fprintf(stderr, "Could not open input file %s for reading\n", argv[1]);
    exit(43);
  }

  /////////////////////////////////////////////////////////////////////////////
  // File header
  /////////////////////////////////////////////////////////////////////////////
  printf("File header\n");
  printf("Magic bytes: ");
  for(count = 0; count < 4; count++)
    printf("%c", fgetc(input));
  printf("\n");

  fileutil_displayinteger(input, "Version: "); printf("\n");
  fileutil_displayinteger(input, "Header length: "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be 1): "); printf("\n");
  fileutil_displayinteger(input, "Timestamp: "); printf("\n");
  fileutil_displaywindowslanguage(input, "Language: "); printf("\n");
  fileutil_displayguid(input, "GUID 1: "); printf("\n");
  fileutil_displayguid(input, "GUID 2: "); printf("\n");

  offset = fileutil_displaylong(input, "Section start offset: "); printf("\n");
  length = fileutil_displaylong(input, "Section length: "); printf("\n");

  // We need to be able to jump over some unwanted bytes here
  offset -= 4 + (4 * 4) + 4 + (16 * 2) + (8 * 2);
  printf("Skipping %d bytes: ", offset);
  for(count = 0; count < offset; count++){
    printf("%02x ", fgetc(input));
  }
  printf("\n");

  /////////////////////////////////////////////////////////////////////////////
  // Header section table
  /////////////////////////////////////////////////////////////////////////////
  printf("\nHeader section table\n");
  fileutil_displayinteger(input, "Unknown (should be 510): "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be 0): "); printf("\n");
  fileutil_displaylong(input, "File length: "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be 0): "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be 0): "); printf("\n");
  
  /////////////////////////////////////////////////////////////////////////////
  // Directory listing header
  /////////////////////////////////////////////////////////////////////////////
  printf("\nDirectory listing header\n");
  printf("Magic bytes: ");
  for(count = 0; count < 4; count++)
    printf("%c", fgetc(input));
  printf("\n");

  fileutil_displayinteger(input, "Version (should be 1): "); printf("\n");
  fileutil_displayinteger(input, "Length of header: "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be 0): "); printf("\n");
  chunksize = fileutil_displayinteger(input, "Chunk size: "); printf("\n");
  density = fileutil_displayinteger(input, "Density of quickref (usually 2): "); printf("\n");
  fileutil_displayinteger(input, "Depth of index tree (1 = none):"); printf("\n");
  fileutil_displayinteger(input, "Chunk number for root index chunk: "); printf("\n");
  fileutil_displayinteger(input, "Chunk number for root listing chunk: "); printf("\n");
  fileutil_displayinteger(input, "Final listing chunk: "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be -1): "); printf("\n");
  maxchunk = fileutil_displayinteger(input, "Number of directory chunks: "); printf("\n");
  fileutil_displaywindowslanguage(input, "Language: "); printf("\n");
  fileutil_displayguid(input, "GUID (5D02926A-212E-11D0-9DF9-00A0C922E6EC): "); printf("\n");
  fileutil_displayinteger(input, "Length (should be 84): "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be -1): "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be -1): "); printf("\n");
  fileutil_displayinteger(input, "Unknown (should be -1): "); printf("\n");

  /////////////////////////////////////////////////////////////////////////////
  // Chunks
  /////////////////////////////////////////////////////////////////////////////
  for(count = 0; count < maxchunk; count++){
    printf("\nChunk %d\n", count);
    printf("Magic bytes: ");
    for(count = 0; count < 4; count++)
      printf("%c", fgetc(input));
    printf("\n");
    
    waste = fileutil_displayinteger(input, "Wasted (or quickref) chunk space: "); printf("\n");
    fileutil_displayinteger(input, "Always zero: "); printf("\n");
    fileutil_displayinteger(input, "Previous chunk number: "); printf("\n");
    fileutil_displayinteger(input, "Next chunk number: "); printf("\n");
    offset = chunksize - 4 - (4 * 4);

    while(offset > 0){
      // Filename
      printf("\n  Remaining chunk length: %d\n", offset);
      length = fileutil_displaybyte(input, "  Name length: "); printf("\n");
      offset--;

      if(length == 0)
	break;

      printf("  Filename: ");
      for(count = 0; count < length; count++){
	printf("%c", fgetc(input));
	offset--;
      }
      printf("\n");
      
      fileutil_displayencinteger(input, "  Content section: ", &read); printf("\n");
      offset -= read;
      fileutil_displayencinteger(input, "  Offset: ", &read); printf("\n");
      offset -= read;
      fileutil_displayencinteger(input, "  Length: ", &read); printf("\n");
      offset -= read;
    }

    // Display the quickrefs
    for(count = 0; count < (1 + (1 << density)); count++){
      fileutil_displayinteger(input, "  Quick reference entry: "); printf("\n");
      offset -= 4;
    }

    offset -= 2;
    printf("\n  Skipping %d bytes: ", offset);
    for(count = 0; count < offset; count++){
      printf("%02x ", fgetc(input));
    }
    printf("\n");

    fileutil_displayshort(input, "  Number of entries in this chunk: "); printf("\n");
  }
}
