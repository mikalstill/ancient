# # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#
# Makefile for GDMS tests
#
# Author: Blake Swadling <blake@swadling.org>, Sept 2002
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# for make clean
RM = rm -f

# compiler settings
CXX = g++
CXXDEFINES = -D_GNU_SOURCE -D__WXGTK__ -D__CEP_UNIT_TEST__
CXXFLAGS = -g -Wall $(CXXDEFINES)

# linker settings
LXX = g++
LXXFLAGS = -L..
LXXLIBS = -lcppunit -lm

# objects for make targets
RUNNER = alltests.o
MATRIX = cep_testMatrix.o
CONFIG = cep_testConfig.o
WINDOWS = cep_testWindows.o

# additional objects required for linking
OBJS = ../cepError.o ../cepUtility.o
OBJS_MATRIX  =
OBJS_CONFIG  =
OBJS_WINDOWS = ../cepDataWindower.o ../cepWindowAlg.o\
../cepWindowRect.o ../cepwindowhamming.o ../cepwindowchebyshev.o\
../cepwindowblackman.o
OBJS_ALL = $(OBJS) $(OBJS_MATRIX) $(OBJS_CONFIG) $(OBJS_WINDOWS)


%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

all: runner $(MATRIX) $(CONFIG) $(WINDOWS)
	$(LXX) $(LXXFLAGS) -o test$@ $(MATRIX) $(CONFIG) $(WINDOWS) $(RUNNER) $(OBJS_ALL) $(LXXLIBS)

tidy :
	@$(RM) *.o core

clean : tidy
	@$(RM) test*

runner: $(OBJS_RUNNER)
  
matrix: runner $(OBJS_MATRIX)
	$(LXX) $(LXXFLAGS) -o test$@ $(MATRIX) $(RUNNER) $(OBJS) $(LXXLIBS)

config: runner $(OBJS_CONFIG)
	$(LXX) $(LXXFLAGS) -o test$@ $(CONFIG) $(RUNNER) $(OBJS) $(LXXLIBS)

windows: runner $(OBJS_WINDOWS)
	$(LXX) $(LXXFLAGS) -o test$@ $(WINDOWS) $(RUNNER) $(OBJS) $(OBJS_WINDOWS) $(LXXLIBS)

