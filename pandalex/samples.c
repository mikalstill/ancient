/* A sample application using pandalex -- this is pdfdump */

#include <stdarg.h>
#include <stdio.h>
#include <zlib.h>
#include "samples.h"
#include "lexinterface.h"

enum{
  pdfdump_dump = 0,
    pdfdump_meta
    };
int pdfdump_application;

pdfdump_dictint_list *dictint_list;

// Some demo code for how to use PandaLex
int main(int argc, char *argv[]){
  pandalex_init();

  // Parse the command line to find out what we are doing today -- this needs more thought
  if(strcmp(argv[0], "pdfmeta") == 0){
    pdfdump_application = pdfdump_meta;
  }
  else
    pdfdump_application = pdfdump_dump;

  // Setup the callbacks
  pandalex_setupcallback(pandalex_event_begindocument, pdfdump_begindocument);

  pandalex_setupcallback(pandalex_event_specver, pdfdump_specversion);
  pandalex_setupcallback(pandalex_event_entireheader, pdfdump_entireheader);
  pandalex_setupcallback(pandalex_event_objstart, pdfdump_objstart);

  pandalex_setupcallback(pandalex_event_dictitem_string, pdfdump_dictitem_string);
  pandalex_setupcallback(pandalex_event_dictitem_name, pdfdump_dictitem_name);
  pandalex_setupcallback(pandalex_event_dictitem_array, pdfdump_dictitem_array);
  pandalex_setupcallback(pandalex_event_dictitem_object, pdfdump_dictitem_object);
  pandalex_setupcallback(pandalex_event_dictitem_dict, pdfdump_dictitem_dict);
  pandalex_setupcallback(pandalex_event_dictitem_dictend, pdfdump_dictitem_dictend);
  pandalex_setupcallback(pandalex_event_dictitem_int, pdfdump_dictitem_int);

  pandalex_setupcallback(pandalex_event_stream, pdfdump_stream);
  pandalex_setupcallback(pandalex_event_dictint, pdfdump_dictint);
  
  // Initialise the dictint_list structure;
  if((dictint_list = (pdfdump_dictint_list *)
      malloc(sizeof(pdfdump_dictint_list))) == NULL){
    fprintf(stderr, "Could not initialise the dictint list\n");
    exit(42);
  }

  dictint_list->next = NULL;

  // Start parsing
  pandalex_parse();

  return 0;
}

char *pandalex_xsnprintf(char *, ...);

// Arguement is the name of the file as a char *
void pdfdump_begindocument(int event, va_list argptr){
  char *filename;

  filename = va_arg(argptr, char *);
  printf("Information for document: \"%s\"\n\n", filename);
}

void pdfdump_specversion(int event, va_list argptr){
  printf("Specification version is: %s\n", (char *) va_arg(argptr, char *));
}

void pdfdump_entireheader(int event, va_list argptr){
  int    i;
  char   *textMatch = (char *) va_arg(argptr, char *);

  printf("Entire document header is: ");

  for(i = 0; i < strlen(textMatch); i++){
    if(isprint(textMatch[i])) printf("%c ", textMatch[i]);
    else printf("\\%d ", textMatch[i]);
  }

  printf("\n");
}

void pdfdump_objstart(int event, va_list argptr){
  int generation, number;

  number = va_arg(argptr, int);
  generation = va_arg(argptr, int);

  printf("Object %d started (generation %d)\n",
	 number, generation);
}

void pdfdump_dictitem_string(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
  printf("  [String] %s = \"%s\"\n", name, value);
}

void pdfdump_dictitem_name(int event, va_list argptr){
  printf("Dictionary name\n");
}

void pdfdump_dictitem_array(int event, va_list argptr){
  printf("Dictionary array\n");
}

void pdfdump_dictitem_object(int event, va_list argptr){
  printf("Dictionary object\n");
}

void pdfdump_dictitem_dict(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("Subdictionary \"%s\" starts\n", name);
}

void pdfdump_dictitem_dictend(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("Subdictionary \"%s\" ends\n", name);
}

void pdfdump_dictitem_int(int event, va_list argptr){
  int value;
  char *name;

  name = va_arg(argptr, char *);
  value = va_arg(argptr, int);

  printf("  [Integer] %s = %d\n", name, value);
}

void pdfdump_stream(int event, va_list argptr){
  char *filter;
  int length;
  char *lengthObj;
  char *streamData;
  int streamDataLen;
  pdfdump_dictint_list  *now;
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
	pdfdump_procstream(filter, now->number, streamData, streamDataLen);
	found = 1;
      }

      now = now->next;
    }

    // No -- save info and wait
    if(found == 0){
      // now is already the end of the list
      if((now->next = (pdfdump_dictint_list *)
	  malloc(sizeof(pdfdump_dictint_list))) == NULL){
	fprintf(stderr, "Could not add to list of waiting streams\n");
	exit(42);
      }

      printf("Do something about waiting for the info\n");

      now->value = (char *) pandalex_strmcpy(lengthObj, -1);
      now->filter = (char *) pandalex_strmcpy(filter, -1);

      now->stream = (char *) pandalex_strmcpy(streamData + 2, streamDataLen - 2);
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
      pdfdump_procstream(filter, length, streamData, streamDataLen);
    }
    else{
      fprintf(stderr, "Stream with length having undefined meaning %d\n",
	      length);
      exit(44);
    }
  }
}

