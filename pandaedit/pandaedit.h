#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <ctype.h>

#include <stack>

#include <pandalex.h>
#include "objectmodel.h"

#ifndef PANDAEDIT_H
#define PANDAEDIT_H

typedef void (*progressCallback) (void);

pdf *parse (char *filename, const progressCallback progress);

// Parsing interface
void pandaedit_begindocument (int, va_list);
void pandaedit_specversion (int, va_list);
void pandaedit_objstart (int, va_list);
void pandaedit_objend (int, va_list);

void pandaedit_dictitem_string (int, va_list);
void pandaedit_dictitem_name (int, va_list);
void pandaedit_dictitem_arraystart (int, va_list);
void pandaedit_dictitem_arrayitem (int, va_list);
void pandaedit_dictitem_arrayend (int, va_list);
void pandaedit_dictitem_object (int, va_list);
void pandaedit_dictitem_dict (int, va_list);
void pandaedit_dictitem_dictend (int, va_list);
void pandaedit_dictitem_int (int, va_list);

void pandaedit_stream (int, va_list);
void pandaedit_procstream (char *, int, char *, int);

int pandaedit_atoi (char *);

#endif
