tutorial: output.sgml output.ps
	./wordcount.pl output.sgml | wc -w

output.sgml: tutorial.sgml tiff.sgml introduction.sgml concepts.sgml theory.sgml builddb.pl code2db.pl text2db.pl
	./builddb.pl < tutorial.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps
