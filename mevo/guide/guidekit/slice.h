/*
  TiVo slice decoder

  Copyright 2000 Andrew Tridgell (tridge@samba.org)

  released under the GNU GPL v2 

  THIS PROGRAM AND DERIVATIVES MUST NOT BE USED TO AVOID
  SUBSCRIBING TO THE TIVO SERVICE IN COUNTRIES WHERE THAT
  SERVICE IS AVAILABLE. 
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define TYPE_INT 0
#define TYPE_STRING 1
#define TYPE_OBJECT 2
#define TYPE_FILE 3

#define MAX_TYPES 200
#define MAX_ATTRS 200

typedef unsigned char u8;

struct tag {
	char *name;
	int type;
};

void load_schema(char *fname);

