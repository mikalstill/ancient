#include "tdb.h"
#include "spinlock.h"

typedef struct trivsql_internal_state
{
  TDB_CONTEXT *db;
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

trivsql_state *trivsql_init(char *);
void trivsql_docreate(char *, char *);
void trivsql_doinsert(char *, char *, char *);
trivsql_recordset *trivsql_doselect(char *, char *);

int *trivsql_parsecols(char *, char *);
void trivsql_displayrs(trivsql_recordset *, char *, char *);
void trivsql_addrow(trivsql_recordset *, char *, int, int *);

void *trivsql_xmalloc(size_t);
void trivsql_dbwrite(trivsql_state *, char *, char *);
char *trivsql_dbread(trivsql_state *, char *);
char *trivsql_xsnprintf(char *, ...);
void trivsql_xfree(void *);
void *trivsql_xrealloc(void *, size_t);
