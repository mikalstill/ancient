/* A sample application using pandalex -- this is pandaedit */

#include "pandaedit.h"
#include "stringArray.h"
#include "objectmodel.h"
#include "render.h"

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
  object foo(-1, -1);

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

  pdfRender renPage(firstpagestream);
  renPage.render();

  return 0;
}

int pandaedit_atoi(char *str){
  if(str == NULL) return 0;
  return atoi(str);
}
