// Sample callback functions -- note that multiple varargian arguments
// are presented in reverse order...

#include <stdarg.h>
#include <stdio.h>
#include <zlib.h>
#include "samples.h"

pandalex_sample_dictint_list *dictint_list;

void pandalex_sample_specversion(int event, va_list argptr){
  printf("Spec version is: %s\n", (char *) va_arg(argptr, char *));
}

void pandalex_sample_entireheader(int event, va_list argptr){
  int    i;
  char   *textMatch = (char *) va_arg(argptr, char *);

  printf("Entire document header is: ");

  for(i = 0; i < strlen(textMatch); i++){
    if(isprint(textMatch[i])) printf("%c ", textMatch[i]);
    else printf("\\%d ", textMatch[i]);
  }

  printf("\n");
}

void pandalex_sample_objstart(int event, va_list argptr){
  printf("Object started - generation %d of number %d\n",
	 va_arg(argptr, int), va_arg(argptr, int));
}

void pandalex_sample_dictitem_string(int event, va_list argptr){
  printf("Dictionary string\n");
}

void pandalex_sample_dictitem_name(int event, va_list argptr){
  printf("Dictionary name\n");
}

void pandalex_sample_dictitem_array(int event, va_list argptr){
  printf("Dictionary array\n");
}

void pandalex_sample_dictitem_object(int event, va_list argptr){
  printf("Dictionary object\n");
}

void pandalex_sample_dictitem_dict(int event, va_list argptr){
  printf("Dictionary dict\n");
}

void pandalex_sample_dictitem_int(int event, va_list argptr){
  printf("Dictionary int\n");
}

void pandalex_sample_stream(int event, va_list argptr){
  char *filter;
  int length;
  char *lengthObj;
  char *streamData;
  int streamDataLen;
  pandalex_sample_dictint_list  *now;
  int found;

  filter = va_arg(argptr, char *);
  length = (int) va_arg(argptr, char *);
  lengthObj = va_arg(argptr, char *);
  streamData = va_arg(argptr, char *);
  streamDataLen = va_arg(argptr, int);

  printf("Do something with the stream object\n");

  // Determine if we know the length
  switch(length){
  case -2:
    // We don't - have we seen the obj already?
    now = dictint_list;
    found = 0;

    while((now->next != NULL) && (found == 0)){
      if(strcmp(lengthObj, now->value) == 0){
	// Yes -- do something
	pandalex_sample_procstream(filter, now->number, streamData, streamDataLen);
	found = 1;
      }

      now = now->next;
    }

    // No -- save info and wait
    if(found == 0){
      // now is already the end of the list
      if((now->next = (pandalex_sample_dictint_list *)
	  malloc(sizeof(pandalex_sample_dictint_list))) == NULL){
	fprintf(stderr, "Could not add to list of waiting streams\n");
	exit(42);
      }

      printf("Do something about waiting for the info\n");

      now->value = (char *) strmcpy(lengthObj, -1);
      now->filter = (char *) strmcpy(filter, -1);

      now->stream = (char *) strmcpy(streamData + 2, streamDataLen - 2);
      now->stream[strlen(now->stream)] = '\0';

      now->streamlen = streamDataLen - 2;

      now->waiting = 1;
      now = now->next;
      now->next = NULL;
    }
    break;

  case -1:
    // It was never defined
    fprintf(stderr, "Stream with undefined length met\n");
    exit(43);
    break;

  default:
    if(length > 0){
      // We do -- do something
      pandalex_sample_procstream(filter, length, streamData, streamDataLen);
    }
    else{
      fprintf(stderr, "Stream with length having undefined meaning %d\n",
	      length);
      exit(44);
    }
  }
}

