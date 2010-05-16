#!/bin/bash

# $1: the serial device to monitor

for button in clock set
do
  for count in 1 2 3
  do
    ./mobd.py --output="$button-$count" $1 &
    echo "Press $button"
    sleep 10
    killall -9 mobd.py
    ./decode.py "$button-$count" | grep 401
  done
done