#include <stdio.h>

#ifndef FILEUTIL_H
#define FILEUTIL_H

void fileutil_insertstring(FILE *, char *);
void fileutil_insertinteger(FILE *, int);
void fileutil_insertshort(FILE *, int);

void fileutil_displaystring(FILE *, char *);
long fileutil_displaylong(FILE *, char *);
int fileutil_displayinteger(FILE *, char *);
int fileutil_displayshort(FILE *, char *);
char *fileutil_displaybyteblock(FILE *, char *, int);
int fileutil_displaybyte(FILE *, char *);
int fileutil_displaybyte_actual(FILE *, char *);
int fileutil_displaywindowslanguage(FILE *, char *);
int fileutil_displayencinteger(FILE *, char *, int *);

typedef union memopad_internal_mint32{
  int i;
  char c[3];
} mint32;

typedef union memopad_internal_mint64{
  long long i;
  char c[7];
} mint64;
#endif
