all:	lexer.y lexer.l
	bison -d lexer.y
	flex -olexer.c lexer.l
	gcc lexer.c lexer.tab.c -lfl -lpanda -ltiff -lpng -ljpeg -o pandascript -ggdb
