# How do I add a test to this file?
# Follow the numbered comments...

# 1: If it is a new test subset, add it's name here (starting with test). Note
# that you _do_not_ have to add targets here, especially if you only want them
# as part of huge.
noinst_PROGRAMS = testall testwindows testmatrix testconfig testls testdataset testnumeric testfft testdate testinterp
#testinterp
EXTRA_PROGRAMS = testallhuge

testwindows_SOURCES = alltests.cpp cep_testWindows.cpp
testwindows_LDADD = ../cepDataWindower.o ../cepWindowAlg.o ../cepWindowRect.o ../cepwindowhanning.o ../cepwindowhamming.o ../cepwindowchebyshev.o ../cepwindowblackman.o ../cepErrorHandler.o ../cepConsoleErrorHandler.o ../cepConfiguration.o 

testmatrix_SOURCES = alltests.cpp cep_testMatrix.cpp

testconfig_SOURCES = alltests.cpp cep_testConfig.cpp

testdataset_SOURCES = alltests.cpp cep_testDataset.cpp
testdataset_LDADD = ../cepDataset.o ../cepStringArray.o ../cepMatrixIO.o

testls_SOURCES = alltests.cpp cep_testLs.cpp
testls_LDADDACTUAL = ../cepLs.o 
testls_LDADD = $(testls_LDADDACTUAL) $(testdataset_LDADD)

testnumeric_SOURCES = alltests.cpp cep_testNumeric.cpp

testfft_SOURCES = alltests.cpp cep_testCfft.cpp

testdate_SOURCES = alltests.cpp cep_testDate.cpp
testdate_LDADD = ../cepDate.o

testinterp_SOURCES = alltests.cpp cep_testInterp.cpp
testinterp_LDADD = ../cepInterp.o

# 2: Above this line, you need to add a line test<name>_SOURCES, which lists
# your .cpp files, and (if you need it), a line test<name>_LDADD, which lists
# object files from the source directory which you need for the test

# 3: Add the .cpp files here, and add a variable if you used objects from the
# source directory. Sometimes, if it is heaps of tests which you only want to
# run very occassionally, it might belong in (4) instead...
testall_SOURCES = alltests.cpp cep_testWindows.cpp cep_testMatrix.cpp cep_testConfig.cpp cep_testLs.cpp cep_testDataset.cpp cep_testNumeric.cpp cep_testCfft.cpp cep_testDate.cpp cep_testInterp.cpp

testall_LDADD = $(testwindows_LDADD) $(testls_LDADDACTUAL) $(testdataset_LDADD) $(testdate_LDADD) $(testinterp_LDADD)

# 4: A special target for really long winded tests
testallhuge_SOURCES = alltests.cpp cep_testPresentation000.cpp cep_testPresentation1000.cpp cep_testPresentation200.cpp cep_testPresentation400.cpp cep_testPresentation600.cpp cep_testPresentation800.cpp


# These get built into everything
LDFLAGS = ../cepError.o ../cepUtility.o -lcppunit -lm

# 5: cd ..; automake; autoconf; ./configure; cd tests; make
