#include <stdio.h>

#ifndef FILEUTIL_H
#define FILEUTIL_H
void memopad_insertstring(FILE *, char *);
void memopad_insertinteger(FILE *, int);
void memopad_insertshort(FILE *, int);

void memopad_displaystring(FILE *, char *);
int memopad_displayinteger(FILE *, char *);
int memopad_displayshort(FILE *, char *);

typedef union memopad_internal_mint32{
  int i;
  char c[3];
} mint32;
#endif
