#!/usr/bin/python

# Fetch solar exposure information using the complicated meteodata.py script,
# and turn that data into something the home automation system understands.

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import commands
import datetime
import os
import time
import MySQLdb

SCRIPT_PATH = '/data/src/stillhq_public/trunk/homeautomation'

db = MySQLdb.connect(user = 'root', db = 'home')
cursor = db.cursor(MySQLdb.cursors.DictCursor)

# This only works for days in the past because of BOM limitations...
now = datetime.datetime.now()
one_day = datetime.timedelta(days=1)

for days in xrange(7):
  now -= one_day

  t = time.mktime((now.year, now.month, now.day, 0, 0, 0, 0, 0 ,0))
  cursor.execute('select * from sensors where sensor="BOM Solar" and '
                 'epoch_seconds = %d;'
                 % t)
  if cursor.rowcount == 0:
    cmd = ('%s/meteodata.py --lat %s --lon %s '
           '--date %04d%02d%02d --cache '
           '--path=%s/meteodata-cache/ --silent'
           %(SCRIPT_PATH, sys.argv[1], sys.argv[2], now.year,
             now.month, now.day, SCRIPT_PATH))
    value = None

    try:
      print 'Running: %s' % cmd
      out = commands.getoutput(cmd)

      print 'Got: %s' % out
      (date, value) = out.split(',')

    except:
      pass

    if value is None:
      print 'Could not parse data for %04d%02d%02d' %(now.year, now.month, now.day)
      print
      continue

    try:
      cursor.execute('insert into sensors (epoch_seconds, sensor, value, ip) '
                     'values(%s, "BOM Solar", "%s", "BOM");'
                     %(t, value))
      cursor.execute('commit;')

    except Exception, e:
      print 'Error: %s' % e

  print 'Done for %04d%02d%02d' %(now.year, now.month, now.day)
  print

