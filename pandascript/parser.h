/* A Bison parser, made by GNU Bison 1.875a.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PANDA_TEXTANNOTATION = 258,
     PANDA_FREETEXTANNOTATION = 259,
     PANDA_LINEANNOTATION = 260,
     PANDA_INSERTANNOTATION = 261,
     PANDA_SETLINESTART = 262,
     PANDA_ADDLINESEGMENT = 263,
     PANDA_ADDCUBICCURVESEGMENT = 264,
     PANDA_ADDQUADRATICCURVESEGMENTONE = 265,
     PANDA_ADDQUADRATICCURVESEGMENTTWO = 266,
     PANDA_CLOSELINE = 267,
     PANDA_RECTANGLE = 268,
     PANDA_ENDLINE = 269,
     PANDA_STROKELINE = 270,
     PANDA_FILLLINE = 271,
     PANDA_SETLINEWIDTH = 272,
     PANDA_SETLINECAP = 273,
     PANDA_SETLINEJOIN = 274,
     PANDA_SETLINEDASH = 275,
     PANDA_SETFILLCOLOR = 276,
     PANDA_SETLINECOLOR = 277,
     PANDA_IMAGEBOX = 278,
     PANDA_IMAGEBOXROT = 279,
     PANDA_TEXTBOX = 280,
     PANDA_TEXTBOXROT = 281,
     PANDA_TEXTBOXROTALIGN = 282,
     PANDA_PAGEDURATION = 283,
     PANDA_TRANSDURATION = 284,
     PANDA_TRANSSTYLE = 285,
     PANDA_SETAUTHOR = 286,
     PANDA_SETCREATOR = 287,
     PANDA_SETTITLE = 288,
     PANDA_SETSUBJECT = 289,
     PANDA_SETKEYWORDS = 290,
     PANDA_CREATEFONT = 291,
     PANDA_SETFONT = 292,
     PANDA_SETFONTSIZE = 293,
     PANDA_SETFONTMODE = 294,
     PANDA_SETCHARACTERSPACING = 295,
     PANDA_SETWORDSPACING = 296,
     PANDA_SETHORIZONTALSCALING = 297,
     PANDA_SETLEADING = 298,
     PANDA_GETFONTMETRIC = 299,
     PANDA_CLOSE = 300,
     PANDA_NEWPAGE = 301,
     PANDA_NEWTEMPLATE = 302,
     PANDA_HIDETOOLBAR = 303,
     PANDA_HIDEMENUBAR = 304,
     PANDA_HIDEWINDOWUI = 305,
     PANDA_FITWINDOW = 306,
     PANDA_CENTERWINDOW = 307,
     PANDA_NFSPAGEMODE = 308,
     PANDA_FULLSCREEN = 309,
     PANDA_TEXTDIRECTION = 310,
     FLOAT = 311,
     QUOTESTR = 312,
     ANYTHING = 313,
     IMGTYPE = 314,
     INTEGER = 315
   };
#endif
#define PANDA_TEXTANNOTATION 258
#define PANDA_FREETEXTANNOTATION 259
#define PANDA_LINEANNOTATION 260
#define PANDA_INSERTANNOTATION 261
#define PANDA_SETLINESTART 262
#define PANDA_ADDLINESEGMENT 263
#define PANDA_ADDCUBICCURVESEGMENT 264
#define PANDA_ADDQUADRATICCURVESEGMENTONE 265
#define PANDA_ADDQUADRATICCURVESEGMENTTWO 266
#define PANDA_CLOSELINE 267
#define PANDA_RECTANGLE 268
#define PANDA_ENDLINE 269
#define PANDA_STROKELINE 270
#define PANDA_FILLLINE 271
#define PANDA_SETLINEWIDTH 272
#define PANDA_SETLINECAP 273
#define PANDA_SETLINEJOIN 274
#define PANDA_SETLINEDASH 275
#define PANDA_SETFILLCOLOR 276
#define PANDA_SETLINECOLOR 277
#define PANDA_IMAGEBOX 278
#define PANDA_IMAGEBOXROT 279
#define PANDA_TEXTBOX 280
#define PANDA_TEXTBOXROT 281
#define PANDA_TEXTBOXROTALIGN 282
#define PANDA_PAGEDURATION 283
#define PANDA_TRANSDURATION 284
#define PANDA_TRANSSTYLE 285
#define PANDA_SETAUTHOR 286
#define PANDA_SETCREATOR 287
#define PANDA_SETTITLE 288
#define PANDA_SETSUBJECT 289
#define PANDA_SETKEYWORDS 290
#define PANDA_CREATEFONT 291
#define PANDA_SETFONT 292
#define PANDA_SETFONTSIZE 293
#define PANDA_SETFONTMODE 294
#define PANDA_SETCHARACTERSPACING 295
#define PANDA_SETWORDSPACING 296
#define PANDA_SETHORIZONTALSCALING 297
#define PANDA_SETLEADING 298
#define PANDA_GETFONTMETRIC 299
#define PANDA_CLOSE 300
#define PANDA_NEWPAGE 301
#define PANDA_NEWTEMPLATE 302
#define PANDA_HIDETOOLBAR 303
#define PANDA_HIDEMENUBAR 304
#define PANDA_HIDEWINDOWUI 305
#define PANDA_FITWINDOW 306
#define PANDA_CENTERWINDOW 307
#define PANDA_NFSPAGEMODE 308
#define PANDA_FULLSCREEN 309
#define PANDA_TEXTDIRECTION 310
#define FLOAT 311
#define QUOTESTR 312
#define ANYTHING 313
#define IMGTYPE 314
#define INTEGER 315




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 19 "parser.y"
typedef union YYSTYPE {
  int      intVal;
  char     *textVal;
  double   fltVal;
} YYSTYPE;
/* Line 1240 of yacc.c.  */
#line 163 "y.tab.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



