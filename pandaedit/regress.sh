#!/bin/bash

# Regression test against a database of real PDFs. The PDFs are assumed to be
# located at /data/mikal/testdata/pdf-testing/<sequential number>/pdf

# Before this will work you need to symlink your GNU time executable to the name time.gnu

# $1 is a regression cycle indicator -- a three digit number
# $2 is a number to start from (to restart an old test)

if [ "%$2%" == "%%" ]
then
  count=1
else
  count=$2
fi

segfault=0
parse=0
ok=0
other=0

while [ -d /data/mikal/testdata/pdf-testing/$count ]
do
  time.gnu -v -o /data/mikal/testdata/pdf-testing/$count/pandaedit-$1-time ./pandaedit /data/mikal/testdata/pdf-testing/$count/pdf > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1-out 2> /data/mikal/testdata/pdf-testing/$count/pandaedit-$1-err
  retval=$?
  
  if [ $retval == 139 ]
  then
    # Bash just uses this
    echo "Segmentation fault" > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1
    segfault=$(( $segfault + 1 ))
  elif [ $retval == 11 ]
  then
    # The time command changes it to this (see signal(7))
    echo "Segmentation fault" > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1
    segfault=$(( $segfault + 1 ))
  elif [ $retval == 42 ]
  then
    echo "Parse error" > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1
    parse=$(( $parse + 1 ))
  elif [ $retval == 0 ]
  then
    echo "Ok" > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1
    ok=$(( $ok + 1 ))

    for item in `grep "PNG filename is" /data/mikal/testdata/pdf-testing/$count/pandaedit-$1-out | grep -v "Binary" | sed 's/.*PNG filename is //'`
    do
      filename=`echo $item | sed -e 's/.*\///' -e "s/pandaedit./pandaedit-$1./" -e 's/.p/-/' -e 's/\.......$/.png/'`
      echo "$item -> /data/mikal/testdata/pdf-testing/$count/$filename"
      mv $item /data/mikal/testdata/pdf-testing/$count/$filename
    done
  else
    echo "Other ($retval)" > /data/mikal/testdata/pdf-testing/$count/pandaedit-$1
    other=$(( $other + 1 ))
  fi

  echo "$count: sf = $segfault, pf = $parse, of = $other, ok = $ok"
  count=$(( $count + 1 ))
done