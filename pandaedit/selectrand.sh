#!/bin/bash

max=`ls /data/mikal/testdata/pdf-testing/ | wc -l | tr -d " "`

randmax=32767
num=$(( 1 + ($max * $RANDOM) / ($randmax + 1) ))

echo "$num of $max" >> /dev/stderr
echo /data/mikal/testdata/pdf-testing/$num/pdf

