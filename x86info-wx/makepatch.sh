#!/bin/bash

# Make a release copy of the new code
rm -rf wx-release
cp -R wx wx-release
cd wx-release
rm -f config.status config.log config.cache Makefile
rm -rf .deps
find . -type d -name "CVS" -exec rm -rf {} \;
cd ..

# Make a test copy of Dave Jone's code
rm -rf x86info-1.11-release
cp -R x86info-1.11 x86info-1.11-release
cd x86info-1.11-release
find . -type d -name "CVS" -exec rm -rf {} \;