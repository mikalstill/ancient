#!/bin/bash

for item in `ls s*.mfc`
do
  ./encode codebook.mfc $item `echo $item | sed 's/mfc$/enc/'`
done