#!/bin/bash

# $1: the serial device to monitor
# $2: the test

for count in 1 2 3
do
  echo "Setup"
  read dummy
  ./mobd.py --output="$2-$count" $1 > /dev/null &
  echo "Press $2"
  sleep 5
  killall -9 mobd.py
  gzip "$2-$count.txt"
  ./decode.py "$2-$count.txt.gz" | grep -i "Trip computer display" | uniq -c
done
