%{
  #include <stdio.h>
  #include <stdarg.h>
  #include "trivsql.h"

  #define YYERROR_VERBOSE 1

  trivsql_state *gState;
%}

%token CREATE TABLE 
%token INSERT VALUES INTO
%token SELECT FROM STRING

%%

sql      : create sql | insert sql | sel sql
         |
         ;

create   : CREATE TABLE STRING '(' colvalspec ')' ';' 
{ trivsql_docreate($3, $5); } 
         ;

insert   : INSERT INTO STRING '(' colvalspec ')' VALUES '(' colvalspec ')' ';'
{ trivsql_doinsert($3, $5, $9); }
         ;

sel      : SELECT '*' FROM STRING ';' 
{ trivsql_rs *rs; trivsql_doselect(gState, -1, NULL, (char *) $4, -1, NULL, rs); }
         ;

colvalspec : STRING ',' colvalspec 
{ $$ = trivsql_xsnprintf("%s;%s", $1, $3); } 
         | '\'' STRING '\'' ',' colvalspec 
{ $$ = trivsql_xsnprintf("%s;%s", $2, $5); } 
         | STRING 
{ $$ = trivsql_xsnprintf("%s", $1); }
         | '\'' STRING '\''
{ $$ = trivsql_xsnprintf("%s", $2); }
         |
         ;

%%

int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  gState = trivsql_init("foo.tdb");
  yyparse();

  return 0;
}

trivsql_state *trivsql_init(char *filename){
  trivsql_state *state;

  state = (trivsql_state *) trivsql_xmalloc(sizeof(trivsql_state));
  state->db = tdb_open(filename, 0, 0, O_RDWR | O_CREAT, 0600);

  return state;
}

void trivsql_docreate(char *tname, char *cols)
{
  char *t;
  char *u;

  int colCount = 0;

  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  trivsql_dbwrite(gState, t, "0");
  trivsql_xfree(t);

  u = strtok(cols, ";");
  while(u != NULL){
    printf("[%s]\n", u);

    t = trivsql_xsnprintf("trivsql_%s_col%d", tname, colCount);
    trivsql_dbwrite(gState, t, u);
    trivsql_xfree(t);

    colCount++;
    u = strtok(NULL, ";");
  }

  t = trivsql_xsnprintf("trivsql_%s_numcols", tname);
  u = trivsql_xsnprintf("%d", colCount);
  trivsql_dbwrite(gState, t, u);
  trivsql_xfree(t);
  trivsql_xfree(u);
} 

void trivsql_doinsert(char *tname, char *cols, char *vals){
  char *t;
  char *u;
  char *c;
  int rowNumber;
  int *colNumbers;
  int i;
  int col;
  int numCols;

  // Determine if the table exists, and if so how many rows it has
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_dbread(gState, t);
  
  if(u == NULL){
    fprintf(stderr, "Table does not exist\n");
    return;
  }
  rowNumber = atoi(u);
  trivsql_xfree(u);
  trivsql_xfree(t);
  
  // How many columns do we have?
  for(i = 0, numCols = 0; i < strlen(cols); i++)
    if(cols[i] == ';')
      numCols++;
  
  colNumbers = trivsql_xmalloc(sizeof(int) * numCols);
  
  // Determine that the named columns exist
  col = 0;
  c = strtok(cols, ";");
  while(c != NULL){
    i = 0;
    while(1){
      t = trivsql_xsnprintf("trivsql_%s_col%d", tname, i);
      u = trivsql_dbread(gState, t);

      if(u == NULL){
	trivsql_xfree(t);
	fprintf(stderr, "%s is an unknown column\n", c);
	return;
      }
      else if(strcmp(u, c) == 0){
	trivsql_xfree(t);
	trivsql_xfree(u);
	break;
      }

      trivsql_xfree(t);
      trivsql_xfree(u);
      i++;
    }

    colNumbers[col] = i;
    c = strtok(NULL, ";");
    col++;
  }

  // How we have the right number of values?
  for(i = 0, col = 0; i < strlen(vals); i++)
    if(vals[i] == ';')
      col++;
  
  if(col != numCols){
    fprintf(stderr, "The number of values does not match the number of columns specified (%d cols, %d vals)\n", numCols, col);
    return;
  }

  
}

int trivsql_doselect(trivsql_state *state,
		  int colc, char *cols[], char *table, int condc, 
		  char *conds[], trivsql_rs *rs){
 
  int i, rc;
  char *t, *u;
  
  // Check that all the columns exist
  for(i = 0; i < colc; i++){
    // todo_mikal
  }

  // If we're getting all the columns
  if(colc == -1){
    t = trivsql_xsnprintf("trivsql_%s_numcols", table);
    u = trivsql_dbread(state, t);

    if(!u){
      trivsql_xfree(t);
      return -1;
    }

    trivsql_xfree(t);
    colc=atoi(u);
    trivsql_xfree(u);
  }

  // Build an empty record set
  rs = (trivsql_rs *) trivsql_xmalloc(sizeof(trivsql_rs));
  rs->numFields = colc;

  // Find all the rows which match this selection
  t = trivsql_xsnprintf("trivsql_%s_numrows", table);
  u = trivsql_dbread(state, t);

  if(!u){
    trivsql_xfree(t);
    return -2;
  }

  trivsql_xfree(t);
  rc=atoi(u);
  trivsql_xfree(u);

  for(i = 0; i < rc; i++){
    printf(".");
  }

  return 0;
}

void *
trivsql_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      fprintf(stderr, "trivsql memory allocation error");
      exit(42);
    }

  return buffer;
}

void
trivsql_dbwrite (trivsql_state *state, char *key, char *value)
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
trivsql_dbread (trivsql_state *state, char *key)
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
trivsql_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = (char *) trivsql_xrealloc (output, size);
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
trivsql_xfree (void *memory)
{
  if (memory != NULL)
    free(memory);
}

void *
trivsql_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      fprintf(stderr, "Realloc of memory failed");
      exit(42);
    }

  return buffer;
}

