short: output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb eps < source.sgml > output.sgml

output-html.sgml: source.sgml
	builddb jpg < source.sgml > output-html.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

output.html: output-html.sgml
	db2html --nochunks output.sgml