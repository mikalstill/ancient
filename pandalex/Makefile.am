noinst_PROGRAMS = lexer
lexer_SOURCES = lexer.y lexer.l samples.c

y.tab.h y.tab.c: lexer.y
	$(YACC) -d lexer.y
