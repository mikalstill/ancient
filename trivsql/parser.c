
/*  A Bison parser, made from parser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	CREATE	257
#define	TABLE	258
#define	INSERT	259
#define	VALUES	260
#define	INTO	261
#define	SELECT	262
#define	FROM	263
#define	STRING	264
#define	WHERE	265
#define	LIKE	266

#line 1 "parser.y"

  #include <stdio.h>
  #include <stdarg.h>
  #include "trivsql.h"

  #define YYERROR_VERBOSE 1

  trivsql_state *gState;
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		46
#define	YYFLAG		-32768
#define	YYNTBASE	19

#define YYTRANSLATE(x) ((unsigned)(x) <= 266 ? yytranslate[x] : 26)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    18,    13,
    14,     2,     2,    16,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    15,     2,
    17,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     6,     9,    10,    18,    30,    37,    41,    43,
    48,    53,    54,    56
};

static const short yyrhs[] = {    20,
    19,     0,    21,    19,     0,    22,    19,     0,     0,     3,
     4,    10,    13,    23,    14,    15,     0,     5,     7,    10,
    13,    23,    14,     6,    13,    23,    14,    15,     0,     8,
    23,     9,    10,    24,    15,     0,    25,    16,    23,     0,
    25,     0,    11,    25,    17,    25,     0,    11,    25,    12,
    25,     0,     0,    10,     0,    18,    10,    18,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    17,    17,    17,    18,    21,    25,    29,    33,    34,    37,
    38,    39,    42,    43
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","CREATE",
"TABLE","INSERT","VALUES","INTO","SELECT","FROM","STRING","WHERE","LIKE","'('",
"')'","';'","','","'='","'\\''","sql","create","insert","sel","colvalspec","selector",
"str", NULL
};
#endif

static const short yyr1[] = {     0,
    19,    19,    19,    19,    20,    21,    22,    23,    23,    24,
    24,    24,    25,    25
};

static const short yyr2[] = {     0,
     2,     2,     2,     0,     7,    11,     6,     3,     1,     4,
     4,     0,     1,     3
};

static const short yydefact[] = {     4,
     0,     0,     0,     4,     4,     4,     0,     0,    13,     0,
     0,     9,     1,     2,     3,     0,     0,     0,     0,     0,
     0,     0,    14,    12,     8,     0,     0,     0,     0,     0,
     0,     0,     7,     5,     0,     0,     0,     0,    11,    10,
     0,     0,     6,     0,     0,     0
};

static const short yydefgoto[] = {    13,
     4,     5,     6,    11,    29,    12
};

static const short yypact[] = {    11,
     2,     0,    -5,    11,    11,    11,    -2,     7,-32768,    12,
    14,     5,-32768,-32768,-32768,    13,    15,     6,    17,    -5,
    -5,    -5,-32768,    18,-32768,    16,    19,    -5,    10,    20,
    25,     3,-32768,-32768,    21,    -5,    -5,    -5,-32768,-32768,
    22,    23,-32768,    32,    37,-32768
};

static const short yypgoto[] = {     4,
-32768,-32768,-32768,   -20,-32768,   -25
};


#define	YYLAST		38


static const short yytable[] = {    25,
    26,    27,    32,    44,     9,     7,     8,    16,    14,    15,
    39,    40,    10,     1,    36,     2,    17,    41,     3,    37,
    20,    18,    19,    23,    33,    21,    24,    22,    28,    30,
    35,    45,    31,    38,    34,    42,    46,    43
};

static const short yycheck[] = {    20,
    21,    22,    28,     0,    10,     4,     7,    10,     5,     6,
    36,    37,    18,     3,    12,     5,    10,    38,     8,    17,
    16,    10,     9,    18,    15,    13,    10,    13,    11,    14,
     6,     0,    14,    13,    15,    14,     0,    15
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 5:
#line 22 "parser.y"
{ trivsql_docreate(yyvsp[-4], yyvsp[-2]); ;
    break;}
case 6:
#line 26 "parser.y"
{ trivsql_doinsert(yyvsp[-8], yyvsp[-6], yyvsp[-2]); ;
    break;}
case 7:
#line 30 "parser.y"
{ trivsql_recordset *rs; trivsql_displayrs(rs = trivsql_doselect(yyvsp[-2], yyvsp[-4]), yyvsp[-2], yyvsp[-4]); /*trivsql_xfree(rs);*/ ;
    break;}
