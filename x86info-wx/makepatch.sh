#!/bin/bash

cp -R wx wx-release
cd wx-release
rm -f config.status config.log config.cache Makefile
rm -rf .deps
find . -type d -name "CVS" -exec rm -rf {} \;