// This file defines the callbacks that users can setup to use PandaLex

#ifndef PANDALEX_H
#define PANDALEX_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

  enum{
  pandalex_event_begindocument = 0,
    pandalex_event_specver,
    pandalex_event_entireheader,
    pandalex_event_objstart,
    pandalex_event_objend,
    pandalex_event_dictitem_string,
    pandalex_event_dictitem_name,
    pandalex_event_dictitem_arraystart,
    pandalex_event_dictitem_arrayitem,
    pandalex_event_dictitem_arrayend,
    pandalex_event_dictitem_object,
    pandalex_event_dictitem_dict,
    pandalex_event_dictitem_dictend,
    pandalex_event_dictitem_int,
    pandalex_event_stream,
    pandalex_event_dictint,
    pandalex_event_xrefstart,
    pandalex_event_xrefitem,
    pandalex_event_xrefend,
    pandalex_event_trailerstart,
    pandalex_event_trailerend,
    pandalex_event_enddocument,
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
  
  /******************************************************************************
  These functions manage parsing
  ******************************************************************************/
  
  int pandalex_parse(char *filename);
  int pandalex_endparse();
  
  // Other stuff
  void pandalex_init();
  void debuglex(char *, int, char *);
  char *returnStr(char *, int);
  void *pandalex_xmalloc(size_t);
  void *pandalex_xrealloc(void *, size_t);
  char *pandalex_xsnprintf(char *, ...);
  void pandalex_error(char *);
  
  void pandalex_xfree(void *);
  char *pandalex_strmcat(char *, int, char *, int);
  char *pandalex_strmcpy(char *, int);
  int pandalex_intlen(int);
  
#ifdef __cplusplus
}
#endif

#endif
