#!/bin/bash
    
# Regression test the usblogs I have in my collection
tag=`date "+%Y%m%d-%H%M"`

for usblog in `ls *.usblog`
do
  ../usblogdump/usblog-dump -i $usblog > output/$usblog-$tag-full
  if [ `cat output/$usblog-$tag-full | grep "Aborting further decoding" | wc -l | tr -d " "` -gt 0 ]
  then
    echo "$tag: $usblog FAILED TO DECODE" >> FAILED
  fi

  gzip output/$usblog-$tag-full
  cvs add output/$usblog-$tag-full.gz > /dev/null
  ../usblogdump/usblog-dump -i $usblog -r > output/$usblog-$tag-norep
  gzip output/$usblog-$tag-norep
  cvs add output/$usblog-$tag-norep.gz > /dev/null
done

echo $tag > previous
