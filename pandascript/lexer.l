%{
#include "parser.h"
#include <panda/constants.h>
#include <panda/functions.h>

char *returnStr(char *);

%}

%%

[ \t\r\n]+

\"[^\"]*\"              { // This is a memory leak...
                          yylval.textVal = (char *) returnStr(yytext);
                          yylval.textVal = yylval.textVal + 1;
                          yylval.textVal[strlen(yylval.textVal) - 1] = '\0';
                          return QUOTESTR;       }


panda_open                     { return PANDA_OPEN                    ; }
panda_textannotation           { return PANDA_TEXTANNOTATION          ; }
panda_freetextannotation       { return PANDA_FREETEXTANNOTATION      ; }
panda_lineannotation           { return PANDA_LINEANNOTATION          ; }
panda_insertannotation         { return PANDA_INSERTANNOTATION        ; }
panda_setlinestart             { return PANDA_SETLINESTART            ; }
panda_addlinesegment           { return PANDA_ADDLINESEGMENT          ; }
panda_addcubiccurvesegment     { return PANDA_ADDCUBICCURVESEGMENT    ; }
panda_addquadraticcurvesegmentone { return PANDA_ADDQUADRATICCURVESEGMENTONE; }
panda_addquadraticcurvesegmenttwo { return PANDA_ADDQUADRATICCURVESEGMENTTWO; }
panda_closeline                { return PANDA_CLOSELINE               ; }
panda_rectangle                { return PANDA_RECTANGLE               ; }
panda_endline                  { return PANDA_ENDLINE                 ; }
panda_strokeline               { return PANDA_STROKELINE              ; }
panda_fillline                 { return PANDA_FILLLINE                ; }
panda_setlinewidth             { return PANDA_SETLINEWIDTH            ; }
panda_setlinecap               { return PANDA_SETLINECAP              ; }
panda_setlinejoin              { return PANDA_SETLINEJOIN             ; }
panda_setlinedash              { return PANDA_SETLINEDASH             ; }
panda_setfillcolor             { return PANDA_SETFILLCOLOR            ; }
panda_setlinecolor             { return PANDA_SETLINECOLOR            ; }
panda_imagebox                 { return PANDA_IMAGEBOX                ; }
panda_imageboxrot              { return PANDA_IMAGEBOXROT             ; }
panda_textbox                  { return PANDA_TEXTBOX                 ; }
panda_textboxrot               { return PANDA_TEXTBOXROT              ; }
panda_textboxrotalign          { return PANDA_TEXTBOXROTALIGN         ; }
panda_pageduration             { return PANDA_PAGEDURATION            ; }
panda_transduration            { return PANDA_TRANSDURATION           ; }
panda_transstyle               { return PANDA_TRANSSTYLE              ; }
panda_setauthor                { return PANDA_SETAUTHOR               ; }
panda_setcreator               { return PANDA_SETCREATOR              ; }
panda_settitle                 { return PANDA_SETTITLE                ; }
panda_setsubject               { return PANDA_SETSUBJECT              ; }
panda_setkeywords              { return PANDA_SETKEYWORDS             ; }
panda_setfontsize              { return PANDA_SETFONTSIZE             ; }
panda_setfontmode              { return PANDA_SETFONTMODE             ; }
panda_setcharacterspacing      { return PANDA_SETCHARACTERSPACING     ; }
panda_setwordspacing           { return PANDA_SETWORDSPACING          ; }
panda_sethorizontalscaling     { return PANDA_SETHORIZONTALSCALING    ; }
panda_setleading               { return PANDA_SETLEADING              ; }
panda_close                    { return PANDA_CLOSE                   ; }
panda_newpage                  { return PANDA_NEWPAGE                 ; }
panda_newtemplate              { return PANDA_NEWTEMPLATE             ; }
panda_hidetoolbar              { return PANDA_HIDETOOLBAR             ; }
panda_hidemenubar              { return PANDA_HIDEMENUBAR             ; }
panda_hidewindowui             { return PANDA_HIDEWINDOWUI            ; }
panda_fitwindow                { return PANDA_FITWINDOW               ; }
panda_centerwindow             { return PANDA_CENTERWINDOW            ; }
panda_nfspagemode              { return PANDA_NFSPAGEMODE             ; }
panda_fullscreen               { return PANDA_FULLSCREEN              ; }
panda_textdirection            { return PANDA_TEXTDIRECTION           ; }

[0-9]+\.[0-9]+          { yylval.fltVal = atof(yytext);
                          return FLOAT;          }

[0-9]+                  { yylval.intVal = atoi(yytext);
                          return INTEGER;            }

%%

char *returnStr(char *yytext){
  char *lval;

  if((lval = malloc(sizeof(char) * strlen(yytext) + 1)) == NULL)
    error("Could not make space for lexer return.");
  strncpy(lval, yytext, strlen(yytext) + 1);

  return lval;
}
