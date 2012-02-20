#!/usr/bin/python

# Track number of SMART errors per disk

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import re
import time
import MySQLdb

# 2011-02-01T13:23:04.83576+11:00 0 0013a2004008b9bb 93 request {"buttons"=>"0b00100000", "reason"=>"watchdog backoff", "revision"=>20, "voltage"=>"2.60"}
STATUS_RE = re.compile('.* 0 ([0-9abcdef]*).*voltage"=>"(.*)".*')

def Collect(cursor):
  voltages = {}

  f = open('/data/src/stillhq_private/radish/cache/RadioServer.log', 'r')
  l = f.readline()

  while l:
    m = STATUS_RE.match(l)
    if m:
      mac = m.group(1)
      voltage = m.group(2)
      voltages[mac] = voltage

    l = f.readline()

  for mac in voltages:
    print '%s last seen at %s volts' %(mac, voltages[mac])
    cursor.execute('insert into sensors (epoch_seconds, sensor, value, ip) '
                   'values(%s, "Radish voltage", "%s", "%s");'
                   %(time.time(), voltages[mac], mac))
    cursor.execute('commit;')

if __name__ == '__main__':
  Collect(None)
