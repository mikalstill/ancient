#!/bin/bash

rm /tmp/commitall.log

for item in `ls | grep -v COPYING | egrep -v ".sh$" | egrep -v "~$" | grep -v cvs001`
do
  echo "------------------------------------------------------------------" >> /tmp/commitall.log
  if [ -e $item/CVS ]
  then
    echo $item >> /tmp/commitall.log
    cd $item
    cvs commit 2>> /tmp/commitall.log
    cd ..
  else
    echo "************ No version in $item" >> /tmp/commitall.log
  fi
done

less /tmp/commitall.log
