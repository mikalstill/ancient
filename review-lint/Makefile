short: output.sgml output.ps output.html output.pdf output.txt
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb eps < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

output.txt: output.sgml
	docbook2txt output.sgml

output.html: output.sgml
	docbook2html --nochunks output.sgml