#!/bin/bash

rm /tmp/commitall.log

for item in `ls | grep -v COPYING | egrep -v ".sh$" | egrep -v "~$" | grep -v cvs001`
do
  echo "------------------------------------------------------------------" >> /tmp/commitall.log
  echo $item >> /tmp/commitall.log
  cd $item
  cvs commit 2>> /tmp/commitall.log
  cd ..
done

less /tmp/commitall.log
