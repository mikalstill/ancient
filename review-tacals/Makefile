short: output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb eps < source.sgml > output.sgml

source.sgml: review.sgml metadata
	buildreview < review.sgml > source.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

output.txt: output.sgml
	docbook2txt output.sgml