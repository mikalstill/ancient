#!/bin/bash

cvs -d :pserver:cvs@pserver.samba.org:/cvsroot login
for item in `ls | egrep -v ".sh$" | egrep -v "~$"`
do
  echo -------------------------------------------------------------------
  echo $item
  cd $item
  cvs -z5 up
  cd ..
done

echo -------------------------------------------------------------------
