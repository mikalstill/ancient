#!/bin/bash

# Copyright (c) Michael Still 2002
# Released under the terms of the GNU GPL

# In this case, Nemo wants to be able to specify a list of items, with
# weights associated with them...

# $1 is the list with weights, in the form:
# "1 frog 2 banana 3 hamster"

# Scary assumption number one, people hand me correctly formatted lists
# Incidentally, this will break with numbers exist in the items I am handed
# e.g. Banana42 will break this
NUMBERS=`echo $1 | sed 's/[^0-9 ]//g'`
WORDS=`echo $1 | sed 's/[0-9]//g'`
WEIGHTED=""

# Build the list of options, including the weights
for NUM in $NUMBERS
do
  WORD=`echo $WORDS | sed 's/ .*$//'`
  WORDS=`echo $WORDS | sed "s/^$WORD *//"`

  COUNT=0
  while [ $COUNT -lt $NUM ]
  do
    WEIGHTED=`echo "$WEIGHTED $WORD"`
    COUNT=$(( $COUNT + 1 ))
  done  
done

# Get the random number
LOBOUND=1
HIBOUND=`echo $WEIGHTED | wc -w`
RANDMAX=32767
BINUMBER=$(( $LOBOUND + ($HIBOUND * $RANDOM) / ($RANDMAX + 1) ))

# Get the item -- I can't use shift, because it is not on the command line
COUNT=1
while [ $COUNT -lt $BINUMBER ]
do
  WEIGHTED=`echo $WEIGHTED | sed 's/^[^ ]*//'`
  COUNT=$(( $COUNT + 1 ))
done

# The first word should be magic selected one
echo $WEIGHTED | sed 's/ .*$//'
