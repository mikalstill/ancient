tutorial: output.sgml output.ps
	striptext -c < output.sgml | wc -w

output.sgml: tutorial.sgml
	builddb eps < tutorial.sgml > output.sgml 2> output.blog

output.ps: output.sgml
	db2ps output.sgml 2> errs

output.pdf: output.ps
	ps2pdf output.ps