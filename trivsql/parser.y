%{
  #include <stdio.h>
  #include <stdarg.h>
  #include "tsql.h"

  #define YYERROR_VERBOSE 1

  tsql_state *gState;
%}

%token CREATE TABLE 
%token INSERT VALUES INTO
%token SELECT FROM STRING

%%

sql      : create | insert | sel
         ;

create   : CREATE TABLE STRING '(' colvalspec ')' ';' 
{ tsql_docreate($3, $5); } 
         ;

insert   : INSERT '(' colvalspec ')' INTO STRING VALUES '(' colvalspec ')'
{ doinsert($6, $3, $9); }
         ;

sel      : SELECT '*' FROM STRING ';' 
{ tsql_rs *rs; tsql_doselect(gState, -1, NULL, (char *) $4, -1, NULL, rs); }
         ;

colvalspec : STRING ',' colvalspec 
{ $$ = tsql_xsnprintf("%s;%s", $1, $3); } 
         | STRING 
{ $$ = tsql_xsnprintf("%s", $1); }
         ;

%%

int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  gState = tsql_init("foo.tdb");
  yyparse();

  return 0;
}

tsql_state *tsql_init(char *filename){
  tsql_state *state;

  state = (tsql_state *) tsql_xmalloc(sizeof(tsql_state));
  state->db = tdb_open(filename, 0, 0, O_RDWR | O_CREAT, 0600);

  return state;
}

void tsql_docreate(char *tname, char *cols)
{
  char *t;
  char *u;

  int colCount = 0;

  t = tsql_xsnprintf("tsql_%s_numrows", tname);
  tsql_dbwrite(gState, t, "0");
  tsql_xfree(t);

  u = strtok(cols, ";");
  while(u != NULL){
    printf("[%s]\n", u);

    t = tsql_xsnprintf("tsql_%s_col%d", tname, colCount);
    tsql_dbwrite(gState, t, u);
    tsql_xfree(t);

    colCount++;
    u = strtok(NULL, ";");
  }

  t = tsql_xsnprintf("tsql_%s_numcols", tname);
  u = tsql_xsnprintf("%d", colCount);
  tsql_dbwrite(gState, t, u);
  tsql_xfree(t);
  tsql_xfree(u);
} 

void tsql_doinsert(char *tname, char *cols, char *vals){
  char *t;
  char *u;
  char *col;
  int rownum;
  int colnum;
  int colCount;

  t = tsql_xsnprintf("tsql_%s_numrows", tname);
  u = tsql_dbread(gState, t);
  rownum = atoi(u);
  tsql_xfree(t);
  tsql_xfree(u);

  col = strtok(cols, ";");
  while(col != NULL){
    printf("[%s]\n", col);

    t = tsql_xsnprintf("tsql_%s_col%d", tname, colCount);
    tsql_dbwrite(gState, t, col);
    tsql_xfree(t);

    colCount++;
    u = strtok(NULL, ";");
  }
  
}

int tsql_doselect(tsql_state *state,
		  int colc, char *cols[], char *table, int condc, 
		  char *conds[], tsql_rs *rs){
 
  int i, rc;
  char *t, *u;
  
  // Check that all the columns exist
  for(i = 0; i < colc; i++){
    // todo_mikal
  }

  // If we're getting all the columns
  if(colc == -1){
    t = tsql_xsnprintf("tsql_%s_numcols", table);
    u = tsql_dbread(state, t);

    if(!u){
      tsql_xfree(t);
      return -1;
    }

    tsql_xfree(t);
    colc=atoi(u);
    tsql_xfree(u);
  }

  // Build an empty record set
  rs = (tsql_rs *) tsql_xmalloc(sizeof(tsql_rs));
  rs->numFields = colc;

  // Find all the rows which match this selection
  t = tsql_xsnprintf("tsql_%s_numrows", table);
  u = tsql_dbread(state, t);

  if(!u){
    tsql_xfree(t);
    return -2;
  }

  tsql_xfree(t);
  rc=atoi(u);
  tsql_xfree(u);

  for(i = 0; i < rc; i++){
    printf(".");
  }

  return 0;
}

void *
tsql_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      fprintf(stderr, "tsql memory allocation error");
      exit(42);
    }

  return buffer;
}

void
tsql_dbwrite (tsql_state *state, char *key, char *value)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL){
    fprintf(stderr, "Cannot store NULL key\n");
    return;
  }

  if (value == NULL){
    fprintf(stderr, "Cannot store NULL data\n");
    return;
  }

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;
  dbdata.dptr = value;
  dbdata.dsize = strlen (value) + 1;

  if (tdb_store (state->db, dbkey, dbdata, TDB_REPLACE) != 0)
    {
      fprintf(stderr, "Database storage error\n");
      return;
    }
}

char *
tsql_dbread (tsql_state *state, char *key)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL){
    fprintf(stderr, "Cannot lookup a NULL key\n");
    return NULL;
  }

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;

  dbdata = tdb_fetch (state->db, dbkey);

  return dbdata.dptr;
}

char *
tsql_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = (char *) tsql_xrealloc (output, size);
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
        {
          /* Up to glibc 2.0.6 and Microsoft's implementation */
          size += 100;
        }
      else
        {
          /* Check if we are done */
          if (result < size)
            break;

          /* Glibc from now on */
          size = result + 1;
        }
    }

  va_end (ap);
  return output;
}

void
tsql_xfree (void *memory)
{
  if (memory != NULL)
    free(memory);
}

void *
tsql_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      fprintf(stderr, "Realloc of memory failed");
      exit(42);
    }

  return buffer;
}

