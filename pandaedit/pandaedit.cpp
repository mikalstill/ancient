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
bool streamIsContents;

plot_state *bitmap = NULL;
int objnum = 0;
int objgen = 0;

typedef struct{
  int objnum;
  int objgen;
  string contents;
} pandaedit_objitem;

vector<pandaedit_objitem> pagelist;

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

  // Is this object in the list of pages?
  for(unsigned int i = 0; i < pagelist.size(); i++){
    if((pagelist[i].objnum == objnum) &&
       (pagelist[i].objgen == objgen)){
      objtype = "Page";
      return;
    }
  }
}

void pandaedit_objend(int event, va_list argptr){
  int generation, number;

  number = va_arg(argptr, int);
  generation = va_arg(argptr, int);

  printf("Object %d ended (generation %d)\n",
	 number, generation);

  // Reset to there being no filter, for the next object
  filter = "";
  streamIsContents = false;
}

void pandaedit_dictitem_string(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);

  if(strcmp(name, "Filter") == 0){
    filter = value;
  }

  // Kids is used to list the pages within the document -- this
  // is overly simplistic at the moment (todo_mikal)
  else if((objtype ==  "Pages") && 
	  (strcmp(name, "Kids") == 0)){
    stringArray tokens(value + 1, " ");
    int inset = 0;
    while(tokens[inset] != ""){
      pandaedit_objitem temp;
      temp.objnum = atoi(tokens[inset++].c_str());
      temp.objgen = atoi(tokens[inset++].c_str());
      pagelist.push_back(temp);
      inset++;
    }
    return;
  }

  // MediaBox defines the page size e.g. [0 0 594 841]
  else if(strcmp(name, "MediaBox") == 0){
    stringArray tokens(value + 1, "[");
    if((bitmap = plot_newplot(atoi(tokens[2].c_str()), atoi(tokens[3].c_str()))) == NULL){
      fprintf(stderr, "Could not allocate a new page bitmap\n");
      exit(1);
    }
    return;
  }

  // Display unhandled values
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

  // Each page has a contents object, which contains the stream which
  // defines the layout of the page
  if((objtype == "Page") &&
     (strcmp("Contents", name) == 0)){
    streamIsContents = true;
    return;
  }

  // We never care about parent objects
  if(strcmp("Parent", name) == 0) return;

  // Pages objects are references by the type field
  if(strcmp("Pages", name) == 0) return;

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
  // todo_mikal: process stream data
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
