%{
#include "vcard.tab.h"
extern int yylval;
%}

%%

[Bb][Ee][Gg][Ii][Nn]      { printf("*begin* "); return BEGIN; }
[Vv][Cc][Aa][Rr][Dd]      { printf("*vcard* "); return VCARD; }
[Ee][Nn][Dd]              { printf("*end* "); return END; }

[A-Za-z]+                 { yylval = yytext; printf("*name* "); return NAME; }
[A-Za-z;\.=]+             { yylval = yytext; printf("*string* "); return STRING; }
[0-9\.]+                  { yylval = yytext; printf("*number* "); return NUMBER; }

[ \t\r\n]                 { printf("*whitespace* "); }
.                         { printf("*anything* "); return yytext[0]; }

%%
