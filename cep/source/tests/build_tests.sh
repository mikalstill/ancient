#!/bin/bash

# This is a hack that will build all the tests in a given directory
# for this to work, you must be able to locate the libcppunit.

BINARY=test
ALLTESTS=alltests.cpp
TEST_PREFIX=cep_test

DEFINES="-D_GNU_SOURCE -D__WXGTK__ -D__CEP_UNIT_TEST__"
CCFLAGS="-g -Wall"

FILES="../cepMatrix.cpp ../cepError.cpp ../cepConfiguration.cpp"
OBJS="../cepMatrix.o ../cepError.o ../cepConfiguration.o $TEST_PREFIX*.o ../cepUtility.o"
LIBS="-lcppunit -lm -lfl"

# look for alltests
if [ ! -f $ALLTESTS ]; then
	echo "$ALLTESTS was not located."
	echo 'Please change $ALLTESTS to point to it or jst bloody make one :)'
	exit 1
fi

#look for test cases
TEST_FILES=`ls $TEST_PREFIX*`
if [ $? -ne 0 ]; then
	echo "There are no tests defined. Remember that test names"
	echo "must bear the prefix 'cep_test' and that it is case sensitive"
	exit 2 
fi

for FILE in `ls $TEST_PREFIX*.cpp`; do
	OBJ_NAME=`echo $FILE| sed 's/.cpp//g'`
	echo "g++ $CCFLAGS $DEFINES -c -o $OBJ_NAME.o $FILE"
	g++ $CCFLAGS $DEFINES -c -o $OBJ_NAME.o $FILE
done

#make compile the main and link the objects
echo "g++ $CCFLAGS $DEFINES -o $BINARY $OBJS $ALLTESTS $LIBS:"
g++ $CCFLAGS $DEFINES -o $BINARY $OBJS $ALLTESTS $LIBS

if [ $? -ne 0 ]; then
	echo build failed!
else
	echo success!
	echo test executable is called \"$BINARY\".
fi
\n
