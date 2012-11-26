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
import DNS
import MySQLdb

import gflags
import sql


AUTH = yaml.load(open('/home/mikal/.netflow'))


def Collect(cursor):
  DNS.DiscoverNameServers()
  user_cache = {}

  remote_db = MySQLdb.connect(user = AUTH['username'],
                              db = AUTH['database'],
                              passwd = AUTH['password'],
                              host = AUTH['hostname'])
  remote_cursor = remote_db.cursor(MySQLdb.cursors.DictCursor)

  now = datetime.datetime.now()
  one_minute = datetime.timedelta(minutes=1)
  five_minutes = datetime.timedelta(minutes=5)

  # We need to be five minutes behind so the summary db can keep up
  now -= five_minutes

  for i in range(30 * 24 * 60):
    # Internal flows aren't logged in the db, so we ignore molokai here to
    # avoid double counting flows originating from molokai
    statement = ('select internalip, sum(bytes) from flows where '
                 'time >= %s and time < %s and node="zii" group by internalip;'
                 %(sql.FormatSqlValue('date', now - five_minutes),
                   sql.FormatSqlValue('date', now)))
    remote_cursor.execute(statement)

    usage = {}
    epoch = time.mktime((now - five_minutes).timetuple())

    for row in remote_cursor:
      ip = row['internalip']
      print '%s %s %s' %(now - five_minutes, ip,
                         row['sum(bytes)'])

      name = 'Netflow'
      if ip == '192.168.1.20':
        name = 'Gateway Netflow'

      cursor.execute('insert into sensors '
                     '(epoch_seconds, sensor, value, hostname) '
                     'values(%s, "%s", "%s", "%s");'
                     %(epoch, name, row['sum(bytes)'], ip))
      cursor.execute('commit;')

      # Determine the "user" for this IP
      if ip not in user_cache:
        try:
          ip_rev = ip.split('.')
          ip_rev.reverse()
          arpa = '%s.in-addr.arpa' % '.'.join(ip_rev)
          hostname = DNS.dnslookup(arpa, qtype='PTR')[0]
          owner = DNS.dnslookup(hostname, qtype='TXT')[0][0]
          print ('%s Looking up the owner of %s gave %s'
                 %(datetime.datetime.now(), ip, owner))
        except Exception, e:
          print 'Owner lookup error for %s: %s' %(ip, e)
          owner = 'Unknown'

        user_cache[ip] = owner

      print '%s Owner of this IP is %s' %(datetime.datetime.now(),
                                          user_cache[ip])
      usage.setdefault(user_cache[ip], 0)
      usage[user_cache[ip]] += row['sum(bytes)']

    for owner in usage:
      cursor.execute('insert into sensors '
                     '(epoch_seconds, sensor, value, hostname) '
                     'values(%s, "User Netflow", "%s", "%s");'
                     %(epoch, usage[owner], owner))
      cursor.execute('commit;')

  now -= one_minute


if __name__ == '__main__':
  Collect(None)
