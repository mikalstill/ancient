#!/bin/bash

if [ "%$1%" != "%dirty%" ]
then
  for item in `find . -type f -name "*.[ch]"`
  do
    for page in `scripts/kernel-doc -man $item | egrep "^\.TH" | grep -v "Kernel API" | grep -v "DRM" | sed -e 's/^\.TH \"//' -e 's/\".*$//'`
    do
      echo "$item: $page"
    done
  done
fi
