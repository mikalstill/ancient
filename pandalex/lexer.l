%{
#include "parser.h"
#include "pandalex.h"
#include <unistd.h>
#include <stdlib.h>

#undef YY_INPUT
#define YY_INPUT(b, r, ms) (r = pandalex_gettext(b, ms))

void *
pandalex_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      pandalex_error ("pandalex_xmalloc failed to allocate memory");
    }

  return buffer;
}
void *
pandalex_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      pandalex_error ("pandalex_xrealloc failed to allocate memory");
    }

  return buffer;
}

char *
pandalex_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = pandalex_xrealloc (output, size);
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
        {
          /* Up to glibc 2.0.6 and Microsoft's implementation*/
          size += 100;
        }
      else
        {
          /* Check if we are done */
      if (result < size)
            break;

          /* Glibc from now on */
          size = result + 1;
        }
  }

  va_end (ap);
  return output;
}

void
pandalex_error(char *msg){
fprintf(stderr, "%s\n", msg);
exit(42);
}
%}

%x BINARY
%%

<INITIAL>\%PDF-[0-9]+\.[0-9]+    { debuglex(yytext, yyleng, "version");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return PDFVER;
                                                                             }

<INITIAL>[ \t\r\n]+              { debuglex(yytext, yyleng, "whitespace");
                                                                          }

<INITIAL>xref                    { debuglex(yytext, yyleng, "xref");
                            yylval.sval.data = (char *) returnStr(yytext + 1, yyleng);
                            yylval.sval.len = yyleng - 1;
                            return XREF;
                                                                             }

<INITIAL>trailer                 { debuglex(yytext, yyleng, "trailer");
                            yylval.sval.data = (char *) returnStr(yytext + 1, yyleng);
                            yylval.sval.len = yyleng - 1;
                            return TRAILER;
                                                                             }

<INITIAL>\/[@:=#a-zA-Z\-_0-9\.\+,]+ { debuglex(yytext, yyleng, "name");
                            yylval.sval.data = (char *) returnStr(yytext + 1, yyleng);
                            yylval.sval.len = yyleng - 1;
                            return NAME;
                                                                             }

<INITIAL>\<\<\/[#a-zA-Z\-_0-9\.\+]+ { debuglex(yytext, yyleng, "dbllt-name");
                            yyless(2);
                            return DBLLT;
                                                                             }

<INITIAL>\/[#a-zA-Z\-_0-9\.\+]+\>\>     { debuglex(yytext, yyleng, "name-dblgt");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext + 1, yyleng);
                            yylval.sval.len = yyleng - 1;
                            return NAME;
}

                        /* --- stuff required for objects ---               */
<INITIAL>R                       { debuglex(yytext, yyleng, "object reference");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return OBJREF; 
                                                                             }

<INITIAL>R\>\>                       { debuglex(yytext, yyleng, "object-reference-dblgt");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return OBJREF; 
                                                                             }

<INITIAL>R\/                       { debuglex(yytext, yyleng, "object-reference-namestart");
                            yyless(yyleng - 1);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return OBJREF; 
                                                                             }

<INITIAL>obj                     { debuglex(yytext, yyleng, "obj");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return OBJ; 
                                                                             }

<INITIAL>endobj                  { debuglex(yytext, yyleng, "endobj");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return ENDOBJ; 
                                                                             }

<INITIAL>stream                  { debuglex(yytext, yyleng, "stream");
                          BEGIN(BINARY);
                          return STREAM;                                     }

<INITIAL>[+-]?[0-9]+             { debuglex(yytext, yyleng, "integer");
                            yylval.intVal = atoi(yytext);
                            return INT; 
                                                                             }

<INITIAL>[+-]?[0-9]+\>\>             { debuglex(yytext, yyleng, "integer-dblgt");
                            yyless(yyleng - 2);
                            yylval.intVal = atoi(yytext);
                            return INT; 
                                                                             }


<INITIAL>[+-]?[0-9]+\.[0-9]+     { debuglex(yytext, yyleng, "floating point");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return FP; 
                                                                             }

<INITIAL>[+-]?[0-9]+\.[0-9]+\>\>     { debuglex(yytext, yyleng, "floating point");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return FP; 
                                                                             }

<INITIAL>\<\<                    { debuglex(yytext, yyleng, "<<");
                            return DBLLT; 
                                                                             }

<INITIAL>\>\>                    { debuglex(yytext, yyleng, ">>");
                            return DBLGT; 
                                                                             }

<INITIAL>\>\>\>\>                    { debuglex(yytext, yyleng, ">>>>");
                            yyless(yyleng - 2);
                            return DBLGT; 
                                                                             }


