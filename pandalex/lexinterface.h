// This file defines the callbacks that users can setup to use PandaLex

#include <stdarg.h>

#define gpandalex_callback_specver              0
#define gpandalex_callback_entireheader         1
#define gpandalex_callback_objstart             2
#define gpandalex_callback_dictitem_string      3
#define gpandalex_callback_dictitem_name        4
#define gpandalex_callback_dictitem_array       5
#define gpandalex_callback_dictitem_object      6
#define gpandalex_callback_dictitem_dict        7
#define gpandalex_callback_dictitem_int         8
#define gpandalex_callback_stream               9
#define gpandalex_callback_dictint              10
#define gpandalex_callback_max                  11

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

