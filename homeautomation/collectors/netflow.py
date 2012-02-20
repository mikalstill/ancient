#!/usr/bin/python

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import os
import re
import subprocess
import sys
import time
import yaml
import MySQLdb

import gflags
import sql


AUTH = yaml.load(open('/home/mikal/.netflow'))


def Collect(cursor):
  remote_db = MySQLdb.connect(user = AUTH['username'],
                              db = AUTH['database'],
                              passwd = AUTH['password'],
                              host = AUTH['hostname'])
  remote_cursor = remote_db.cursor(MySQLdb.cursors.DictCursor)

  now = datetime.datetime.now()
  one_minute = datetime.timedelta(minutes=1)
  five_minutes = datetime.timedelta(minutes=5)

  for i in range(60):
    statement = ('select internalip, sum(bytes) from flows where '
                 'time >= %s and time < %s group by internalip;'
                 %(sql.FormatSqlValue('date', now - five_minutes),
                   sql.FormatSqlValue('date', now)))
    remote_cursor.execute(statement)
    for row in remote_cursor:
        print '%s %s %s' %(now - five_minutes, row['internalip'],
                           row['sum(bytes)'])

        epoch = time.mktime((now - five_minutes).timetuple())
        name = 'Netflow'
        if row['internalip'] == '192.168.1.14':
          name = 'Gateway Netflow'
        cursor.execute('insert into sensors '
                       '(epoch_seconds, sensor, value, hostname) '
                       'values(%s, "%s", "%s", "%s");'
                       %(epoch, name, row['sum(bytes)'], row['internalip']))
        cursor.execute('commit;')

    now -= one_minute


if __name__ == '__main__':
  Collect(None)
