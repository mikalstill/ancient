#!/bin/bash

count=1 

rm /home/httpd/html/news/opensource/*
export pubdate=`date "+%d %B %Y"`

for item in `find . -type f -name ABOUT -print | sed -e 's/^\.\///' -e 's/\/ABOUT//' | sort -r`
do
  export project=$item
  export description=`cat $item/ABOUT`

  subst.pl < cvstools/template > /tmp/$$
  mv /tmp/$$ /home/httpd/html/news/opensource/`printf "%06d" $count`
  count=$(( $count + 1 ))
done

export project="Summary"
export description="This is a summary of the projects on this page: <ul>"

subst.pl < cvstools/template2 > /tmp/$$

for item in `find . -type f -name ABOUT -print | sed -e 's/^\.\///' -e 's/\/ABOUT//' | sort`
do
  echo "<li><a href=\"#$item\">$item</a>" >> /tmp/$$
done

mv /tmp/$$ /home/httpd/html/news/opensource/`printf "%06d" $count`
