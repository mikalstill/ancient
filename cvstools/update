#!/bin/bash

export CVSROOT=/home/httpd/html/cvs
for item in `ls $CVSROOT | grep -v CVS`
do
  echo $item
  if [ -e $item ]
  then
    cd $item
    cvs up
    cd ..
  else
    cvs co $item
  fi
done