<INITIAL,BINARY>([#a-zA-Z0-9\.:'+\-!_]+[a-zA-Z0-9\.:'+\-!_\\]*)+ { 
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;

                            {
                              int i;
                              for(i = 0; i < yyleng - 8; i++){
                                if(strcmp(yytext + i, "endstream") == 0){
                                  debuglex(yytext, yyleng, "string (at stream end)");
                                  BEGIN(INITIAL);
                                  return ENDSTREAM;
                                  }
                                }
                              }
                            debuglex(yytext, yyleng, "string");

                            return STRING; 
                                                                             }

<INITIAL,BINARY>\[(([^\[\]])|(\\\[)|([^\\]\\\]))*\] {
                          debuglex(yytext, yyleng, "bracketted string v1");
                            yylval.sval.data = (char *) returnStr(yytext, 
								  yyleng); 
                            yylval.sval.len = yyleng;
                            return STRING; 
                                                                             }

<INITIAL,BINARY>\((([^\(\)])|(\\\()|([^\\]\\\)))*\) {
                          debuglex(yytext, yyleng, "bracketted string v2");
                            yylval.sval.data = (char *) returnStr(yytext, 
								  yyleng);
                            yylval.sval.len = yyleng; 
                            return STRING; 
                                                                             }

<INITIAL,BINARY>\<(([^\<\>])|(\\\<)|([^\\]\\\>))*\> {
                          debuglex(yytext, yyleng, "bracketted string v3");
                            yylval.sval.data = (char *) returnStr(yytext, 
								  yyleng);
                            yylval.sval.len = yyleng; 
                            return STRING; 
                                                                             }

<INITIAL,BINARY>([#a-zA-Z0-9\.:'+\-!_]+[a-zA-Z0-9\.:'+\-!_\\]*)+\>\> { 
                          debuglex(yytext, yyleng, "string-dblgt");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return STRING; 
                                                                             }

<INITIAL,BINARY>[\[\(][#<>a-zA-Z0-9\.\ :'+\-_\\\(\)]+[\)\]]\>\> {
                          debuglex(yytext, yyleng, "bracketted-string-dblgt");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return STRING; 
                                                                             }

                        /* --- Array handling ---                           */

<INITIAL>\[                      { debuglex(yytext, yyleng, "[");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return ARRAY; 
                                                                             }

<INITIAL>\]                      { debuglex(yytext, yyleng, "]");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return ENDARRAY; 
                                                                             }

<INITIAL>\]\>\>                      { debuglex(yytext, yyleng, "]");
                            yyless(yyleng - 2);
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng; 
                            return ENDARRAY; 
                                                                             }

                        /* --- Stuff needed for the xref and trailer ---    */

<INITIAL>\%\%EOF                 { debuglex(yytext, yyleng, "end of file");
                            return PDFEOF; 
                                                                             }

                        /* --- Stuff used to match binary streams --- 

                               The following amazing production is
                               used to deal with the massive
                               streams that images can create              */

<BINARY>[^end]+           { debuglex(yytext, yyleng, "binary mode");
                          yylval.sval.data = (char *) returnStr(yytext, yyleng);
                          yylval.sval.len = yyleng;
                          return ANYTHING;
                                                                             }

.                       { debuglex("!", yyleng, "the catch all");
                            yylval.sval.data = (char *) returnStr(yytext, yyleng);
                            yylval.sval.len = yyleng;
                            return ANYTHING; 
                                                                             }

%%

// TODO mikal: This -1 syntax can go away now
void debuglex(char *text, int len, char *desc){
#if defined DEBUG
  int i;

  printf("Lexer rule is \"%s\", match is \"", desc);  

  for(i = 0; i < ((len == -1) ? strlen(text) : len); i++){
    if(text[i] == '\n') printf(" \\n " );
    else if(text[i] == '\t') printf(" \\t ");
    else if(text[i] == '\r') printf(" \\r ");
    else if(text[i] == ' ') printf(" sp ");
    else if(isprint(text[i])) printf("%c", text[i]);
    else printf(" \\%d ", (unsigned char) text[i]);
    }

  printf("\"\n");
#endif
}

// TODO mikal: This -1 syntax can go away now
char *returnStr(char *yytext, int len){
  char *lval;

  if((lval = malloc(sizeof(char) * 
    ((len == -1) ? strlen(yytext) : len) + 1)) == NULL)
    pandalex_error("Could not make space for lexer return.");
  memcpy(lval, yytext, ((len == -1) ? strlen(yytext) : len) + 1);

  return lval;
}

void beginBinary(){
  BEGIN(BINARY);
}

void endBinary(){
  BEGIN(INITIAL);
}