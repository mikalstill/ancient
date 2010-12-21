#!/usr/bin/python

import datetime
import re
import socket
import time
import urllib
import MySQLdb

DATA_RE = re.compile('.*<pre>(.*)</pre>.*', re.DOTALL)


def Collect(cursor):
  for ip in ['192.168.1.10', '192.168.1.251', '192.168.1.252', '192.168.1.253']:
    try:
      print '%s: Fetching %s' %(datetime.datetime.now(), ip)

      timeout = socket.getdefaulttimeout()
      socket.setdefaulttimeout(2)

      remote = None
      for i in range(5):
        try:
          remote = urllib.urlopen('http://%s' % ip)
          break
        except Exception, e:
          print ('%s: %s: Attempt %s: Exception %s'
                 %(datetime.datetime.now(), ip, i + 1, e))
          time.sleep(5)
      socket.setdefaulttimeout(timeout)

      data = remote.read()
      remote.close()

      m = DATA_RE.match(data)
      if not m:
        print '%s: %s: NO MATCHING DATA' %(datetime.datetime.now(), ip)
        print data

      else:
        for l in m.group(1).split('\n'):
          print '%s: %s: %s' %(datetime.datetime.now(), ip, l.rstrip())

          try:
            (name, value) = l.split(': ')

            if not name in ['Fridge temperature', 'Freezer temperature',
                            'Sensor count']:
              cursor.execute('insert into sensors'
                             '(epoch_seconds, sensor, value, ip) '
                             'values(%s, "%s", "%s", "%s");'
                             %(time.time(), name, value, ip))
              cursor.execute('commit;')

          except Exception, e:
            print '%s: %s: VALUE ERROR: %s' %(datetime.datetime.now(), ip, e)

    except Exception, e:
      print '%s: %s: ERROR %s' %(datetime.datetime.now(), ip, e)
