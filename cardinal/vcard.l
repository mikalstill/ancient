%{
#include "vcard.tab.h"
extern int yylval;
extern int parsemode;
%}

%%

\r?\n[ \t]                                 { printf("*%d, fold* ", parsemode); return LINEFOLD; }

[Bb][Ee][Gg][Ii][Nn]                       { printf("*%d, begin* ", parsemode); return(parsemode == 1 ? BEGIN : STRING); }
[Vv][Cc][Aa][Rr][Dd]                       { printf("*%d, vcard* ", parsemode); return(parsemode == 1 ? VCARD : STRING); }
[Ee][Nn][Dd]                               { printf("*%d, end* ", parsemode); return(parsemode == 1 ? END : STRING); }

[Ee][Nn][Cc][Oo][Dd][Ii][Nn][Gg]           { printf("*%d, encoding* ", parsemode); return(parsemode == 1 ? ENCODING : STRING); }
  
[Ff][Nn]                                   { printf("*%d, fn* ", parsemode); return(parsemode == 1 ? FN : STRING); }
[Nn]                                       { printf("*%d, n* ", parsemode); return(parsemode == 1 ? N : STRING); }
[Nn][Ii][Cc][Kk][Nn][Aa][Mm][Ee]           { printf("*%d, nickname* ", parsemode); return(parsemode == 1 ? NICKNAME : STRING); }
[Pp][Hh][Oo][Tt][Oo]                       { printf("*%d, photo* ", parsemode); return(parsemode == 1 ? PHOTO : STRING); }
[Bb][Dd][Aa][Yy]                           { printf("*%d, bday* ", parsemode); return(parsemode == 1 ? BDAY : STRING); }
 
[Aa][Dd][Rr]                               { printf("*%d, adr* ", parsemode); return(parsemode == 1 ? ADR : STRING); }
[LL][Aa][Bb][Ee][Ll]                       { printf("*%d, label* ", parsemode); return(parsemode == 1 ? LABEL : STRING); }

[Vv][Ee][Rr][Ss][Ii][Oo][Nn]               { printf("*%d, version* ", parsemode); return(parsemode == 1 ? VERSION : STRING); }
[Uu][Rr][Ll]                               { printf("*%d, url* ", parsemode); return(parsemode == 1 ? URL : STRING); }

[Tt][Ee][Ll]                               { printf("*%d, tel* ", parsemode); return(parsemode == 1 ? TEL : STRING); }
[Ee][Mm][Aa][Ii][Ll]                       { printf("*%d, email* ", parsemode); return(parsemode == 1 ? EMAIL : STRING); }
[Mm][Aa][Ii][Ll][Ee][Rr]                   { printf("*%d, mailer* ", parsemode); return(parsemode == 1 ? MAILER : STRING); }

[Tt][Zz]                                   { printf("*%d, tz* ", parsemode); return(parsemode == 1 ? TZ : STRING); }
[Gg][Ee][Oo]                               { printf("*%d, geo* ", parsemode); return(parsemode == 1 ? GEO : STRING); }

[Tt][Ii][Tt][Ll][Ee]                       { printf("*%d, title* ", parsemode); return(parsemode == 1 ? TITLE : STRING); }
[Rr][Oo][Ll][Ee]                           { printf("*%d, role* ", parsemode); return(parsemode == 1 ? ROLE : STRING); }
[Ll][Oo][Gg][Oo]                           { printf("*%d, logo* ", parsemode); return(parsemode == 1 ? LOGO : STRING); }
[Aa][Gg][Ee][Nn][Tt]                       { printf("*%d, agent* ", parsemode); return(parsemode == 1 ? AGENT : STRING); }
[Oo][Rr][Gg]                               { printf("*%d, org* ", parsemode); return(parsemode == 1 ? ORG : STRING); }

[Cc][Aa][Tt][Ee][Gg][Oo][Rr][Ii][Ee][Ss]   { printf("*%d, categories* ", parsemode); return(parsemode == 1 ? CATEGORIES : STRING); }
[Nn][Oo][Tt][Ee]                           { printf("*%d, note* ", parsemode); return(parsemode == 1 ? NOTE : STRING); }
[Pp][Rr][Oo][Dd][Ii][Dd]                   { printf("*%d, prodid* ", parsemode); return(parsemode == 1 ? PRODID : STRING); }
[Rr][Ee][Vv]                               { printf("*%d, rev* ", parsemode); return(parsemode == 1 ? REV : STRING); }
[Ss][Oo][Rr][Tt]-[Ss][Tt][Rr][Ii][Nn][Gg]  { printf("*%d, sort-string* ", parsemode); return(parsemode == 1 ? SORTSTRING : STRING); }
[Ss][Oo][Uu][Nn][Dd]                       { printf("*%d, sound* ", parsemode); return(parsemode == 1 ? SOUND : STRING); }
[Uu][Ii][Dd]                               { printf("*%d, uid* ", parsemode); return(parsemode == 1 ? UID : STRING); }

[Cc][Ll][Aa][Ss][Ss]                       { printf("*%d, class* ", parsemode); return(parsemode == 1 ? CLASS : STRING); }
[Kk][Ee][Yy]                               { printf("*%d, key* ", parsemode); return(parsemode == 1 ? KEY : STRING); }

[a-zA-Z0-9 ;\.=\-/\/@]+                    { printf("*%d, string = %s* ", parsemode, yytext); return STRING; }

[ \t\r\n]                                  { printf("*%d, whitespace* ", parsemode); }
.                                          { printf("*anything = %c* ", yytext[0]); return yytext[0]; }

%%
