#include "tdb.h"
#include "spinlock.h"

#define SELTRUE 1
#define SELFALSE 0
typedef int (*trivsql_selectorfunc) (char *arg1, char *arg2);

typedef struct trivsql_internal_state
{
  TDB_CONTEXT *db;
  char *selArgOne;
  char *selArgTwo;
  trivsql_selectorfunc selector;
} trivsql_state;

typedef struct trivsql_internal_col
{
  char *val;
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
} trivsql_recordset;

// Internal functions
trivsql_state *trivsql_init(char *);
void trivsql_docreate(char *, char *);
void trivsql_doinsert(char *, char *, char *);
trivsql_recordset *trivsql_doselect(char *, char *);

int *trivsql_parsecols(char *, char *, int *);
int trivsql_findcol(char *, char *, char *);
void trivsql_displayrs(trivsql_recordset *, char *, char *);
void trivsql_addrow(trivsql_recordset *, char *, int, int *);
char *trivsql_getallcolumns(char *);

void *trivsql_xmalloc(size_t);
void trivsql_dbwrite(trivsql_state *, char *, char *);
char *trivsql_dbread(trivsql_state *, char *);
char *trivsql_xsnprintf(char *, ...);
void trivsql_xfree(void *);
void *trivsql_xrealloc(void *, size_t);
int trivsql_min(int, int);

// Selectors
int trivsql_selequal(char *, char *);
int trivsql_sellike(char *, char *);

// Interface methods
trivsql_state *trivsql_opendb(char *);
trivsql_recordset *trivsql_execute(trivsql_state *, char *);
int trivsql_gettext(char *, int);
