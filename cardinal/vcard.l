%{
#include "vcard.tab.h"
extern int yylval;
%}

%%

\r?\n[ \t]                        { printf("*fold* "); return LINEFOLD; }

[Bb][Ee][Gg][Ii][Nn]              { printf("*begin* "); return BEGIN; }
[Vv][Cc][Aa][Rr][Dd]              { printf("*vcard* "); return VCARD; }
[Ee][Nn][Dd]                      { printf("*end* "); return END; }

[Ff][Nn]                          { printf("*fn* "); return FN; }
[Nn]                              { printf("*n* "); return N; }
[Nn][Ii][Cc][Kk][Nn][Aa][Mm][Ee]  { printf("*nickname* "); return NICKNAME; }
[Pp][Hh][Oo][Tt][Oo]              { printf("*photo* "); return PHOTO; }
[Bb][Dd][Aa][Yy]                  { printf("*bday* "); return BDAY; }

[a-zA-Z0-9 ;\.]+                      { printf("*string = %s* ", yytext); return STRING; }

[ \t\r\n]                         { printf("*whitespace* "); }
.                                 { printf("*anything = %s* ", yytext); return yytext[0]; }

%%
