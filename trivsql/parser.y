%{
  #include <stdio.h>
  #include "trivsql.h"

  #define YYERROR_VERBOSE 1

  #undef YY_INPUT
  #define YY_INPUT(b, r, ms) (r = trivsql_gettext(b, ms);)

  trivsql_state *gState;
%}

%token CREATE TABLE 
%token INSERT VALUES INTO
%token SELECT FROM STRING WHERE LIKE

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

sel      : SELECT cvsaster FROM STRING selector ';'
{ trivsql_recordset *rs; trivsql_displayrs(rs = trivsql_doselect($4, $2), $4, $2); /*trivsql_xfree(rs);*/ }
         ;

cvsaster : colvalspec { $$ = trivsql_xsnprintf("%s", $1); }
         | '*' { $$ = trivsql_xsnprintf("*"); }
         ;

colvalspec : str ',' colvalspec { $$ = trivsql_xsnprintf("%s;%s", $1, $3); } 
         | str { $$ = trivsql_xsnprintf("%s", $1); }
         ;

selector : WHERE str '=' str { gState->selector = trivsql_selequal; gState->selArgOne = $2; gState->selArgTwo = $4 }
         | WHERE str LIKE str { gState->selector = trivsql_sellike; gState->selArgOne = $2; gState->selArgTwo = $4 }
	 | { gState->selector = NULL }
         ;

str      : STRING { $$ = $1 } 
         | '\'' STRING '\'' { $$ = $2 }
         ; 

%%

int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}
