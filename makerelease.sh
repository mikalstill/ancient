#!/bin/bash

# Do a generic release, including building a tarball
# $1 is the package to release
# $2 is the version number

svn copy $1 ../branches/$1-release-$2

# More stuff here