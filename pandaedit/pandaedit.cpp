/* A sample application using pandalex -- this is pandaedit */

#include "pandaedit.h"
#include "stringArray.h"
#include "objectmodel.h"
#include "render.h"

string filter;
string objtype;

// todo_mikal: make multithreaded?
pdf *thePDF;
object *currentObject = NULL;
stack < dictionary > currentDictionary;
stack < string > currentDictionaryName;
progressCallback gProgress = NULL;

pdf *
pandaedit (char *filename, const progressCallback progress)
{
  pandalex_init ();

  // Setup the callbacks
  pandalex_setupcallback (pandalex_event_begindocument,
			  pandaedit_begindocument);

  pandalex_setupcallback (pandalex_event_specver, pandaedit_specversion);
  pandalex_setupcallback (pandalex_event_objstart, pandaedit_objstart);
  pandalex_setupcallback (pandalex_event_objend, pandaedit_objend);

  pandalex_setupcallback (pandalex_event_dictitem_string,
			  pandaedit_dictitem_string);
  pandalex_setupcallback (pandalex_event_dictitem_name,
			  pandaedit_dictitem_name);
  pandalex_setupcallback (pandalex_event_dictitem_arraystart,
			  pandaedit_dictitem_arraystart);
  pandalex_setupcallback (pandalex_event_dictitem_arrayitem,
			  pandaedit_dictitem_arrayitem);
  pandalex_setupcallback (pandalex_event_dictitem_arrayend,
			  pandaedit_dictitem_arrayend);
  pandalex_setupcallback (pandalex_event_dictitem_object,
			  pandaedit_dictitem_object);
  pandalex_setupcallback (pandalex_event_dictitem_dict,
			  pandaedit_dictitem_dict);
  pandalex_setupcallback (pandalex_event_dictitem_dictend,
			  pandaedit_dictitem_dictend);
  pandalex_setupcallback (pandalex_event_dictitem_int,
			  pandaedit_dictitem_int);

  pandalex_setupcallback (pandalex_event_stream, pandaedit_stream);

  // Start parsing
  gProgress = progress;
  pandalex_parse (filename);
  return thePDF;
}

int
pandaedit_atoi (char *str)
{
  if (str == NULL)
    return 0;
  return atoi (str);
}
