short: output.sgml output.ps output.pdf output.rtf output.html
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

output.rtf: output.sgml
	db2rtf output.sgml

output.html: output.sgml
	db2html --nochunks output.sgml
	mv output/output.html .
	rm -rf output