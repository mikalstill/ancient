all: vcard.l vcard.y
	flex vcard.l
	bison -d vcard.y
	gcc *.c -o vcard -lfl
