#!/usr/bin/python

# This script fetches information from BOM for the nearest station. We munge
# this so that it looks like just another sensor in our network.

import datetime
import time
import urllib
import MySQLdb


def Collect(cursor):
  for url in ['http://reg.bom.gov.au/products/IDN60903/IDN60903.94926.shtml',
              'http://reg.bom.gov.au/products/IDN60903/IDN60903.94925.shtml']:
    id = url.split('.')[-2]

    print '%s: Fetching (url = %s, id = %s)' %(datetime.datetime.now(), url,
                                               id)
    remote = urllib.urlopen(url)
    print '%s: Fetch done' % datetime.datetime.now()

    in_reading = False
    reading = []

    for line in remote.readlines():
      line = line.rstrip()

      if in_reading and line.startswith('  <td>'):
        if len(reading) > 0:
          reading.append(line[6:-5])

        else:
          t = line[6:-5]
          print '%s: Attempting to parse: %s' %(datetime.datetime.now(), t)

          (year, month) = datetime.datetime.now().timetuple()[:2]
          day = int(t.split('/')[0])
          hour = int(t.split('/')[1].split(':')[0])
          minute = int(t.split('/')[1].split(':')[1][:-2])

          if t[-2] == 'p' and hour != 12:
            hour += 12
          elif t[-2] == 'a' and hour == 12:
            hour = 0

          print '%s: Most recent reading from %s/%s/%s %s:%s' %(datetime.datetime.now(),
                                                                year, month, day,
                                                                hour, minute)
          dt = datetime.datetime(year, month, day, hour, minute)
          print '%s: Parsed date: %s' %(datetime.datetime.now(), dt)
          reading.append(time.mktime(dt.timetuple()))

      if in_reading and line == '</tr>':
        in_reading = False
        print '%s: Reading: %s' %(datetime.datetime.now(), repr(reading))

        epoch = reading[0]
        reading = reading[1:]

        for field in ['BOM Rain since 9am', 'BOM Pressure hPa',
                      '-', '-', 'BOM Wind gust km/h', 'BOM Wind speed km/h',
                      'BOM Wind direction', '-', 'BOM Relative humidity %',
                      'BOM Dew point', 'BOM Apparent temperature',
                      'BOM Temperature']:
          if field != '-':
            cursor.execute('insert ignore into sensors(epoch_seconds, sensor, '
                           'value, ip) values(%s, "%s", "%s", "%s");'
                           %(epoch, field, reading.pop(), id))
            cursor.execute('commit;')
          else:
            reading.pop()

        break

      if line == '<tr class="rowleftcolumn">':
        in_reading = True
        reading = []

    remote.close()
