// Demonstrates how to use the compress() and uncompress() calls in zlib
//
// Copyright (c) Michael Still 2003, released under the terms of the GNU GPL

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

int main(int argc, char *argv[]){
  char *uncompressed, *compressed, *extracted;
  unsigned long cLen, eLen;
  int retval;

  uncompressed = (char *) strdup("This is the is the is the is the input string");
  if((compressed = (char *) malloc(1024)) == NULL){
    printf("Could not allocate memory for compression\n");
    exit(42);
  }
  printf("Uncompressed length = %d\n", strlen(uncompressed) + 1);
  printf("Uncompressed data: %s\n", uncompressed);

  // There is an alternate version of this call where you can set the compression
  // level called compress2
  retval = compress(compressed, &cLen, uncompressed, strlen(uncompressed) + 1);
  switch(retval){
  case Z_MEM_ERROR:
    printf("Compression memory error\n");
    exit(42);
    
  case Z_BUF_ERROR:
    printf("Output buffer too small\n");
    exit(42);
  }
  printf("Compressed length = %d\n", cLen);
  printf("Compressed data: %s\n", compressed);
  
  // Now uncompress it. The documentation says that we really should know the
  // uncompressed length first, but let's try to guess...
  eLen = cLen;
  extracted = NULL;
  retval = 4;
  while((eLen < 100000) && (retval != Z_OK)){
    printf("Trying eLen of %d\n", eLen);
    if(extracted != NULL)
      free(extracted);
    if((extracted = (char *) malloc(eLen)) == NULL){
      printf("Could not allocate memory for decompression\n");
      exit(42);
    }

    retval = uncompress(extracted, &eLen, compressed, cLen);
    switch(retval){
    case Z_MEM_ERROR:
      printf("Compression memory error\n");
      break;
      
    case Z_BUF_ERROR:
      printf("Output buffer too small\n");
      break;
    }

    if(retval != Z_OK)
      eLen *= 2;
  }
  printf("Extracted length = %d\n", eLen);
  printf("Extracted data: %s\n", extracted);
}
