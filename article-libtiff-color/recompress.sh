#!/bin/bash

count=1
cp $1 $1-0.tif

while [ $count -lt 200 ]
do
  echo tiffcp -c jpeg $1-$(( $count -1 )).tif $1-$count.tif
  ./recompress $1-$(( $count -1 )).tif $1-$count.tif
  count=$(( $count + 1 ))
done
