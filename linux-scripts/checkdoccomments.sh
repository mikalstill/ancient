#!/bin/bash

for item in `cat - | sed 's/: /~/'`
do
  file=`echo $item | cut -f 1 -d "~"`
  page=`echo $item | cut -f 2 -d "~"`

  if [ -f Documentation/DocBook/man/$page.sgml ]
  then
    echo "Found: $file: $page"
  else
    echo "Missing: $file: $page"
  fi
done
