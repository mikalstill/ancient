# How do I add a test to this file?
# Follow the numbered comments...

# 1: If it is a new test subset, add it's name here (starting with test)
noinst_PROGRAMS = testall testallhuge testwindows testmatrix testconfig testls testdataset testpresentation

testwindows_SOURCES = alltests.cpp cep_testWindows.cpp
testwindows_LDADD = ../cepDataWindower.o ../cepWindowAlg.o ../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o ../cepwindowblackman.o

testmatrix_SOURCES = alltests.cpp cep_testMatrix.cpp

testconfig_SOURCES = alltests.cpp cep_testConfig.cpp

testls_SOURCES = alltests.cpp cep_testLs.cpp
testls_LDADD = ../cepLs.o

testdataset_SOURCES = alltests.cpp cep_testDataset.cpp
testdataset_LDADD = ../cepDataset.o

testpresentation_SOURCES = alltests.cpp cep_testPresentation000.cpp cep_testPresentation1000.cpp cep_testPresentation1200.cpp cep_testPresentation1400.cpp cep_testPresentation1600.cpp cep_testPresentation1800.cpp cep_testPresentation2000.cpp cep_testPresentation200.cpp cep_testPresentation2200.cpp cep_testPresentation2400.cpp cep_testPresentation2600.cpp cep_testPresentation2800.cpp cep_testPresentation3000.cpp cep_testPresentation3200.cpp cep_testPresentation3400.cpp cep_testPresentation3600.cpp cep_testPresentation3800.cpp cep_testPresentation4000.cpp cep_testPresentation400.cpp cep_testPresentation4200.cpp cep_testPresentation4400.cpp cep_testPresentation600.cpp cep_testPresentation800.cpp

# 2: Above this line, you need to add a line test<name>_SOURCES, which lists
# your .cpp files, and (if you need it), a line test<name>_LDADD, which lists
# object files from the source directory which you need for the test

# 3: Add the .cpp files here, and add a variable if you used objects from the
# source directory. Sometimes, if it is heaps of tests which you only want to
# run very occassionally, it might belong in (4) instead...
testall_SOURCES = alltests.cpp cep_testWindows.cpp cep_testMatrix.cpp cep_testConfig.cpp cep_testLs.cpp cep_testDataset.cpp
testall_LDADD = $(testwindows_LDADD) $(testls_LDADD) $(testdataset_LDADD)

# 4: Big occassional tests go here instead of above. The same rules apply...
testallhuge_SOURCES = $(testall_SOURCES) $(testpresentation_SOURCES)
testallhuge_LDADD = $(testall_LDADD)

# These get built into everything
LDFLAGS = ../cepError.o ../cepUtility.o -lcppunit -lm

# 5: cd ..; automake; autoconf; ./configure; cd tests; make