#!/bin/bash

echo "Finding files matching $1" >> /dev/stderr
rm -f insertall-$$.tmp 2> /dev/null

for item in `ls $1`
do
  echo "Including $item" >> /dev/stderr
  echo "<execute><cmd>builddb.pl</cmd><input>$item</input></execute>" >> insertall-$$.tmp
done

builddb.pl < insertall-$$.tmp
rm insertall-$$.tmp