case 8:
#line 33 "parser.y"
{ yyval = trivsql_xsnprintf("%s;%s", yyvsp[-2], yyvsp[0]); ;
    break;}
case 9:
#line 34 "parser.y"
{ yyval = trivsql_xsnprintf("%s", yyvsp[0]); ;
    break;}
case 10:
#line 37 "parser.y"
{ gState->selector = trivsql_selequal; gState->selArgOne = yyvsp[-2]; gState->selArgTwo = yyvsp[0] ;
    break;}
case 11:
#line 38 "parser.y"
{ gState->selector = trivsql_sellike; gState->selArgOne = yyvsp[-2]; gState->selArgTwo = yyvsp[0] ;
    break;}
case 12:
#line 39 "parser.y"
{ gState->selector = NULL ;
    break;}
case 13:
#line 42 "parser.y"
{ yyval = yyvsp[0] ;
    break;}
case 14:
#line 43 "parser.y"
{ yyval = yyvsp[-1] ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 46 "parser.y"


int yyerror(char *s){
  printf("\nsql parsing error: %s\n", s);
  exit(42);
}

// The main routine for the engine
int main(int argc, char *argv[]){
  gState = trivsql_init("foo.tdb");
  yyparse();

  return 0;
}
trivsql_state *trivsql_init(char *filename){
  trivsql_state *state;

  state = (trivsql_state *) trivsql_xmalloc(sizeof(trivsql_state));
  state->db = tdb_open(filename, 0, 0, O_RDWR | O_CREAT, 0600);

  return state;
}

void trivsql_docreate(char *tname, char *cols)
{
  char *t;
  char *u;

  int colCount = 0;

  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  trivsql_dbwrite(gState, t, "0");
  trivsql_xfree(t);

  u = strtok(cols, ";");
  while(u != NULL){
    t = trivsql_xsnprintf("trivsql_%s_col%d", tname, colCount);
    trivsql_dbwrite(gState, t, u);
    trivsql_xfree(t);

    colCount++;
    u = strtok(NULL, ";");
  }

  t = trivsql_xsnprintf("trivsql_%s_numcols", tname);
  u = trivsql_xsnprintf("%d", colCount);
  trivsql_dbwrite(gState, t, u);
  trivsql_xfree(t);
  trivsql_xfree(u);
} 

void trivsql_doinsert(char *tname, char *cols, char *vals){
  char *t, *u, *c;
  int rowCount, i, col, numCols;
  int *colNumbers;

  if((rowCount = trivsql_getrowcount(tname)) == -1){
    return;
  }
  
  // Get ready for columns
  if((colNumbers = trivsql_parsecols(tname, cols, &numCols)) == NULL){
    return;
  }

  // How we have the right number of values?
  col = 1;
  for(i = 0; i < strlen(vals); i++)
    if(vals[i] == ';')
      col++;
  
  if(col != numCols){
    fprintf(stderr, "The number of values does not match the number of columns specified (%d cols, %d vals)\n", 
	    numCols, col);
    return;
  }

  // Save each column value
  c = strtok(vals, ";");
  col = 0;
  while(c != NULL){
    t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[col], rowCount);
    trivsql_dbwrite(gState, t, c);
    trivsql_xfree(t);
    
    c = strtok(NULL, ";");
    col++;
  }

  // And we should keep count of how many of the rows are in the table
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_xsnprintf("%d", rowCount + 1);
  trivsql_dbwrite(gState, t, u);
  trivsql_xfree(t);
  trivsql_xfree(u);
}

