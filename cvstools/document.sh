#!/bin/bash

files="ABOUT README ChangeLog INSTALL AUTHORS NEWS"

for item in `ls | grep -v COPYING | egrep -v ".sh$" | egrep -v "~$" | grep -v TODO`
do
  for file in $files
  do
    if [ ! -e $item/$file ]
    then
	echo "$file $item" > $item/$file
	emacs $item/$file
	cd $item
	cvs add $file
	cd ..
    fi
  done

  if [ ! -e $item/COPYING ]
  then
    cp COPYING $item/COPYING
    cd $item
    cvs add COPYING
    cd ..
  fi
done

