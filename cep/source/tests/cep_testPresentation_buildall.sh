#!/bin/bash

# I had to do this, because otherwise GCC never exits with code files this
# large (at least on my little laptop)
count=0
jumpsize=200

while [ $count -lt 1200 ]
do
  echo "$count..."
  ./cep_testPresentation_build.sh $count $(( $count + $jumpsize - 1 )) cep_testPresentation`printf "%03d" $count`.cpp
  count=$(( $count + $jumpsize ))
done
