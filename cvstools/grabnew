#!/bin/bash

cat > grabnew-$$.tmp

cvs -d :pserver:cvs@pserver.samba.org:/cvsroot login
for item in `cat grabnew-$$.tmp`
do
  if [ "%$item%" != "%[DIR]%" ]
  then
    if [ ! -e $item ]
    then
      cvs -z5 -d :pserver:cvs@pserver.samba.org:/cvsroot co $item
    fi
  fi
done

rm grabnew-$$.tmp
