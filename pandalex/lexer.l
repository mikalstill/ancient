%{
#include "lexer.tab.h"
#include "lexinterface.h"

extern int binaryMode;

void debuglex(char *, int, char *, int);
char *returnStr(char *, int);

%}

%%

\%PDF-[0-9]+\.[0-9]+    { debuglex(yytext, -1, "version", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return((binaryMode == 0) ? VERSION : ANYTHING);    }

[ \t\r\n]+              { debuglex(yytext, -1, "whitespace", binaryMode);
                          if(binaryMode == 1){
                            yylval.textVal = (char *) returnStr(yytext, -1);
                            return ANYTHING;
                          }                                                  }

xref                    { debuglex(yytext, -1, "xref", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext + 1, -1);
                          return((binaryMode == 0) ? XREF : ANYTHING);       }

trailer                 { debuglex(yytext, -1, "trailer", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext + 1, -1);
                          return((binaryMode == 0) ? TRAILER : ANYTHING);    }

\/[a-zA-Z\-_0-9\.]+     { debuglex(yytext, -1, "name", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext + 1, -1);
                          return((binaryMode == 0) ? NAME : ANYTHING);       }

                        /* --- stuff required for objects ---               */
R                       { debuglex(yytext, -1, "object reference", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return((binaryMode == 0) ? OBJREF : ANYTHING);     }

obj                     { debuglex(yytext, -1, "obj", binaryMode);
                          return((binaryMode == 0) ? OBJ : ANYTHING);        }

endobj                  { debuglex(yytext, -1, "endobj", binaryMode);
                          return((binaryMode == 0) ? ENDOBJ : ANYTHING);     }

stream                  { debuglex(yytext, -1, "stream", binaryMode);
                          return STREAM;                                     }

endstream               { debuglex(yytext, -1, "endstream", 0);
                          binaryMode = 0;
                          return ENDSTREAM;                                  }

[+-]?[0-9]+             { debuglex(yytext, -1, "integer", binaryMode);
                          if(binaryMode == 0){
                            yylval.intVal = atoi(yytext);
                            return INT;
                            }
                          else{
                            yylval.textVal = yytext;
                            return ANYTHING;
                            }                                                }

[+-]?[0-9]+\.[0-9]+     { debuglex(yytext, -1, "floating point", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return((binaryMode == 0) ? FP : ANYTHING);         }

\<\<                    { debuglex(yytext, -1, "<<", binaryMode);
                          return DBLLT;                                      }

\>\>                    { debuglex(yytext, -1, ">>", binaryMode);
                          return DBLGT;                                      }

([a-zA-Z0-9\.:'+\-!_]+[a-zA-Z0-9\.:'+\-!_\\\/]*)+ { 
                          debuglex(yytext, -1, "string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

[\[\(][<>a-zA-Z0-9\.\ :'+\-_\\\(\)]+[\)\]] {
                          debuglex(yytext, -1, "bracketted string", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return((binaryMode == 0) ? STRING : ANYTHING);     }

                        /* --- Array handling ---                           */

\[                      { debuglex(yytext, -1, "[", binaryMode);
                          return((binaryMode == 0) ? ARRAY : ANYTHING);      }

\]                      { debuglex(yytext, -1, "]", binaryMode);
                          return((binaryMode == 0) ? ENDARRAY : ANYTHING);   }

                        /* --- Stuff needed for the xref and trailer ---    */

\%\%EOF                 { debuglex(yytext, -1, "end of file", binaryMode);
                          return((binaryMode == 0) ? PDFEOF : ANYTHING);     }

                        /* --- Stuff used to match binary streams --- 

                               The following amazing production is
                               used to deal with the massive
                               streams that images can create              */

([^a-zA-Z0-9+\-_\/\t\ \r\n()\\\[\]]+[a-df-zA-DF-Z0-9\[\]\/\\_\t\(\)+\-\[\]]*)+ { 
                          debuglex(yytext, yyleng, "run of wierd characters",
                            binaryMode);
                          yylval.sval.data = (char *) 
                            returnStr(yytext, yyleng);
                          yylval.sval.len = yyleng;

                          return ANYTHING;
                                                                             }

.                       { debuglex(yytext, -1, "the catch all", binaryMode);
                          yylval.textVal = (char *) returnStr(yytext, -1);
                          return ANYTHING;                                   }

%%

void debuglex(char *text, int len, char *desc, int binMode){
#if defined DEBUG
  int i;

  printf("Binary mode is %s, lexer rule is \"%s\", match is \"",
	 (binaryMode == 1) ? "on" : "off", desc);  

  for(i = 0; i < ((len == -1) ? strlen(text) : len); i++){
    if(text[i] == '\n') printf(" \\n " );
    else if(text[i] == '\t') printf(" \\t ");
    else if(text[i] == '\r') printf(" \\r ");
    else if(text[i] == ' ') printf(" sp ");
    else if(isprint(text[i])) printf("%c", text[i]);
    else printf(" \\%d ", text[i]);
    }

  printf("\"\n");
#endif
}

char *returnStr(char *yytext, int len){
  char *lval;

  #if defined DEBUG
     if(binaryMode == 0) printf("Calling returnStr on %s\n", yytext);
     printf("yytext was %d characters long\n", strlen(yytext));
  #endif

  if((lval = malloc(sizeof(char) * 
    ((len == -1) ? strlen(yytext) : len) + 1)) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext) + 1);

  return lval;
}
