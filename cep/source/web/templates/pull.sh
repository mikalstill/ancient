#!/bin/bash

rm -f *.html
for item in `ls ~mikal/opensource/cvs001/source/web/templates/*.html`
do
  ln $item `echo $item | sed 's/^.*\///'`
done

chown -R apache.apache *.html