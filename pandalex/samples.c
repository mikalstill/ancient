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
  pandalex_sample_dictint_list  *now;
  int found;

  filter = va_arg(argptr, char *);
  length = (int) va_arg(argptr, char *);
  lengthObj = va_arg(argptr, char *);
  streamData = va_arg(argptr, char *);

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
	pandalex_sample_procstream(filter, now->number, streamData);
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

      now->stream = (char *) strmcpy(streamData + 2, -1);
      now->stream[strlen(now->stream) - 1] = '\0';

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
      pandalex_sample_procstream(filter, length, streamData);
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
      pandalex_sample_procstream(now->filter, value, now->stream);
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

void pandalex_sample_procstream(char *filter, int length, char *data){
  char *uncompressed;
  uLong srcLen, dstLen;
  int result;

  printf("Do something with the stream filter = %s, length = %d\n", filter, length);

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

  // Do something with the stream
  if(strcmp(filter, "FlateDecode") == 0){
    printf("Do something involving Flate\n");
    
    if((uncompressed = (char *) malloc(sizeof(char) * (length) + 1)) == NULL){
      fprintf(stderr, "Could not make enough space to decompress Flate stream\n");
      exit(42);
    }

    srcLen = strlen(data) - 1;
    dstLen = length;

    if((result = uncompress(uncompressed, &dstLen, data, srcLen)) != Z_OK){
      fprintf(stderr, "Flate decompression failed because of ");

      switch(result){
      case Z_MEM_ERROR:
	fprintf(stderr, "not enough memory\n");
	break;

      case Z_BUF_ERROR:
	fprintf(stderr, "not enough space if destination buffer\n");
	break;

      case Z_DATA_ERROR:
	fprintf(stderr, "corrupt input data\n");

	debuglex(data, length, "Flate compression failure", 0);
        fprintf(stderr, "\n\nsrcLen is %d\n", srcLen);
	break;
      }

      exit(46);
    }

    printf("----------- UNCOMPRESSED STREAM IS -------------------------------------------");
    
  }
  else if(strcmp(filter, "LZWDecode") == 0){
    fprintf(stderr, "LZW compression is encumbered by Patents and therefore not supported\n");
  }
  else if(strcmp(filter, "CCITTFaxDecode") == 0){
    printf("Do something involving CCITTFax compression (TIFF)\n");
  }
}


