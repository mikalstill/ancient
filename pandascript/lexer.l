%{
#include "lexer.tab.h"
#include <panda/constants.h>
#include <panda/functions.h>

char *returnStr(char *);

%}

%%

[ \t\r\n]+

\".*\"                  { // This is a memory leak...
                          yylval.textVal = (char *) returnStr(yytext);
                          yylval.textVal = yylval.textVal + 1;
                          yylval.textVal[strlen(yylval.textVal) - 1] = '\0';
                          return QUOTESTR;       }

imagebox                { return IMAGEBOX;       }
imageboxrot             { return IMAGEBOXROT;    }
textbox                 { return TEXTBOX;        }

output                  { return OPENDOC;        }
newpage                 { return NEWPAGE;        }
close                   { return CLOSE;          }

setfontmode             { return SETFONTMODE;    }
setcharacterspacing     { return SETCHARSPACING; }
setwordspacing          { return SETWORDSPACING; }
sethorizontalscaling    { return SETHORIZSCALING;}
setleading              { return SETLEADING;     }
setfont                 { return SETFONT;        }

[0-9]+\.[0-9]+          { yylval.fltVal = atof(yytext);
                          return FLOAT;          }

[0-9]+                  { yylval.intVal = atoi(yytext);
                          return INT;            }

TIFFIMG                 { yylval.intVal = gImageTiff;
                          return IMGTYPE;        }

JPEGIMG                 { yylval.intVal = gImageJpeg;
                          return IMGTYPE;        }

%%

char *returnStr(char *yytext){
  char *lval;

  if((lval = malloc(sizeof(char) * strlen(yytext) + 1)) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext) + 1);

  return lval;
}
