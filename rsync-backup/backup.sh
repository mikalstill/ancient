#!/bin/bash

if [ "%$1%" == "%%" ]
then
  path="/"
else
  path=$1
fi

logger "rsync-backup $$ Starting"
for item in `find $path -xdev -type f -name ".BACKUP" -print`
do
  echo "Seen $item"
  ldir=`echo $item | sed 's/.BACKUP//'`
  rdir=`cat $item`
  logger "rsync-backup $$ Local is $ldir"
  logger "rsync-backup $$ Remote is $rdir"

  logger "rsync-backup $$ Starting rsync"
  date >> $item-LOG
  rsync --exclude=.BACKUP --one-file-system --progress -rcv $ldir/. $rdir/ >> $item-LOG
  logger "rsync-backup $$ Sync returned $?"
done
logger "rsync-backup $$ Finished"