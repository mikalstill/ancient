#!/bin/bash

mac=`./getmac.sh`
master=`cat rsync-master`
user=`cat rsync-user`

echo "Syncing clocks"
ntpdate ntp.aarnet.edu.au

echo "Syncing $mac with $user@$master" 

echo "Pulling configuration from master"
export RSYNC_RSH=ssh
rsync -rauv $user@$master:opensource/syncagent/. .
echo "  Done"

echo "Pulling the configured filesystems"
for item in `cat rsync-$mac | tr " " "_"`
do
  local=`echo $item | cut -f 1 -d "_"`
  foreign=`echo $item | cut -f 2 -d "_"`

  echo "$foreign -> $local"
  rsync -rauv $user@$master:$foreign/. $local/
done

echo "Pushing the configured filesystems"
for item in `cat rsync-$mac | tr " " "_"`
do
  local=`echo $item | cut -f 1 -d "_"`
  foreign=`echo $item | cut -f 2 -d "_"`

  echo "$local -> $foreign"
  rsync -rauv $local/. $user@$master:$foreign/
done

echo "All done"
