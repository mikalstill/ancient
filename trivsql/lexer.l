%{
#include "parser.h"
%} 

%%

[Ss][Ee][Ll][Ee][Cc][Tt]                   { return SELECT; }
[Cc][Rr][Ee][Aa][Tt][Ee]                   { return CREATE; }
[Ii][Nn][Ss][Ee][Rr][Tt]                   { return INSERT; }
[Vv][Aa][Ll][Uu][Ee][Ss]                   { return VALUES; }
[Ii][Nn][Tt][Oo]                           { return INTO; }
[Tt][Aa][Bb][Ll][Ee]                       { return TABLE; }
[Ff][Rr][Oo][Mm]                           { return FROM; }

[a-zA-Z0-9]+                               { yylval = trivsql_xsnprintf("%s", yytext); return STRING; }
[ \t\r\n]                                  { }
.                                          { return yytext[0]; }

%%
