/******************************************************************************
  Copyright (C) Michael Still 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#include "trivsql_tdb.h"
#include "trivsql_spinlock.h"

#ifndef TRIVSQL_HEADER
#define TRIVSQL_HEADER

#ifdef __cplusplus
extern "C"
{
#endif

#define TRIVSQL_FALSE 0
#define TRIVSQL_TRUE 1

#define TRIVSQL_NOSUCHTABLE 2
#define TRIVSQL_BADVALUES 3
#define TRIVSQL_MEMORYERROR 4
#define TRIVSQL_TDBNULLKEY 5
#define TRIVSQL_TDBNULLDATA 6
#define TRIVSQL_TDBSTOREERROR 7
#define TRIVSQL_NOSUCHCOLUMN 8
#define TRIVSQL_NOROWSTOUPDATE 9
#define TRIVSQL_BADSELCOLARG 10
#define TRIVSQL_DBOPENFAIL 11

#define SELTRUE 1
#define SELFALSE 0

typedef int (*trivsql_selectorfunc) (char *arg1, char *arg2);

typedef struct trivsql_internal_seltreenode
{
  char *selArgOne;
  int selColOne;
  char *selArgTwo;
  int selColTwo;
  trivsql_selectorfunc selector;
  struct trivsql_internal_seltreenode *left, *right;
} trivsql_seltreenode;

typedef struct trivsql_internal_col
{
  char *val;
  char *key;
  struct trivsql_internal_col *next;
} trivsql_col;

typedef struct trivsql_internal_row
{
  trivsql_col *cols;
  struct trivsql_internal_row *next, *prev;
} trivsql_row;

typedef struct trivsql_internal_rs
{
  int numCols;
  int numRows;
  trivsql_row *rows;
  trivsql_row *currentRow;

  char *tname;
  char *cols;

  int errno;
  char *errstring;
} trivsql_recordset;

typedef struct trivsql_internal_state
{
  TDB_CONTEXT *db;
  trivsql_recordset *rs;
  trivsql_seltreenode *seltree;
  char *table;
} trivsql_state;

// Internal functions
trivsql_state *trivsql_init(char *);
void trivsql_docreate(char *, char *);
void trivsql_doinsert(char *, char *, char *);
void trivsql_doselect(char *, char *);
void trivsql_doalter(char *, char *);

int *trivsql_parsecols(char *, char *, int *);
int trivsql_findcol(char *, char *, char *);
void trivsql_addrow(trivsql_recordset *, char *, int, int *);
char *trivsql_getallcolumns(char *);
trivsql_recordset *trivsql_makers();
trivsql_seltreenode *trivsql_makest();
trivsql_seltreenode* trivsql_makesel(trivsql_selectorfunc, char *, char *);
trivsql_seltreenode* trivsql_makeslr(trivsql_selectorfunc,
				     trivsql_seltreenode *,
				     trivsql_seltreenode *);
int trivsql_executeselector(trivsql_seltreenode *, int);

void trivsql_checktable(char *, trivsql_recordset *);

void *trivsql_xmalloc(size_t);
void trivsql_dbwrite(trivsql_state *, char *, char *);
char *trivsql_dbread(trivsql_state *, char *);
char *trivsql_xsnprintf(char *, ...);
void trivsql_xfree(void *);
void *trivsql_xrealloc(void *, size_t);
int trivsql_min(int, int);
int trivsql_initok(trivsql_state *);

// Selectors
int trivsql_selequal(char *, char *);
int trivsql_sellike(char *, char *);
int trivsql_selor(int, int);
int trivsql_seland(int, int);

// Interface methods
trivsql_state *trivsql_opendb(char *);
trivsql_recordset *trivsql_execute(trivsql_state *, char *);
int trivsql_gettext(char *, int);
void trivsql_displayrs(trivsql_recordset *);
void trivsql_rsmovefirst(trivsql_recordset *);
void trivsql_rsmovenext(trivsql_recordset *);
int trivsql_rseof(trivsql_recordset *);
int trivsql_rsbof(trivsql_recordset *);
char *trivsql_rsfield(trivsql_recordset *, int);
void trivsql_updaters(trivsql_state *, trivsql_recordset *, char *, char *);
void trivsql_rsupdatefield(trivsql_state *, trivsql_recordset *, 
			   int, char *newval);

#ifdef __cplusplus
}
#endif

#endif
