#include <stdio.h>
#include "fileutil.h"

// DWORD = int

int main(int argc, char *argv[]){
  FILE *input;
  
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
  printf("Magic bytes: %c%c%c%c\n", fgetc(input), fgetc(input), fgetc(input),
	 fgetc(input));
  fileutil_displayinteger(input, "Version: ");
  fileutil_displayinteger(input, "Header length: ");
  fileutil_displayinteger(input, "Unknown (should be 1): ");
  fileutil_displayinteger(input, "Timestamp: ");
  fileutil_displaywindowslanguage(input, "Language: ");
  fileutil_displayguid(input, "GUID 1: ");
  fileutil_displayguid(input, "GUID 2: ");
}
