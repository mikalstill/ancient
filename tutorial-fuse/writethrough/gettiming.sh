#!/bin/bash

make clean
make mrproper
make allyesconfig

gnu.time --output=/tmp/foo make
cat /tmp/foo >> /home/opensource/tutorial-fuse/kernel-$1
echo "" >> /home/opensource/tutorial-fuse/kernel-$1
