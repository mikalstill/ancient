#!/usr/bin/python

# Grab data from lmsensors about CPU temperature on the MythTV machine (the
# box hosting the homeautomation stuff)

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import re
import socket
import subprocess
import time
import MySQLdb


TEMP_RE = re.compile('(.*): *\+([0-9.]+).*')
RPM_RE = re.compile('(.*): *([0-9.]+) RPM.*')

def Collect(cursor):
  p = subprocess.Popen('sensors', shell=True, stdout=subprocess.PIPE)
  
  print 'Querying lmsensors'
  for l in p.stdout.readlines():
    l = l.rstrip()
    print '%s: %s' %(datetime.datetime.now(), l)

    m = TEMP_RE.match(l)
    if m:
      print 'Matched temp %s = %s' %(m.group(1), m.group(2))
      cursor.execute('insert into sensors '
                     '(epoch_seconds, sensor, value, hostname) '
                     'values(%s, "%s", "%s", "%s");'
                     %(time.time(), m.group(1), m.group(2),
                       socket.gethostname()))
      cursor.execute('commit;')

    m = RPM_RE.match(l)
    if m:
      print 'Matched rpm %s = %s' %(m.group(1), m.group(2))
      cursor.execute('insert into sensors '
                     '(epoch_seconds, sensor, value, hostname) '
                     'values(%s, "%s", "%s", "%s");'
                     %(time.time(), m.group(1), m.group(2),
                       socket.gethostname()))
      cursor.execute('commit;')

if __name__ == '__main__':
  Collect(None)
