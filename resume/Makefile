short: output.sgml output.ps output.pdf output.rtf
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

output.rtf: output.sgml
	db2rtf output.sgml