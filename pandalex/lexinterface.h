// This file defines the callbacks that users can setup to use PandaLex

#include <stdarg.h>

enum{
  pandalex_event_begindocument = 0,
    pandalex_event_specver,
    pandalex_event_entireheader,
    pandalex_event_objstart,
    pandalex_event_dictitem_string,
    pandalex_event_dictitem_name,
    pandalex_event_dictitem_array,
    pandalex_event_dictitem_object,
    pandalex_event_dictitem_dict,
    pandalex_event_dictitem_int,
    pandalex_event_stream,
    pandalex_event_dictint,
    pandalex_event_max
    };

// Callbacks are defined so that they have a type for the arguments they
// possess associated with them. Where possible the arguments created by the
// lexer will be converted into the types needed by the callback. If not,
// an error is returned
typedef void (*pandalex_callback_type)(int, va_list);

/******************************************************************************
  This function is used to setup callbacks for the PandaLex parser
******************************************************************************/

void pandalex_setupcallback(int, pandalex_callback_type);
void pandalex_callback(int, ...);

