#!/bin/sh

if [ -e output.sgml ]
then
  echo "output.sgml already exists, I refuse to clobber it..."
  exit
fi

date=`date`
cat reports-header.sgml | sed "s/%date%/$date/" > output.sgml

if [ "$1" =  "all" ]
then
  for item in `ls week*sgml`
  do
    cat $item >> output.sgml
  done
else
  cat $1.sgml >> output.sgml
fi
cat reports-footer.sgml >> output.sgml

docbook2ps output.sgml 2> output.errs
ps2pdf output.ps

# Check for errors on the generation output
errlen=`cat output.errs | wc -l | tr -d " "`
if [ $errlen -gt 0 ]
then
  echo "*** There were $errlen errors generating the meetings report"
fi
