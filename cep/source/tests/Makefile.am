# How do I add a test to this file?
# Follow the numbered comments...

# 1: If it is a new test subset, add it's name here (starting with test)
noinst_PROGRAMS = testall testwindows testmatrix testconfig testls testdataset

testwindows_SOURCES = alltests.cpp cep_testWindows.cpp
testwindows_LDADD = ../cepDataWindower.o ../cepWindowAlg.o ../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o ../cepwindowblackman.o

testmatrix_SOURCES = alltests.cpp cep_testMatrix.cpp

testconfig_SOURCES = alltests.cpp cep_testConfig.cpp

testls_SOURCES = alltests.cpp cep_testLs.cpp
testls_LDADD = ../cepLs.o

testdataset_SOURCES = alltests.cpp cep_testDataset.cpp
testdataset_LDADD = ../cepDataset.o

# 2: Above this line, you need to add a line test<name>_SOURCES, which lists
# your .cpp files, and (if you need it), a line test<name>_LDADD, which lists
# object files from the source directory which you need for the test

# 3: Add the .cpp files here, and add a variable if you used objects from the
# source directory
testall_SOURCES = alltests.cpp cep_testWindows.cpp cep_testMatrix.cpp cep_testConfig.cpp cep_testLs.cpp cep_testDataset.cpp
testall_LDADD = $(testwindows_LDADD) $(testls_LDADD) $(testdataset_LDADD)

# These get built into everything
LDFLAGS = ../cepError.o ../cepUtility.o -lcppunit -lm

# 4: cd ..; automake; autoconf; ./configure; cd tests; make