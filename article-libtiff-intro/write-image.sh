#!/bin/sh

for item in `cat write-image | tr " " "_"`
do
  count=0
  while [ $count -lt 7 ]
  do
    echo $item | tr "_" " "
    count=$(( $count + 1 ))
  done

  count=0
  while [ $count -lt 25 ]
  do
    echo -n "0x00, "
    count=$(( $count + 1 ))
  done

  echo ""
done
