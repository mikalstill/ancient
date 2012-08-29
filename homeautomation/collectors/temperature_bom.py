#!/usr/bin/python

# This script fetches information from BOM for the nearest station. We munge
# this so that it looks like just another sensor in our network.

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import re
import time
import urllib
import MySQLdb

import cachedfetch

DATA_RE = re.compile(' *<td headers="t[0-9]-(.*)">(.*)</td>')
ONE_DAY = datetime.timedelta(days=1)

DESCRIPTIONS = {'temp': 'BOM Temperature',
                'apptmp': 'BOM Apparent Temperature',
                'dewpoint': 'BOM Dew Point',
                'relhum': 'BOM Relative Humidity',
                'delta-t': 'BOM Delta-T',
                't4-wind-dir': 'BOM Wind Direction',
                't4-wind-spd-kmh': 'BOM Wind Speed KM/h',
                't4-wind-gust-kmh': 'BOM Wind Gust KM/h',
                't4-wind-spd_kts': 'BOM Wind Speed Knots',
                't4-wind-gust_kts': 'BOM Wind Gust Knots',
                'press-qnh': 'BOM Pressure QNH kPa',
                'press-msl': 'BOM Pressure MSL kPa',
                'rainsince9am': 'BOM Rain since 9am'}
                

def Collect(cursor):
  reading_time = None
  now = datetime.datetime.now()


  for url in ['http://reg.bom.gov.au/products/IDN60903/IDN60903.94926.shtml',
              'http://reg.bom.gov.au/products/IDN60903/IDN60903.94925.shtml']:
    id = url.split('.')[-2]

    print '%s: Fetching (url = %s, id = %s)' %(datetime.datetime.now(), url,
                                               id)
    remote = cachedfetch.Fetch(url, maxage=1800)
    print '%s: Fetch done' % datetime.datetime.now()

    for line in remote.split('\n'):
      line = line.rstrip()
      m = DATA_RE.match(line)

      if m:
        field = m.group(1).split(' ')[-1]
        value = m.group(2)

        if field == 'datetime':
          print '%s: timestr --> %s' %(datetime.datetime.now(), line)
          (monthday, timestr) = value.split('/')

          # Find the right day
          monthday = int(monthday)
          reading_time = now
          while monthday != reading_time.day:
            reading_time -= ONE_DAY

          # And the right time
          tstruct = time.strptime(timestr, '%I:%M%p')
          reading_time = datetime.datetime(reading_time.year,
                                           reading_time.month,
                                           reading_time.day,
                                           tstruct.tm_hour,
                                           tstruct.tm_min)

        else:
          print '  %s: %s = %s (%s, %s)' %(reading_time,
                                           DESCRIPTIONS.get(field, field),
                                           value, monthday, timestr)

          cursor.execute('insert ignore into sensors'
                         '(epoch_seconds, sensor, value, hostname) '
                         'values(%s, "%s", "%s", "%s");'
                         %(time.mktime(reading_time.timetuple()),
                           DESCRIPTIONS.get(field, field),
                           value, id))
          cursor.execute('commit;')
                           
