#include <stdio.h>

#ifndef FILEUTIL_H
#define FILEUTIL_H

void fileutil_insertstring(FILE *, char *);
void fileutil_insertinteger(FILE *, int);
void fileutil_insertshort(FILE *, int);

void fileutil_displaystring(char *, char *, long long *);
long fileutil_displaylong(char *, char *, long long *);
int fileutil_displayinteger(char *, char *, long long *);
int fileutil_displayshort(char *, char *, long long *);
char *fileutil_displaybyteblock(char *, char *, int, long long *);
int fileutil_displaybyte(char *, char *, long long *);
int fileutil_displaybyte_actual(char *, char *, long long *);
int fileutil_displaywindowslanguage(char *, char *, long long *);
int fileutil_displayencinteger(char *, char *, int *, long long *);

typedef union memopad_internal_mint32{
  int i;
  char c[3];
} mint32;

typedef union memopad_internal_mint64{
  long long i;
  char c[7];
} mint64;
#endif