trivsql_recordset *trivsql_doselect(char *tname, char *cols){
  int *colNumbers;
  int row, rowCount, numCols, addMe, sac1, sac2;
  char *t, *u, *sa1, *sa2;
  trivsql_recordset *rrs;

  // Get ready for columns
  if((colNumbers = trivsql_parsecols(tname, cols, &numCols)) == NULL){
    return;
  }

  // Decide what rows on the table match the select condition
  if((rowCount = trivsql_getrowcount(tname)) == -1){
    return;
  }

  // Prepare the selector arguements
  sa1 = gState->selArgOne;
  sa2 = gState->selArgTwo;
  sac1 = trivsql_findcol(tname, cols, sa1);
  sac2 = trivsql_findcol(tname, cols, sa2);

  // Build the recordset
  rrs = trivsql_xmalloc(sizeof(trivsql_recordset));
  rrs->numCols = numCols;
  rrs->rows = trivsql_xmalloc(sizeof(trivsql_row));
  rrs->rows->next = NULL;
  rrs->rows->cols = NULL;
  rrs->numRows = 0;

  for(row = 0; row < rowCount; row++){
    addMe = SELTRUE;

    if(gState->selector != NULL){
      if(sac1 != -1){
	trivsql_xfree(sa1);
	t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[sac1], row);
	sa1 = trivsql_dbread(gState, t);
	trivsql_xfree(t);
      }

      if(sac2 != -1){
	trivsql_xfree(sa2);
	t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, colNumbers[sac2], row);
	sa2 = trivsql_dbread(gState, t);
	trivsql_xfree(t);
      }

      addMe = (gState->selector)(sa1, sa2);
    }

    if(addMe == SELTRUE)
      trivsql_addrow(rrs, tname, row, colNumbers);
  }

  return rrs;
}

void *
trivsql_xmalloc (size_t size)
{
  void *buffer;

  if ((buffer = malloc (size)) == NULL)
    {
      fprintf(stderr, "trivsql memory allocation error");
      exit(42);
    }

  return buffer;
}

void
trivsql_dbwrite (trivsql_state *state, char *key, char *value)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL){
    fprintf(stderr, "Cannot store NULL key\n");
    return;
  }

  if (value == NULL){
    fprintf(stderr, "Cannot store NULL data\n");
    return;
  }

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;
  dbdata.dptr = value;
  dbdata.dsize = strlen (value) + 1;

  if (tdb_store (state->db, dbkey, dbdata, TDB_REPLACE) != 0)
    {
      fprintf(stderr, "Database storage error\n");
      return;
    }
}

char *
trivsql_dbread (trivsql_state *state, char *key)
{
  TDB_DATA dbkey, dbdata;

  if (key == NULL){
    fprintf(stderr, "Cannot lookup a NULL key\n");
    return NULL;
  }

  // We need to build the structures for the TDB call
  dbkey.dptr = key;
  dbkey.dsize = strlen (key) + 1;

  dbdata = tdb_fetch (state->db, dbkey);

  return dbdata.dptr;
}

