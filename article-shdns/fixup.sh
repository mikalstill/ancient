#!/bin/bash
    
for item in `ls sidebar-*`
do 
  cat $item | sed -e 's/&lt;/</g' -e 's/&gt;/>/g' -e 's/&mdash;/--/g' > $item.new
  mv $item.new $item
done