#!/bin/bash

file=`echo $1 | sed 's/^\.\///'`
id=`echo $file | tr "/" "_" | tr "." "_"`

cat - <<EOF
<chapter id="$id">
<title>$file</title>
!I$file
</chapter>

EOF
