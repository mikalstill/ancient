all: train test report

train: debug.cpp debug.h dhmm.cpp dhmm.h DHMMTrain.cpp
	c++ debug.cpp dhmm.cpp DHMMTrain.cpp -o train

test: debug.cpp debug.h dhmm.cpp dhmm.h test.cpp
	c++ debug.cpp dhmm.cpp test.cpp -o test

report: output.sgml output.ps output.pdf
	striptext -c < output.sgml | wc -w

output.sgml: source.sgml
	builddb < source.sgml > output.sgml

output.ps: output.sgml
	db2ps output.sgml

output.pdf: output.ps
	ps2pdf output.ps

clean:
	rm -f output.sgml output.ps output.pdf train *.o