char *
trivsql_xsnprintf (char *format, ...)
{
  char *output = NULL;
  int size, result;
  va_list ap;

  /* We start with the size of the format string as a guess */
  size = strlen (format);
  va_start (ap, format);

  while (1)
    {
      output = (char *) trivsql_xrealloc (output, size);
      result = vsnprintf (output, size, format, ap);

      if (result == -1)
        {
          /* Up to glibc 2.0.6 and Microsoft's implementation */
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
trivsql_xfree (void *memory)
{
  if (memory != NULL)
    free(memory);
}

void *
trivsql_xrealloc (void *memory, size_t size)
{
  void *buffer;

  if ((buffer = realloc (memory, size)) == NULL)
    {
      fprintf(stderr, "Realloc of memory failed");
      exit(42);
    }

  return buffer;
}

int *trivsql_parsecols(char *tname, char *cols, int *numCols){
  int i, col;
  int *colNumbers = NULL;
  char *t, *u, *coltmp, *c;

  // How many columns do we have?
  *numCols = 1;
  for(i = 0; i < strlen(cols); i++)
    if(cols[i] == ';')
      (*numCols)++;

  coltmp = trivsql_xsnprintf("%s", cols);
  colNumbers = trivsql_xmalloc(sizeof(int) * (*numCols));
  
  // Determine that the named columns exist
  col = 0;
  c = strtok(coltmp, ";");
  while(c != NULL){
    i = 0;
    while(1){
      t = trivsql_xsnprintf("trivsql_%s_col%d", tname, i);
      u = trivsql_dbread(gState, t);

      if(u == NULL){
	trivsql_xfree(t);
	fprintf(stderr, "%s is an unknown column\n", c);
	return NULL;
      }
      else if(strcmp(u, c) == 0){
	trivsql_xfree(t);
	trivsql_xfree(u);
	break;
      }

      trivsql_xfree(t);
      trivsql_xfree(u);
      i++;
    }

    colNumbers[col] = i;
    c = strtok(NULL, ";");
    col++;
  }

  return colNumbers;
}

int trivsql_findcol(char *tname, char *cols, char *col){
  char *t, *u, *coltmp, *c;
  int colNum;

  coltmp = trivsql_xsnprintf("%s", cols);
  colNum = 0;

  // Determine that the named columns exist
  c = strtok(coltmp, ";");
  while(c != NULL){
    if(strcmp(c, col) == 0)
      return colNum;

    c = strtok(NULL, ";");
    colNum++;
  }

  return -1;
}

void trivsql_displayrs(trivsql_recordset *rs, char *tname, char *cols){
  int i, col;
  char *t, *u, *c;
  trivsql_row *theRow;
  trivsql_col *theCol;

  // Print the header line
  printf("Select returned %d rows of %d columns\n\n=", rs->numRows, rs->numCols);
  for(i = 0; i < rs->numCols; i++){
    printf("===============");
  }
  printf("\n|");

  // Determine that the named columns exist
  col = 0;
  c = strtok(cols, ";");
  while(c != NULL){
    i = 0;
    while(1){
      t = trivsql_xsnprintf("trivsql_%s_col%d", tname, i);
      u = trivsql_dbread(gState, t);

      if(u == NULL){
	trivsql_xfree(t);
	fprintf(stderr, "%s is an unknown column\n", c);
	return;
      }
      else if(strcmp(u, c) == 0){
	trivsql_xfree(t);
	trivsql_xfree(u);
	break;
      }

      trivsql_xfree(t);
      trivsql_xfree(u);
      i++;
    }

    printf(" %-12s |", c);
    c = strtok(NULL, ";");
  }
  
  printf("\n=");
  for(i = 0; i < rs->numCols; i++){
    printf("===============");
  }
  printf("\n");

  // Print out the values we have found
  theRow = rs->rows;
  while(theRow->next != NULL){
    printf("|");
    theCol = theRow->cols;
    while(theCol->next != NULL){
      printf(" %-12s |", theCol->val);
      theCol = theCol->next;
    }

    printf("\n-");
    for(i = 0; i < rs->numCols; i++){
      printf("---------------");
    }
    printf("\n");
    theRow = theRow->next;
  }

  printf("\n\n");
}

int trivsql_getrowcount(char *tname){
  char *t, *u;
  int rowCount;

  // Determine if the table exists, and if so how many rows it has
  t = trivsql_xsnprintf("trivsql_%s_numrows", tname);
  u = trivsql_dbread(gState, t);
  
  if(u == NULL){
    fprintf(stderr, "Table does not exist\n");
    return -1;
  }

  rowCount = atoi(u);
  trivsql_xfree(u);
  trivsql_xfree(t);

  return rowCount;
}

void trivsql_addrow(trivsql_recordset *rs, char *tname, int row, int *cols){
  char *t;
  int colCount;
  trivsql_row *theRow;
  trivsql_col *theCol;

  // Make space for the new row
  rs->numRows++;
  theRow = rs->rows;
  while(theRow->next != NULL)
    theRow = theRow->next;

  theRow->next = trivsql_xmalloc(sizeof(trivsql_row));
  theRow->next->next = NULL;

  theRow->cols = trivsql_xmalloc(sizeof(trivsql_col));
  theRow->cols->next = NULL;
  theCol = theRow->cols;

  // Get the row
  for(colCount = 0; colCount < rs->numCols; colCount++){
    t = trivsql_xsnprintf("trivsql_%s_col%drow%d", tname, cols[colCount], row);

    theCol->val = trivsql_dbread(gState, t);
    theCol->next = trivsql_xmalloc(sizeof(trivsql_col));
    theCol->next->next = NULL;
    theCol = theCol->next;

    trivsql_xfree(t);
  }
}
