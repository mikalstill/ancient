%{
  #include <stdio.h>
  #include "trivsql.h"

  #define YYERROR_VERBOSE 1

  trivsql_state *gState;
%}

%token CREATE TABLE 
%token INSERT VALUES INTO
%token SELECT FROM STRING WHERE LIKE
%token ALTER ADD COLUMN
%token UPDATE SET
%token AND OR

%%

sql      : create sql | insert sql | sel sql | alt sql | upd sql
         |
         ;



create   : CREATE TABLE STRING '(' colvalspec ')' ';' 
{ gState->rs = trivsql_makers((char *) $3); trivsql_docreate((char *) $3, (char *) $5); } 
         ;

insert   : INSERT INTO STRING '(' colvalspec ')' VALUES '(' colvalspec ')' ';'
{ gState->rs = trivsql_makers((char *) $3); trivsql_checktable((char *) $3, &gState->rs); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doinsert((char *) $3, (char *) $5, (char *) $9);}}
         ;

sel      : SELECT cvsaster FROM STRING { gState->rs = trivsql_makers((char *) $4); trivsql_xfree(gState->table); gState->table = trivsql_xsnprintf("%s", $4); } wsel ';'
{ trivsql_checktable((char *) $4, &gState->rs); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doselect((char *) $4, (char *) $2);}}
         ;

alt      : ALTER STRING ADD COLUMN STRING ';'
{ gState->rs = trivsql_makers((char *) $2); trivsql_checktable((char *) $2, &gState->rs); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doalter((char *) $2, (char *) $5);}}
         ;

upd      : UPDATE STRING SET STRING '=' str { gState->rs = trivsql_makers((char *) $2); trivsql_xfree(gState->table); gState->table = trivsql_xsnprintf("%s", $2); } wsel ';'
{trivsql_checktable((char *) $2, &gState->rs); if(gState->rs->errno == TRIVSQL_FALSE){trivsql_doselect((char *) $2, (char *) $4); trivsql_updaters(gState, gState->rs, (char *) $4, (char *) $6);}}
         ;



cvsaster : colvalspec { $$ = trivsql_xsnprintf("%s", (char *) $1); }
         | '*' { $$ = trivsql_xsnprintf("*"); }
         ;

colvalspec : str ',' colvalspec { $$ = trivsql_xsnprintf("%s;%s", (char *) $1, (char *) $3); } 
         | str { $$ = trivsql_xsnprintf("%s", (char *) $1); }
         ;

wsel     : WHERE selector { gState->seltree = $2; }
         |
         ;

selector : str '=' str { $$ = trivsql_makesel(trivsql_selequal, $1, $3); }
         | str LIKE str { $$ = trivsql_makesel(trivsql_sellike, $1, $3); }
         | selector AND selector { $$ = trivsql_makeslr(trivsql_seland, $1, $3); }
         | selector OR selector { $$ = trivsql_makeslr(trivsql_seland, $1, $3); }
         | '(' selector ')' { $$ = $2; }
	 | { $$ = NULL }
         ;

str      : STRING { $$ = $1 } 
         | '\'' STRING '\'' { $$ = $2 }
         ; 

%%

int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}
