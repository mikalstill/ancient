short: clean output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: book.sgml
	builddb eps < book.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	touch output.foo foo~; rm output* *~