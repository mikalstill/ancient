#include "trivsql.h"
#include <stdarg.h>

extern trivsql_state *gState;

trivsql_state *trivsql_init(char *filename){
  trivsql_state *state;

  state = (trivsql_state *) trivsql_xmalloc(sizeof(trivsql_state));
  state->db = tdb_open(filename, 0, 0, O_RDWR | O_CREAT, 0600);
  state->rs = NULL;

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
  char *t, *u, *c;
  int rowCount, i, col, numCols;
  int *colNumbers;

  if((rowCount = trivsql_getrowcount(tname)) == -1){
    return;
  }
  
  // Get ready for columns
  if((colNumbers = trivsql_parsecols(tname, cols, &numCols)) == NULL){
    return;
  }

  // How we have the right number of values?
  col = 1;
  for(i = 0; i < strlen(vals); i++)
    if(vals[i] == ';')
      col++;
  
  if(col != numCols){
    gState->rs->errno = TRIVSQL_BADVALUES;
    return;
  }

  // Save each column value
  c = strtok(vals, ";");
  col = 0;
  while(c != NULL){
    t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[col], rowCount);
    trivsql_dbwrite(gState, t, c);
    trivsql_xfree(t);
    
    c = strtok(NULL, ";");
    col++;
  }

  // And we should keep count of how many of the rows are in the table
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_xsnprintf("%d", rowCount + 1);
  trivsql_dbwrite(gState, t, u);
  trivsql_xfree(t);
  trivsql_xfree(u);
}

void trivsql_doselect(char *tname, char *cols){
  int *colNumbers;
  int row, rowCount, numCols, addMe, sac1, sac2;
  char *t, *u, *sa1, *sa2, *localCols;

  // If the columns list is '*', substitute a list of all the columns
  if(strcmp(cols, "*") == 0)
    localCols = trivsql_getallcolumns(tname);
  else
    localCols = cols;

  // Get ready for columns
  if((colNumbers = trivsql_parsecols(tname, localCols, &numCols)) == NULL){
    return;
  }

  // Populate recordset
  gState->rs->numCols = numCols;
  gState->rs->cols = trivsql_xsnprintf("%s", localCols);
  gState->rs->errno = TRIVSQL_TRUE;

  // Decide what rows on the table match the select condition
  if((rowCount = trivsql_getrowcount(tname)) == -1){
    return;
  }

  // Prepare the selector arguements
  if(gState->selector != NULL){
    sa1 = gState->selArgOne;
    sa2 = gState->selArgTwo;
    sac1 = trivsql_findcol(tname, localCols, sa1);
    sac2 = trivsql_findcol(tname, localCols, sa2);
  }

  for(row = 0; row < rowCount; row++){
    addMe = SELTRUE;

    if(gState->selector != NULL){
      if(sac1 != -1){
	trivsql_xfree(sa1);
	t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[sac1], row);
	sa1 = trivsql_dbread(gState, t);
	trivsql_xfree(t);
      }

      if(sac2 != -1){
	trivsql_xfree(sa2);
	t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[sac2], row);
	sa2 = trivsql_dbread(gState, t);
	trivsql_xfree(t);
      }

      addMe = (gState->selector)(sa1, sa2);
    }

    if(addMe == SELTRUE)
      trivsql_addrow(gState->rs, tname, row, colNumbers);
  }
}

void *
trivsql_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      // todo_mikal: improve this
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
    gState->rs->errno = TRIVSQL_TDBNULLKEY;
    return;
  }

  if (value == NULL){
    gState->rs->errno = TRIVSQL_TDBNULLDATA;
    return;
  }

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;
  dbdata.dptr = value;
  dbdata.dsize = strlen (value) + 1;

  if (tdb_store (state->db, dbkey, dbdata, TDB_REPLACE) != 0)
    {
      gState->rs->errno = TRIVSQL_TDBSTOREERROR;
      return;
    }
}

