#!/bin/bash

count=1 

rm /home/httpd/html/news/opensource/*

for item in `find . -type f -name ABOUT -print | sed -e 's/^\.\///' -e 's/\/ABOUT//' | sort -r`
do
  export project=$item
  export description=`cat $item/ABOUT`
  export pubdate=`date "+%d %B %Y"`

  subst.pl < cvstools/template > /tmp/$$
  mv /tmp/$$ /home/httpd/html/news/opensource/`printf "%06d" $count`
  count=$(( $count + 1 ))
done
