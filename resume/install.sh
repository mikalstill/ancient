#!/bin/bash

count=`ls /home/httpd/html/resume | egrep [0-9][0-9][0-9] | sort -n | tail -1`
count=$(( $count + 1 ))

echo $count
mkdir /home/httpd/html/resume/`printf "%03d" $count`
make
cp output.pdf  output.ps  output.rtf  output.sgml /home/httpd/html/resume/`printf "%03d" $count`/