#include "tdb.h"
#include "spinlock.h"

typedef struct tsql_internal_state
{
  TDB_CONTEXT *db;
} tsql_state;

typedef struct tsql_internal_rs
{
  int numFields;
  void *fields;
} tsql_rs;

tsql_state *tsql_init(char *);
void tsql_docreate(char *, char *);
void tsql_doinser(char *, char *, char *);
int tsql_doselect(tsql_state *,
		  int , char *cols[], char *, int,
		  char *conds[], tsql_rs *);

void *tsql_xmalloc(size_t);
void tsql_dbwrite(tsql_state *, char *, char *);
char *tsql_dbread(tsql_state *, char *);
char *tsql_xsnprintf(char *, ...);
void tsql_xfree(void *);
void *tsql_xrealloc(void *, size_t);
