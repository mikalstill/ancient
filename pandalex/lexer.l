%{
#include "lexer.tab.h"

extern int binaryMode;

void debuglex(char *, char *, int);
char *returnStr(char *);

%}

%%

\%PDF-[0-9]+\.[0-9]+    { debuglex(yytext, "version", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? VERSION : ANYTHING);    }

[ \t\r\n]+              { debuglex(yytext, "whitespace", binaryMode);
                          if(binaryMode == 1){
                            yylval.textVal = (char *) returnStr(yytext);
                            return ANYTHING;
                          }                                                  }

\/[a-zA-Z\-_0-9\.]+     { debuglex(yytext, "name", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext + 1);
                          return((binaryMode == 0) ? NAME : ANYTHING);       }

                        /* --- stuff required for objects ---               */
R                       { debuglex(yytext, "object reference", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? OBJREF : ANYTHING);     }

obj                     { debuglex(yytext, "obj", binaryMode);
                          return((binaryMode == 0) ? OBJ : ANYTHING);        }

endobj                  { debuglex(yytext, "endobj", binaryMode);
                          return((binaryMode == 0) ? ENDOBJ : ANYTHING);     }

stream                  { debuglex(yytext, "stream", binaryMode);
                          return STREAM;                                     }

endstream               { debuglex(yytext, "endstream", 0);
                          binaryMode = 0;
                          return ENDSTREAM;                                  }

[+-]?[0-9]+             { debuglex(yytext, "integer", binaryMode);
                          yylval.intVal = atoi(yytext);
                          return((binaryMode == 0) ? INT : ANYTHING);        }

[+-]?[0-9]+\.[0-9]+     { debuglex(yytext, "floating point", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? FP : ANYTHING);        }

\<\<                    { debuglex(yytext, "<<", binaryMode);
                          return DBLLT;                                      }

\>\>                    { debuglex(yytext, ">>", binaryMode);
                          return DBLGT;                                      }

([a-zA-Z0-9\.:'+\-!_]+[a-zA-Z0-9\.:'+\-!_\\\/]*)+ { 
                          debuglex(yytext, "string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

\([a-zA-Z0-9\.\ :'+\-_]+\) {
                          debuglex(yytext, "bracketted string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

                        /* --- Array handling ---                           */

\[                      { debuglex(yytext, "[", binaryMode);
                          return((binaryMode == 0) ? ARRAY : ANYTHING);      }

\]                      { debuglex(yytext, "]", binaryMode);
                          return((binaryMode == 0) ? ENDARRAY : ANYTHING);   }

                        /* --- Stuff needed for the xref and trailer ---    */

\%\%EOF                 { debuglex(yytext, "end of file", binaryMode);
                          return((binaryMode == 0) ? PDFEOF : ANYTHING);     }

                        /* --- Stuff used to match binary streams --- 

                               The following amazing production is
                               used to deal with the massive
                               streams that images can create              */

([^a-zA-Z0-9+\-_\/\t\ \r\n()\\\[\]]+[a-df-zA-DF-Z0-9\[\]\/\\_\t\(\)+\-\[\]]*)+ { 
                          debuglex(yytext, "run of wierd characters",
                            binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return ANYTHING;
                                                                             }

.                       { debuglex(yytext, "the catch all", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext);
                          return ANYTHING;                                   }

%%

void debuglex(char *text, char *desc, int binMode){
#if defined DEBUG
  int i;

  printf("Binary mode is %s, lexer rule is \"%s\", match is \"",
	 (binaryMode == 1) ? "on" : "off", desc);  

  for(i = 0; i < strlen(text); i++){
    if(text[i] == '\n') printf(" \\n" );
    else if(text[i] == '\t') printf(" \\t ");
    else if(text[i] == '\r') printf(" \\r ");
    else if(text[i] == ' ') printf(" sp ");
    else if(isprint(text[i])) printf("%c", text[i]);
    else printf(" \\%d ", text[i]);
    }

  printf("\"\n");
#endif
}

char *returnStr(char *yytext){
  char *lval;

  #if defined DEBUG
     if(binaryMode == 0) printf("Calling returnStr on %s\n", yytext);
     printf("yytext was %d characters long\n", strlen(yytext));
  #endif

  if((lval = malloc(sizeof(char) * strlen(yytext) + 1)) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext) + 1);

  return lval;
}
