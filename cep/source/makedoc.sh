#!/bin/bash

for item in `ls *.h`
do
  autodocbook $item
done