void pandalex_sample_dictint(int event, va_list argptr){
  int found;
  int objnum, objgen, value;
  char *objref;
  pandalex_sample_dictint_list *now;

  // Get the passed information
  objnum = va_arg(argptr, int);
  objgen = va_arg(argptr, int);
  value = va_arg(argptr, int);

  printf("Do something with the dictint %d %d R = %d\n", objnum, objgen, value);

  // Information is handed to this event in a slightly different manner to
  // the stream event handler. Fix this.
  if((objref = (char *) malloc((intlen(objnum) + intlen(objgen) + 5) * sizeof(char))) == NULL){
    fprintf(stderr, "Could not allocate enough space for objref\n");
    exit(42);
  }
  
  sprintf(objref, "%d %d R", objnum, objgen);

  // Are we already waiting?
  now = dictint_list;
  found = 0;

  while((now->next != NULL) && (found == 0)){
    if(strcmp(objref, now->value) == 0){
      // Yes -- do something
      pandalex_sample_procstream(now->filter, value, now->stream, now->streamlen);
      found = 1;
    }

    now = now->next;
  }

  // No -- save data and wait
  if(found == 0){
    // now is already the end of the list
    if((now->next = (pandalex_sample_dictint_list *)
	malloc(sizeof(pandalex_sample_dictint_list))) == NULL){
      fprintf(stderr, "Could not add to list of waiting streams\n");
      exit(42);
    }
    
    now->value = (char *) strmcpy(objref, -1);
    now->filter = NULL;
    now->stream = NULL;
    now->waiting = 2;
    now = now->next;
    now->next = NULL;
  }
}

void pandalex_sample_procstream(char *filter, int length, char *data, int dataLen){
  char *uncompressed, *dataPtr;
  uLong srcLen, dstLen = 4096;
  int result, i;

  // Check length
  if(length < 1){
    fprintf(stderr, "Stream length is not believable\n");
    return;
  }

  // Check there is a filter at all
  if(filter == NULL){
    fprintf(stderr, "This stream is not compressed!\n");
    return;
  }

  // If the stream starts with a \r or a \n or a \r\n, then these should be stripped off
  dataPtr = data;
  while((dataPtr[0] == '\r') || (dataPtr[0] == '\n')) dataPtr++;

  // Do something with the stream
  if(strcmp(filter, "FlateDecode") == 0){
    printf("Do something involving Flate\n");
    
    //    printf("--------------------------------------------------");
    //    for(i = 0; i < dataLen; i++)
    //      printf("%c", data[i]);
    //    printf("--------------------------------------------------");

    if((uncompressed = (char *) malloc(sizeof(char) * dstLen)) == NULL){
      fprintf(stderr, "Could not make enough space to decompress Flate stream\n");
      exit(42);
    }

    srcLen = dataLen - 1;

    // We grow the output buffer until we no longer get buffer size errors
    while((result = uncompress(uncompressed, &dstLen, dataPtr, srcLen)) == Z_BUF_ERROR){
      printf("Expand the output buffer from %d to %d\n", dstLen, dstLen * 2);
      
      dstLen *= 2;
      if((uncompressed = (char *) realloc(uncompressed, dstLen)) == NULL){
	// We could not grow the buffer, so we exit
	fprintf(stderr, "Could not allocate enough space for decompression\n");
	exit(42);
      }
    }

    if(result != Z_OK){
      fprintf(stderr, "Flate decompression failed because of ");

      switch(result){
      case Z_MEM_ERROR:
	fprintf(stderr, "not enough memory\n");
	break;

      case Z_DATA_ERROR:
	fprintf(stderr, "corrupt input data\n");
	break;
      }

      debuglex(data, srcLen, "Flate compression failure", 0);
      exit(46);
    }
    
    printf("----------- UNCOMPRESSED STREAM IS -------------------------------------------\n");
    printf("%s\n", uncompressed);
    printf("------------------------------------------------------------------------------\n");
  }
  else if(strcmp(filter, "LZWDecode") == 0){
    printf("LZW compression is encumbered by Patents and therefore not supported\n");
  }
  else if(strcmp(filter, "CCITTFaxDecode") == 0){
    printf("Do something involving CCITTFax compression (TIFF)\n");
  }
}


