all:	lexer.y lexer.l
	bison -d lexer.y
	flex -olexer.c lexer.l
	gcc samples.c lexer.c lexer.tab.c -lfl -o lexer -ggdb
