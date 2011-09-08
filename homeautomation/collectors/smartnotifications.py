#!/usr/bin/python

# Track number of SMART errors per disk

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import re
import socket
import subprocess
import time
import MySQLdb


ERRORS_RE = re.compile('.* (.*): *([0-9]+) errors.*')

def Collect(cursor):
  p = subprocess.Popen('/data/src/stillhq_public/trunk/admintools/'
                       'smartnotifications', shell=True, stdout=subprocess.PIPE)
  
  print 'Querying smartnotifications'
  for l in p.stdout.readlines():
    l = l.rstrip()
    print '%s: %s' %(datetime.datetime.now(), l)

    m = ERRORS_RE.match(l)
    if m:
      print 'Matched SMART errors %s = %s' %(m.group(1), m.group(2))
      cursor.execute('insert into sensors '
                     '(epoch_seconds, sensor, value, hostname) '
                     'values(%s, "SMART errors %s", "%s", "%s");'
                     %(time.time(), m.group(1), m.group(2),
                       socket.gethostname()))
      cursor.execute('commit;')

if __name__ == '__main__':
  Collect(None)
