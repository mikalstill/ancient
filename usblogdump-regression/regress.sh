#!/bin/bash
    
# Regression test the usblogs I have in my collection
tag=`date "+%Y%m%d-%H%M"`

for usblog in `ls *.usblog`
do
  ../usblogdump -i $usblog > output/$usblog-$tag-full
  cvs add output/$usblog-$tag-full > /dev/null
  ../usblogdump -i $usblog -r > output/$usblog-$tag-norep
  cvs add output/$usblog-$tag-norep > /dev/null

  if [ -e previous ]
  then
    previous=`cat previous`
    if [ `diff output/$usblog-$previous-full output/$usblog-$tag-full | wc -l | tr -d " "` -gt 0 ]
    then
      echo "$usblog: Full pass differs"
    fi
    if [ `diff output/$usblog-$previous-norep output/$usblog-$tag-norep | wc -l | tr -d " "` -gt 0 ]
    then
      echo "$usblog: Suppressed pass differs"
    fi
  fi
done

echo $tag > previous
