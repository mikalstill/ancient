#!/bin/bash

# I had to do this, because otherwise GCC never exits with code files this
# large (at least on my little laptop)
./cep_testPresentation_build.sh 0 1000 cep_testPresentationOne.cpp
./cep_testPresentation_build.sh 1001 2000 cep_testPresentationTwo.cpp
./cep_testPresentation_build.sh 2001 3000 cep_testPresentationThree.cpp
./cep_testPresentation_build.sh 3001 4000 cep_testPresentationFour.cpp
./cep_testPresentation_build.sh 4001 5000 cep_testPresentationFive.cpp