char *
trivsql_dbread (trivsql_state *state, char *key)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL){
    gState->rs->errno = TRIVSQL_TDBNULLKEY;
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

int *trivsql_parsecols(char *tname, char *cols, int *numCols){
  int i, col;
  int *colNumbers = NULL;
  char *t, *u, *coltmp, *c;

  // How many columns do we have?
  *numCols = 1;
  for(i = 0; i < strlen(cols); i++)
    if(cols[i] == ';')
      (*numCols)++;

  coltmp = trivsql_xsnprintf("%s", cols);
  colNumbers = trivsql_xmalloc(sizeof(int) * (*numCols));
  
  // Determine that the named columns exist
  col = 0;
  c = strtok(coltmp, ";");
  while(c != NULL){
    i = 0;
    while(1){
      t = trivsql_xsnprintf("trivsql_%s_col%d", tname, i);
      u = trivsql_dbread(gState, t);

      if(u == NULL){
	trivsql_xfree(t);
	gState->rs->errno = TRIVSQL_NOSUCHCOLUMN;
	return NULL;
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

  return colNumbers;
}

int trivsql_findcol(char *tname, char *cols, char *col){
  char *t, *u, *coltmp, *c;
  int colNum;

  coltmp = trivsql_xsnprintf("%s", cols);
  colNum = 0;

  // Determine that the named columns exist
  c = strtok(coltmp, ";");
  while(c != NULL){
    if(strcmp(c, col) == 0)
      return colNum;

    c = strtok(NULL, ";");
    colNum++;
  }

  return -1;
}

int trivsql_getrowcount(char *tname){
  char *t, *u;
  int rowCount;

  // Determine if the table exists, and if so how many rows it has
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_dbread(gState, t);
  
  if(u == NULL){
    gState->rs->errno = TRIVSQL_NOSUCHTABLE;
    return -1;
  }

  rowCount = atoi(u);
  trivsql_xfree(u);
  trivsql_xfree(t);

  return rowCount;
}

void trivsql_addrow(trivsql_recordset *rs, char *tname, int row, int *cols){
  char *t;
  int colCount;
  trivsql_row *theRow;
  trivsql_col *theCol;

  // Make space for the new row
  rs->numRows++;
  theRow = rs->rows;
  while(theRow->next != NULL)
    theRow = theRow->next;

  theRow->next = trivsql_xmalloc(sizeof(trivsql_row));
  theRow->next->next = NULL;

  theRow->cols = trivsql_xmalloc(sizeof(trivsql_col));
  theRow->cols->next = NULL;
  theCol = theRow->cols;

  // Get the row
  for(colCount = 0; colCount < rs->numCols; colCount++){
    t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, cols[colCount], row);

    theCol->val = trivsql_dbread(gState, t);
    theCol->next = trivsql_xmalloc(sizeof(trivsql_col));
    theCol->next->next = NULL;
    theCol = theCol->next;

    trivsql_xfree(t);
  }
}

char *trivsql_getallcolumns(char *tname)
{
  char *t, *u, *retVal, *retVal2;
  int i, maxCols;

  t = trivsql_xsnprintf("trivsql_%s_numcols", tname);
  u = trivsql_dbread(gState, t);
  maxCols = atoi(u);
  trivsql_xfree(t);
  trivsql_xfree(u);

  retVal = trivsql_xsnprintf("");

  for(i = 0; i < maxCols; i++){
      t = trivsql_xsnprintf("trivsql_%s_col%d", tname, i);
      u = trivsql_dbread(gState, t);

      if(strcmp(retVal, "") != 0)
	retVal2 = trivsql_xsnprintf("%s;%s", retVal, u);
      else
	retVal2 = trivsql_xsnprintf("%s", u);

      trivsql_xfree(t);
      trivsql_xfree(u);
      trivsql_xfree(retVal);
      retVal = retVal2;
  }

  return retVal;
} 

int trivsql_min(int a, int b){
  if(a > b) return b;
  return a;
}

int trivsql_checktable(char *tname){
  char *t, *u;
  
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_dbread(gState, t);
  trivsql_xfree(t);
 
  if(u == NULL)
    return TRIVSQL_NOSUCHTABLE;
  return TRIVSQL_FALSE;
}

trivsql_recordset* trivsql_makers(char *tname){
  trivsql_recordset *rrs;

  // Build the recordset
  rrs = trivsql_xmalloc(sizeof(trivsql_recordset));
  rrs->rows = trivsql_xmalloc(sizeof(trivsql_row));
  rrs->rows->next = NULL;
  rrs->rows->cols = NULL;
  rrs->numCols = 0;
  rrs->numRows = 0;
  rrs->tname = trivsql_xsnprintf("%s", tname);
  rrs->currentRow = rrs->rows;
  rrs->errno = TRIVSQL_FALSE;
  rrs->cols = NULL;

  return rrs;
}
