#include <stdio.h>

#ifndef FILEUTIL_H
#define FILEUTIL_H

void fileutil_insertstring(FILE *, char *);
void fileutil_insertinteger(FILE *, int);
void fileutil_insertshort(FILE *, int);

void fileutil_displaystring(FILE *, char *);
int fileutil_displayinteger(FILE *, char *);
int fileutil_displayshort(FILE *, char *);
int fileutil_displaywindowslanguage(FILE *, char *);

typedef union memopad_internal_mint32{
  int i;
  char c[3];
} mint32;
#endif
