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

%%

[Ss][Ee][Ll][Ee][Cc][Tt]                   { return SELECT; }
[Cc][Rr][Ee][Aa][Tt][Ee]                   { return CREATE; }
[Ii][Nn][Ss][Ee][Rr][Tt]                   { return INSERT; }
[Vv][Aa][Ll][Uu][Ee][Ss]                   { return VALUES; }
[Ii][Nn][Tt][Oo]                           { return INTO; }
[Tt][Aa][Bb][Ll][Ee]                       { return TABLE; }
[Ff][Rr][Oo][Mm]                           { return FROM; }
[Ww][Hh][Ee][Rr][Ee]                       { return WHERE; }
[Ll][Ii][Kk][Ee]                           { return LIKE; }
[Aa][Ll][Tt][Ee][Rr]                       { return ALTER; }
[Aa][Dd][Dd]                               { return ADD; }
[Cc][Oo][Ll][Uu][Mm][Nn]                   { return COLUMN; }
[Uu][Pp][Dd][Aa][Tt][Ee]                   { return UPDATE; }
[Ss][Ee][Tt]                               { return SET; }
[Aa][Nn][Dd]                               { return AND; }
[Oo][Rr]                                   { return OR; }

[a-zA-Z0-9%\-]+                            { yylval = trivsql_xsnprintf("%s", yytext); return STRING; }
[ \t\r\n]                                  { }
.                                          { return yytext[0]; }

%%