void pdfdump_dictint(int event, va_list argptr){
  int found;
  int objnum, objgen, value;
  char *objref;
  pdfdump_dictint_list *now;

  // Get the passed information
  objnum = va_arg(argptr, int);
  objgen = va_arg(argptr, int);
  value = va_arg(argptr, int);

  printf("Do something with the dictint %d %d R = %d\n", objnum, objgen, value);

  // Information is handed to this event in a slightly different manner to
  // the stream event handler. Fix this.
  if((objref = (char *) malloc((pandalex_intlen(objnum) + pandalex_intlen(objgen) + 5) * sizeof(char))) == NULL){
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
      pdfdump_procstream(now->filter, value, now->stream, now->streamlen);
      found = 1;
    }

    now = now->next;
  }

  // No -- save data and wait
  if(found == 0){
    // now is already the end of the list
    if((now->next = (pdfdump_dictint_list *)
	malloc(sizeof(pdfdump_dictint_list))) == NULL){
      fprintf(stderr, "Could not add to list of waiting streams\n");
      exit(42);
    }
    
    now->value = (char *) pandalex_strmcpy(objref, -1);
    now->filter = NULL;
    now->stream = NULL;
    now->waiting = 2;
    now = now->next;
    now->next = NULL;
  }
}

void pdfdump_procstream(char *filter, int length, char *data, int dataLen){
  char *uncompressed, *dataPtr, *linhintdesc[17];
  uLong srcLen, dstLen = 512;
  int result, i, linhintlens[17], number, count;

  linhintlens[0] = 32;
  linhintdesc[0] = pandalex_xsnprintf("Least number of objects in a page");
  linhintlens[1] = 32;
  linhintdesc[1] = pandalex_xsnprintf("Location of the first page object");
  linhintlens[2] = 16;
  linhintdesc[2] = pandalex_xsnprintf("Page objects delta bits");
  linhintlens[3] = 32;
  linhintdesc[3] = pandalex_xsnprintf("Least page length");
  linhintlens[4] = 16;
  linhintdesc[4] = pandalex_xsnprintf("Page length delta bits");
  linhintlens[5] = 32;
  linhintdesc[5] = pandalex_xsnprintf("Least content stream offset");
  linhintlens[6] = 16;
  linhintdesc[6] = pandalex_xsnprintf("Content stream offset delta bits");
  linhintlens[7] = 32;
  linhintdesc[7] = pandalex_xsnprintf("Least content stream length");
  linhintlens[8] = 16;
  linhintdesc[8] = pandalex_xsnprintf("Contents stream length delta bits");
  linhintlens[9] = 16;
  linhintdesc[9] = pandalex_xsnprintf("Greatest shared object number bits");
  linhintlens[10] = 16;
  linhintdesc[10] = pandalex_xsnprintf("Numerically greatest shared object number bits");
  linhintlens[11] = 16;
  linhintdesc[11] = pandalex_xsnprintf("Numeration object fraction bits");
  linhintlens[12] = 16;
  linhintdesc[12] = pandalex_xsnprintf("Denominator object fraction bits");
  linhintlens[13] = 16;
  linhintdesc[13] = pandalex_xsnprintf("?????");

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
    //for(i = 0; i < dataLen; i++)
    //  printf("%c", data[i]);
    //printf("--------------------------------------------------");


    // - 1
    for(i = -10; i < 1; i++){
      srcLen = dataLen + i;
      dstLen = 512;
      printf("[%d] ", i);
     
      if((uncompressed = (char *) malloc(sizeof(char) * dstLen)) == NULL){
	fprintf(stderr, "Could not make enough space to decompress Flate stream\n");
	exit(42);
      }
      
      // We grow the output buffer until we no longer get buffer size errors
      while((result = uncompress(uncompressed, &dstLen, dataPtr, srcLen)) == Z_BUF_ERROR){
	printf(".");
	fflush(stdout);
	
	dstLen *= 2;
	if(((uncompressed = (char *) realloc(uncompressed, dstLen)) == NULL) ||
	   (dstLen > 10000000)){
	  // We could not grow the buffer, so we exit
	  printf("!");
	  fflush(stdout);
	  free(uncompressed);
	  break;
	}
      }

      if(result == Z_OK) printf(" HIT");
      printf(" *\n");
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

      case Z_BUF_ERROR:
	fprintf(stderr, "buffer error\n");
	break;

      default:
	fprintf(stderr, "unknown error (%d)\n", result);
	break;
      }
      
      debuglex(data, srcLen, "Flate compression failure", 0);
      exit(46);
    }
    
    printf("\n");
    printf("----------- UNCOMPRESSED STREAM IS -------------------------------------------\n");
    // printf("%s\n", uncompressed);
    printf("Total uncompressed size: %d\n\n", dstLen);

    count = 0;

    for(i = 0; count < 13;){
      number = 0;
      if(linhintlens[count] == 16){
	number = uncompressed[i] << 8 | uncompressed[i + 1];
	i += 2;
      }
      else{
	number = uncompressed[i] << 24 | uncompressed[i + 1] << 16 |
	  uncompressed[i + 2] << 8 | uncompressed[i + 3];
	i += 4;
      }

      printf("%s [%d]: %d\n", linhintdesc[count], linhintlens[count], number);
      count++;
    }

    printf("\nTotal bytes used: %d\n", i);
    printf("\n------------------------------------------------------------------------------\n");
  }
  else if(strcmp(filter, "LZWDecode") == 0){
    printf("LZW compression is encumbered by Patents and therefore not supported\n");
  }
  else if(strcmp(filter, "CCITTFaxDecode") == 0){
    printf("Do something involving CCITTFax compression (TIFF)\n");
  }
}
