#!/bin/bash

rm -f insertall-$$.tmp 2> /dev/null

for item in `ls | egrep $1 | egrep -v "~$"`
do
  echo "<execute><cmd>builddb.pl</cmd><input>$item</input></execute>" >> insertall-$$.tmp
done

builddb.pl < insertall-$$.tmp
rm insertall-$$.tmp
