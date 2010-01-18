#!/usr/bin/python

import datetime
import re
import time
import urllib
import MySQLdb

DATA_RE = re.compile('.*<pre>(.*)</pre>.*', re.DOTALL)


db = MySQLdb.connect(user = 'root', db = 'home')
cursor = db.cursor(MySQLdb.cursors.DictCursor)

while True:
  for ip in ['192.168.1.252', '192.168.1.253']:
    try:
      remote = urllib.urlopen('http://%s' % ip)
      data = remote.read()
      remote.close()

      m = DATA_RE.match(data)
      if not m:
        print '%s: NO MATCHING DATA' % datetime.datetime.now()
        print data

      else:
        for l in m.group(1).split('\n'):
          print '%s: %s' %(datetime.datetime.now(), l.rstrip())

          try:
            (name, value) = l.split(': ')

            if not name in ['Fridge temperature', 'Freezer temperature',
                            'Sensor count']:
              cursor.execute('insert into sensors'
                             '(epoch_seconds, sensor, value, ip) '
                             'values(%s, "%s", "%s", "%s");'
                             %(time.time(), name, value, ip))
              cursor.execute('commit;')
              print '  **'

          except Exception, e:
            print '%s: VALUE ERROR: %s' %(datetime.datetime.now(), e)

    except Exception, e:
      print '%s: ERROR %s' %(datetime.datetime.now(), e)
      for name in values:
        values[name].append(None)
  
  print
  time.sleep(10)
