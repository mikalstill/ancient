%{
/******************************************************************************
  Copyright (C) Michael Still 2002
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************/

#include "parser.h"

#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = trivsql_gettext(b, ms))
%} 

%x STRINGRULES
%%

<INITIAL>[Ss][Ee][Ll][Ee][Cc][Tt]                   { trivsql_debugtoken("SELECT", yytext); return SELECT; }
<INITIAL>[Cc][Rr][Ee][Aa][Tt][Ee]                   { trivsql_debugtoken("CREATE", yytext); return CREATE; }
<INITIAL>[Ii][Nn][Ss][Ee][Rr][Tt]                   { trivsql_debugtoken("INSERT", yytext); return INSERT; }
<INITIAL>[Vv][Aa][Ll][Uu][Ee][Ss]                   { trivsql_debugtoken("VALUES", yytext); return VALUES; }
<INITIAL>[Ii][Nn][Tt][Oo]                           { trivsql_debugtoken("INTO", yytext); return INTO; }
<INITIAL>[Tt][Aa][Bb][Ll][Ee]                       { trivsql_debugtoken("TABLE", yytext); return TABLE; }
<INITIAL>[Ff][Rr][Oo][Mm]                           { trivsql_debugtoken("FROM", yytext); return FROM; }
<INITIAL>[Ww][Hh][Ee][Rr][Ee]                       { trivsql_debugtoken("WHERE", yytext); return WHERE; }
<INITIAL>[Ll][Ii][Kk][Ee]                           { trivsql_debugtoken("LIKE", yytext); return LIKE; }
<INITIAL>[Aa][Ll][Tt][Ee][Rr]                       { trivsql_debugtoken("ALTER", yytext); return ALTER; }
<INITIAL>[Aa][Dd][Dd]                               { trivsql_debugtoken("ADD", yytext); return ADD; }
<INITIAL>[Cc][Oo][Ll][Uu][Mm][Nn]                   { trivsql_debugtoken("COLUMN", yytext); return COLUMN; }
<INITIAL>[Uu][Pp][Dd][Aa][Tt][Ee]                   { trivsql_debugtoken("UPDATE", yytext); return UPDATE; }
<INITIAL>[Ss][Ee][Tt]                               { trivsql_debugtoken("SET", yytext); return SET; }
<INITIAL>[Aa][Nn][Dd]                               { trivsql_debugtoken("AND", yytext); return AND; }
<INITIAL>[Oo][Rr]                                   { trivsql_debugtoken("OR", yytext); return OR; }

<INITIAL>[ \t\r\n]                                  { trivsql_debugtoken("WHITESPACE", yytext); }
<INITIAL>[^ \t\r\n,\(\)\';\*]+                      { trivsql_debugtoken("SMALLSTRING", yytext); 
                                                      yylval = trivsql_xsnprintf("%s", yytext); return STRING; }
<INITIAL>'                                          { trivsql_debugtoken("BEGINSTRING", yytext); BEGIN(STRINGRULES); }
<INITIAL>.                                          { trivsql_debugtoken("CHAR", yytext); return yytext[0]; }

<STRINGRULES>[^']+                                  { trivsql_debugtoken("STRING", yytext); 
                                                      yylval = trivsql_xsnprintf("%s", yytext); return STRING; }
<STRINGRULES>'                                      { trivsql_debugtoken("ENDSTRING", yytext); BEGIN(INITIAL); }

%%
