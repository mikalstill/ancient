#!/bin/bash

# Regression test the parser against a database of real PDFs. The PDFs are 
# assumed to be located at /home/httpd/html/pdfdb/<6 digit number>/data.pdf

# Before this will work you need to symlink your GNU time executable to the 
# name time.gnu

# $1 is a regression cycle indicator -- a string of the form <version> - <date>
#      I use 0.5" - "`date`
# $2 is a number to start from (to restart an old test)

if [ "%$2%" == "%%" ]
then
  count=1
else
  count=$2
fi

segfault=0
stack=0
parse=0
ok=0
other=0

path="/home/httpd/html/pdfdb"
pcount=`printf "%06d" $count`
highest=`cat $path/highest`
highest=$(( $highest + 1 ))

while [ $count -lt $highest ]
do
  if [ -d $path/$pcount ]
  then
    time.gnu -v -o /tmp/$$ ./example $path/$pcount/data.pdf > /dev/null 2> /tmp/$$.err
    retval=$?
  
    if [ $retval == 139 ]
    then
      # Bash just uses this
      echo "$1	Segmentation fault" >> $path/$pcount/data.pandalex
      segfault=$(( $segfault + 1 ))
    elif [ $retval == 11 ]
    then
      # The time command changes it to this (see signal(7))
      echo "$1	Segmentation fault" >> $path/$pcount/data.pandalex
      segfault=$(( $segfault + 1 ))
    elif [ $retval == 42 ]
    then
      if [ `grep "stack overflow" /tmp/$$.err | wc -l | tr -d " "` -gt 0 ]
      then
	echo "$1	Stack overflow" >> $path/$pcount/data.pandalex
        stack=$(( $stack + 1 ))
      else
        echo "$1	Parse error" >> $path/$pcount/data.pandalex
        parse=$(( $parse + 1 ))
      fi
    elif [ $retval == 0 ]
    then
      echo "$1	Ok" >> $path/$pcount/data.pandalex
      ok=$(( $ok + 1 ))
    else
      echo "$1	Other ($retval)" >> $path/$pcount/data.pandalex
      other=$(( $other + 1 ))
    fi

    cat /tmp/$$ >> $path/$pcount/data.pandalex
    echo "------------------------------------------" >> $path/$pcount/data.pandalex
    echo "$count: seg = $segfault, parse = $parse, stack = $stack, other = $other, ok = $ok"
  fi

  count=$(( $count + 1 ))
  pcount=`printf "%06d" $count`
done

mono ~mikal/opensource/pdfcollection/Class1.exe "/home/httpd/html/pdfdb"