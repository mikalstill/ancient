#!/usr/bin/python

# Grab data from the current cost envi in the lounge room

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import time
import pycurrentcost
import MySQLdb


def Collect(cursor):
  cc = pycurrentcost.CurrentCostReader(port="/dev/ttyUSB2")
  reading = cc.get_reading()

  print ('%s: %s watts %s degrees'
         %(datetime.datetime.now(), reading.channels[1]['watts'],
           reading.temperature))

  cursor.execute('insert into sensors (epoch_seconds, sensor, value, ip) '
                 'values(%s, "Watts", "%s", "CC128");'
                 %(time.time(), reading.channels[1]['watts']))
  cursor.execute('commit;')
  cursor.execute('insert into sensors (epoch_seconds, sensor, value, ip) '
                 'values(%s, "Inside lounge TV", "%s", "CC128");'
                 %(time.time(), reading.temperature))
  cursor.execute('commit;')
