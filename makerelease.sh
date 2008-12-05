#!/bin/bash -e

# Do a generic release, including building a tarball
# $1 is the package to release
# $2 is the version number

#svn copy $1 ../branches/$1-release-$2
#cd ..
#svn commit --message "Release script: Branching $1 for release $2"

mkdir /tmp/$$
cd /tmp/$$
svn co http://www.stillhq.com/svn/branches/$1-release-$2
mv $1-release-$2 $1-$2
tar cvzf $1-$2.tgz $1-$2

echo "Tarball in /tmp/$$"