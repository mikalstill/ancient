short: output.sgml output.ps
	striptext.pl -c output.sgml | wc -w

output.sgml: source.sgml
	builddb.pl < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps
