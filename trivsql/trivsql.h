#include "tdb.h"
#include "spinlock.h"

typedef struct trivsql_internal_state
{
  TDB_CONTEXT *db;
} trivsql_state;

typedef struct trivsql_internal_rs
{
  int numFields;
  void *fields;
} trivsql_rs;

trivsql_state *trivsql_init(char *);
void trivsql_docreate(char *, char *);
void trivsql_doinsert(char *, char *, char *);
int trivsql_doselect(trivsql_state *,
		  int , char *cols[], char *, int,
		  char *conds[], trivsql_rs *);

void *trivsql_xmalloc(size_t);
void trivsql_dbwrite(trivsql_state *, char *, char *);
char *trivsql_dbread(trivsql_state *, char *);
char *trivsql_xsnprintf(char *, ...);
void trivsql_xfree(void *);
void *trivsql_xrealloc(void *, size_t);
