#!/bin/sh

if [ -e output.sgml ]
then
  echo "output.sgml already exists, I refuse to clobber it..."
  exit
fi

cat meetings-header.sgml > output.sgml
for item in `ls week*sgml`
do
  cat $item >> output.sgml
done
cat meetings-footer.sgml >> output.sgml

docbook2ps output.sgml 2> output.errs
ps2pdf output.ps

# Check for errors on the generation output
errlen=`cat output.errs | wc -l | tr -d " "`
if [ $errlen -gt 0 ]
then
  echo "*** There were $errlen errors generating the meetings report"
fi
