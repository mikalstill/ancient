tutorial: output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: 2.201.1.sgml book.sgml
	builddb eps < book.sgml > output.sgml 2> output.blog

output.ps: output.sgml
	db2ps output.sgml 2> errs

output.pdf: output.ps
	ps2pdf output.ps