#!/bin/bash

for item in a b c d e f g h i j k l m n o p q r s t u v w x y z
do
  export LETTER=$item
  subst < templateletter.sgml > letter-$item.sgml
done