// Sample callback functions -- note that multiple varargian arguments
// are presented in reverse order...

#include <stdarg.h>
#include <stdio.h>
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

  streamData = va_arg(argptr, char *);
  lengthObj = va_arg(argptr, char *);
  length = (int) va_arg(argptr, char *);
  filter = va_arg(argptr, char *);

  // Determine if we know the length
  switch(length){
  case -2:
    // We don't - have we seen the obj already?
    now = dictint_list;

    while((dictint_list->next != NULL) && (found == 0)){
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

      now = now->next;
      now->value = (char *) strmcpy(lengthObj);
      now->filter = (char *) strmcpy(filter);
      now->stream = (char *) strmcpy(streamData);
      now->waiting = 1;
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
      fprintf(stderr, "Stream with length having undefined meaning\n");
      exit(44);
    }
  }
}

void pandalex_sample_dictint(int event, va_list argptr){
  // Are we already waiting?
  // Yes -- do something
  // No -- save data and wait
}

void pandalex_sample_procstream(char *filter, int length, char *data){
  // Do something with the stream
}


