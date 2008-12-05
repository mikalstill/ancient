#!/bin/bash

# A simple script to send notifications to IM recipients, works a bit like the mail command
# $1: the recipient
# stdin: the message

f=`mktemp /tmp/notify.XXXXXX`
echo $1 > $f
cat - >> $f
chmod ugo+rw $f
mv $f /data/src/stillhq_public/trunk/gtalkbot/outbox/
