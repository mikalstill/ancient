/* A sample application using pandalex -- this is pandaedit */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <ctype.h>

#include <pandalex.h>
#include <libmplot.h>

#include <vector>
#include <string>

#include "pandaedit.h"
#include "stringArray.h"

string filter;
string objtype;
int objnum = 0;
int objgen = 0;



// Some demo code for how to use PandaLex
int main(int argc, char *argv[]){
  pandalex_init();

  // Setup the callbacks
  pandalex_setupcallback(pandalex_event_begindocument, pandaedit_begindocument);

  pandalex_setupcallback(pandalex_event_specver, pandaedit_specversion);
  pandalex_setupcallback(pandalex_event_entireheader, pandaedit_entireheader);
  pandalex_setupcallback(pandalex_event_objstart, pandaedit_objstart);
  pandalex_setupcallback(pandalex_event_objend, pandaedit_objend);

  pandalex_setupcallback(pandalex_event_dictitem_string, pandaedit_dictitem_string);
  pandalex_setupcallback(pandalex_event_dictitem_name, pandaedit_dictitem_name);
  pandalex_setupcallback(pandalex_event_dictitem_arraystart, pandaedit_dictitem_arraystart);
  pandalex_setupcallback(pandalex_event_dictitem_arrayitem, pandaedit_dictitem_arrayitem);
  pandalex_setupcallback(pandalex_event_dictitem_arrayend, pandaedit_dictitem_arrayend);
  pandalex_setupcallback(pandalex_event_dictitem_object, pandaedit_dictitem_object);
  pandalex_setupcallback(pandalex_event_dictitem_dict, pandaedit_dictitem_dict);
  pandalex_setupcallback(pandalex_event_dictitem_dictend, pandaedit_dictitem_dictend);
  pandalex_setupcallback(pandalex_event_dictitem_int, pandaedit_dictitem_int);

  pandalex_setupcallback(pandalex_event_stream, pandaedit_stream);
  pandalex_setupcallback(pandalex_event_dictint, pandaedit_dictint);
  
  // Start parsing
  pandalex_parse(argv[1]);

  return 0;
}

char *pandalex_xsnprintf(char *, ...);

// Arguement is the name of the file as a char *
void pandaedit_begindocument(int event, va_list argptr){
  char *filename;

  filename = va_arg(argptr, char *);
  printf("Information for document: \"%s\"\n\n", filename);
}

void pandaedit_specversion(int event, va_list argptr){
  //  printf("Specification version is: %s\n", (char *) va_arg(argptr, char *));
}

void pandaedit_entireheader(int event, va_list argptr){
  int    i;
  char   *textMatch = (char *) va_arg(argptr, char *);

//   printf("Entire document header is: ");

//   for(i = 0; i < strlen(textMatch); i++){
//     if(isprint(textMatch[i])) printf("%c ", textMatch[i]);
//     else printf("\\%d ", textMatch[i]);
//   }

//   printf("\n");y
}

void pandaedit_objstart(int event, va_list argptr){
  objnum = va_arg(argptr, int);
  objgen = va_arg(argptr, int);

  printf("Object %d started (generation %d)\n",
	 objnum, objgen);
}

void pandaedit_objend(int event, va_list argptr){
  int generation, number;

  number = va_arg(argptr, int);
  generation = va_arg(argptr, int);

  printf("Object %d ended (generation %d)\n",
	 number, generation);

  // Reset to there being no filter, for the next object
  filter = "";
}

void pandaedit_dictitem_string(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
  printf("  [String] %s = \"%s\"\n", name, value);
}

void pandaedit_dictitem_name(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
  
  // Remember the current object type
  if(strcmp("Type", name) == 0){
    objtype = value;\
    return;
  }

  // Unhandled names
  printf("  [Name] %s = %s\n", name, value);
}

void pandaedit_dictitem_arraystart(int event, va_list argptr){
  char *name;
  
  name = va_arg(argptr, char *);
  printf("  Array %s starts\n", name);
}

void pandaedit_dictitem_arrayitem(int event, va_list argptr){
  char *value;
  
  value = va_arg(argptr, char *);
  printf("  [Array] %s\n", value);
}

void pandaedit_dictitem_arrayend(int event, va_list argptr){
  char *name;
  
  name = va_arg(argptr, char *);
  printf("  Array %s ends\n", name);
}

void pandaedit_dictitem_object(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
  printf("  [Object reference] %s = %s\n", name, value);
}

void pandaedit_dictitem_dict(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("Subdictionary \"%s\" starts\n", name);
}

void pandaedit_dictitem_dictend(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("Subdictionary \"%s\" ends\n", name);
}

void pandaedit_dictitem_int(int event, va_list argptr){
  int value;
  char *name;

  name = va_arg(argptr, char *);
  value = va_arg(argptr, int);

  // We don't use this page count
  if((objtype == "Pages") &&
     (strcmp("Count", name) == 0))
    return;

  // We also don't trust the length packed into the object
  if(strcmp("Length", name) == 0) return;

  printf("  [Integer] %s = %d\n", name, value);
}

void pandaedit_stream(int event, va_list argptr){
  char *streamData;
  int streamDataLen;

  streamData = va_arg(argptr, char *);
  streamDataLen = va_arg(argptr, int);
  
  // If this is a content stream and we have a bitmap, then we can
  // attempt to render the stream
  
}

void pandaedit_dictint(int event, va_list argptr){
  int found;
  int objnum, objgen, value;
  char *objref;

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
}

int pandaedit_atoi(char *str){
  if(str == NULL) return 0;
  return atoi(str);
}
