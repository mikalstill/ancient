#!/bin/bash
# This is a hack that will build all the tests in a given directory
# for this to work, you must be able to locate the libcppunit.

BINARY=test
TEST_PREFIX=cep_test

echo "rm -f $TEST_PREFIX*.o $BINARY"
rm -f $TEST_PREFIX*.o $BINARY
