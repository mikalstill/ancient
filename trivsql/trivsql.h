#include "tdb.h"
#include "spinlock.h"

#define TRIVSQL_TRUE 1
#define TRIVSQL_FALSE 0
#define SELTRUE 1
#define SELFALSE 0
typedef int (*trivsql_selectorfunc) (char *arg1, char *arg2);

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
  trivsql_row *currentRow;

  char *tname;
  char *cols;
} trivsql_recordset;

typedef struct trivsql_internal_state
{
  TDB_CONTEXT *db;
  char *selArgOne;
  char *selArgTwo;
  trivsql_selectorfunc selector;
  trivsql_recordset *rs;
} trivsql_state;

// Internal functions
trivsql_state *trivsql_init(char *);
void trivsql_docreate(char *, char *);
void trivsql_doinsert(char *, char *, char *);
trivsql_recordset *trivsql_doselect(char *, char *);

int *trivsql_parsecols(char *, char *, int *);
int trivsql_findcol(char *, char *, char *);
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
void trivsql_displayrs(trivsql_recordset *);
void trivsql_rsmovefirst(trivsql_recordset *);
void trivsql_rsmovenext(trivsql_recordset *);
int trivsql_rseof(trivsql_recordset *);
int trivsql_rsbof(trivsql_recordset *);
char *trivsql_rsfield(trivsql_recordset *, int);
