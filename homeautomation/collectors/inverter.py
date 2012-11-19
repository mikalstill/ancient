#!/usr/bin/python

import datetime
import socket
import time
import urllib
import MySQLdb

def Collect(cursor):
  for ip in ['zii.home.stillhq.com']:
    try:
      print '%s: Fetching %s' %(datetime.datetime.now(), ip)

      timeout = socket.getdefaulttimeout()
      socket.setdefaulttimeout(2)

      remote = None
      for i in range(5):
        try:
          if ip.startswith(socket.gethostname()):
            # local
            remote = open('/var/www/inverter.txt', 'r')
            break

          else:
            remote = urllib.urlopen('http://%s/inverter.txt' % ip)
            break

        except Exception, e:
          print ('%s: %s: Attempt %s: Exception %s'
                 %(datetime.datetime.now(), ip, i + 1, e))
          time.sleep(5)
      socket.setdefaulttimeout(timeout)

      data = remote.read()
      remote.close()

      fields = ['Read at', 'Temperature', 'Volts DC', 'Current AC', 'Volts AC',
                'Frequency AC', 'Power AC', 'kWh', 'Run', 'Mod']
      values = data.rstrip().split(',')

      for value in values:
        field = fields[0]
        fields = fields[1:]
        print '%s = %s' %(field, value)

        try:
          cursor.execute('insert into sensors'
                         '(epoch_seconds, sensor, value, hostname) '
                         'values(%s, "%s", "%s", "%s");'
                         %(time.time(), field, value, ip))
          cursor.execute('commit;')

        except Exception, e:
          print '%s: %s: VALUE ERROR: %s' %(datetime.datetime.now(), ip, e)

    except Exception, e:
      print '%s: %s: ERROR %s' %(datetime.datetime.now(), ip, e)
