/* A sample application using pandalex -- this is pandaedit */

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <ctype.h>

#include <stack>

#include <pandalex.h>

#include "pandaedit.h"
#include "stringArray.h"
#include "objectmodel.h"

string filter;
string objtype;

pdf *thePDF;
object *currentObject = NULL;
stack<dictionary> currentDictionary;
stack<string> currentDictionaryName;

int main(int argc, char *argv[]){
  pandalex_init();

  ////////////////////////////////////////////////////////////////////////
  // Parse the file named on the command line

  // Setup the callbacks
  pandalex_setupcallback(pandalex_event_begindocument, pandaedit_begindocument);

  pandalex_setupcallback(pandalex_event_specver, pandaedit_specversion);
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
  
  // Start parsing
  pandalex_parse(argv[1]);
  printf("Finished parsing\n");

  ////////////////////////////////////////////////////////////////////////
  // Now we can attempt to render the first page of the document
  // todo_mikal: this is a hack
  object foo;

  // Find the catalog -- I could probably miss this step, but it seems like
  // a good idea for now...
  object& catalog = foo;
  if(!thePDF->findObject(dictitem::diTypeName, "Type", "Catalog", catalog)){
    fprintf(stderr, "Bad PDF: No catalog\n");
    exit(1);
  }

  // Now find the pages object as refered to by the catalog
  if(!catalog.hasDictItem(dictitem::diTypeObjectReference, "Pages")){
    fprintf(stderr, "Bad PDF: No pages object refered to in catalog\n");
    exit(1);
  }
  object& pages = foo;
  if(!catalog.getDict().getValue("Pages", *thePDF, pages)){
    fprintf(stderr, "Bad PDF: Could not get pages object, but the catalog references it!\n");
    exit(1);
  }

  // Now find all the page objects referenced in the pages object
  string kids;
  if(!pages.getDict().getValue("Kids", kids)){
    fprintf(stderr, "Bad PDF: No pages found in PDF\n");
    exit(1);
  }

  // Find the pages, and then display just the first page
  objectlist pagelist(kids, *thePDF);
  object firstpage = pagelist[0];
  if(!firstpage.hasDictItem(dictitem::diTypeObjectReference, "Contents")){
    fprintf(stderr, "Bad PDF: Page is blank\n");
    exit(1);
  }
  object& firstpagestream = foo;
  if(!firstpage.getDict().getValue("Contents", *thePDF, firstpagestream)){
    fprintf(stderr, "Bad PDF: Could not get contents object, but the page references it!\n");
    exit(1);
  }

  printf("Page stream (len %d): %s\n", firstpagestream.getStreamLength(),
	 firstpagestream.getStream());
  return 0;
}

void pandaedit_begindocument(int event, va_list argptr){
  thePDF = new pdf(va_arg(argptr, char *));
}

void pandaedit_specversion(int event, va_list argptr){
  string version = (char *) va_arg(argptr, char *);
  thePDF->setSpecVer(atof(version.c_str()));
}

// Setup an object
void pandaedit_objstart(int event, va_list argptr){
  int objnum, objgen;
  objnum = va_arg(argptr, int);
  objgen = va_arg(argptr, int);

  // Setup a global for the current object
  if(currentObject) delete currentObject;
  currentObject = new object(objnum, objgen);

  // Setup a stack of dictionaries for this object
  while(currentDictionary.size() > 0)
    currentDictionary.pop();
  while(currentDictionaryName.size() > 0)
    currentDictionaryName.pop();

  dictionary temp;
  currentDictionary.push(temp);
  currentDictionaryName.push("TOP");
}

// Now that we have finished the object, push it into the PDF
void pandaedit_objend(int event, va_list argptr){
  if(currentObject != NULL){
    if(!currentDictionary.empty()){
      printf("DEBUG: Adding dictionary\n");
      currentObject->addDictionary(currentDictionary.top());
      currentDictionary.pop();
    }

    thePDF->addObject(*currentObject);
  }
  else{
    printf("DEBUG: Attempt to finish a NULL object\n");
  }
}

void pandaedit_dictitem_string(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeString, name);
  temp.setValue(string(value));
  currentDictionary.top().add(temp);
}

void pandaedit_dictitem_name(int event, va_list argptr){
  char *name, *value;
  
  name = va_arg(argptr, char *);
  value = va_arg(argptr, char *);
 
  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeName, name);
  temp.setValue(string(value));
  currentDictionary.top().add(temp);
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

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeObjectReference, name);
  stringArray token(value, " ");
  temp.setValue(atoi(token[0].c_str()), atoi(token[1].c_str()));
  currentDictionary.top().add(temp);
}

void pandaedit_dictitem_dict(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);
  printf("Subdictionary \"%s\" starts\n", name);
}

void pandaedit_dictitem_dictend(int event, va_list argptr){
  char *name;

  name = va_arg(argptr, char *);

  if(currentDictionary.empty()){
    printf("DEBUG: A non-existant dictionary ended\n");
    return;
  }    

   if(name == NULL){
     if(currentObject != NULL){
       printf("DEBUG: Adding dictionary\n");
       currentObject->addDictionary(currentDictionary.top());
     }
     else{
       printf("DEBUG: Attempt to add a dictionary to a null object\n");
     }
     currentDictionary.pop();
   }
   else{
    printf("DEBUG: Handle sub dictionaries (%s)\n", name);
    if(!currentDictionaryName.empty())
      currentDictionaryName.pop();    
  }
}

void pandaedit_dictitem_int(int event, va_list argptr){
  int value;
  char *name;

  name = va_arg(argptr, char *);
  value = va_arg(argptr, int);

  if(currentDictionary.empty()){
    fprintf(stderr, "Adding item to empty dictionary\n");
    return;
  }

  dictitem temp(dictitem::diTypeInt, name);
  temp.setValue(value);
  currentDictionary.top().add(temp);
}

void pandaedit_stream(int event, va_list argptr){
  char *streamData;
  int streamDataLen;

  streamData = va_arg(argptr, char *);
  streamDataLen = va_arg(argptr, int);
  currentObject->addStream(streamData, streamDataLen);  
}

int pandaedit_atoi(char *str){
  if(str == NULL) return 0;
  return atoi(str);
}
