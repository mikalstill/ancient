#!/bin/bash

echo "Use this script at your own risk! Keep going?"
read dummy

if [ "%$1%" = "%%" ]
then
  echo "Usage: $0 inputname outputname"
  exit
fi

if [ "%$2%" = "%%" ]
then
  echo "Usage: $0 inputname outputname"
  exit
fi

for item in `ls *.cpp *.h`
do
  echo $item
  cat $item | sed "s/$1/$2/g" > $item.new
  mv $item.new $item
done
