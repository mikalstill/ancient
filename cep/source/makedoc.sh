#!/bin/bash

for item in `ls *.h`
do
  autodocbook $item
done

cd man

# Convert to man pages and html
for item in `ls *.sgml`
do
  docbook2man $item
  docbook2html $item
done
