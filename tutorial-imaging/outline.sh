#!/bin/bash

for item in `grep "<title>" $1 | grep sect | tr " " "_"`
do
  depth=`echo $item | sed -e 's/^<sect//' -e 's/>.*$//'`
  title=`echo $item | sed -e 's/^.*<title>//' -e 's/<\/title>.*$//'`

  count=1
  while [ $count -lt $depth ]
  do
    echo -n "  "
    count=$(( $count + 1 ))
  done

  echo $title | tr "_" " "
done
