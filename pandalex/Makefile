all:	lexer.y lexer.l
	bison -d lexer.y
	flex -olexer.c lexer.l
	gcc -DDEBUG samples.c lexer.c lexer.tab.c -lfl -lz -o lexer -ggdb
