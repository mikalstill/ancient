#!/usr/bin/python

# Grab data from the current cost envi in the lounge room

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import time
import pycurrentcost
import time
import MySQLdb


NUMSENSORS=4
RADIO_IDS = {
  '02638': 'Watts',
  '00513': 'Deep Freeze Watts',
  '03061': 'Dryer Watts',
  '00022': 'Fridge Watts'
}


def Collect(cursor):
  cc = pycurrentcost.CurrentCostReader(port="/dev/currentcost")

  sensors_seen = []
  exception_count = 0
  while len(sensors_seen) < NUMSENSORS:
    try:
      reading = cc.get_reading()
      if not reading.radio_id in sensors_seen:
        print ('%s: %s watts %s degrees from radio %s (%s)'
               %(datetime.datetime.now(), reading.channels[1]['watts'],
                 reading.temperature, reading.radio_id,
                 RADIO_IDS[reading.radio_id]))
        sensors_seen.append(reading.radio_id)

        cursor.execute('insert into sensors (epoch_seconds, sensor, value, '
                       'hostname) values(%s, "%s", "%s", "CC128");'
                       %(time.time(), RADIO_IDS[reading.radio_id],
                         reading.channels[1]['watts']))
        cursor.execute('commit;')
      else:
        print ('%s: Duplicate reading from radio %s'
               %(datetime.datetime.now(), reading.radio_id))

    except Exception, e:
      exception_count += 1
      print ('%s: Exception %s, count %d'
             %(datetime.datetime.now(), e, exception_count))
      if exception_count > 9:
        return
      time.sleep(1)

  # The current cost only has a temperature sensor on the display unit, so
  # any temp reading works
  cursor.execute('insert into sensors (epoch_seconds, sensor, value, hostname) '
                 'values(%s, "Study desk", "%s", "CC128");'
                 %(time.time(), reading.temperature))
  cursor.execute('commit;')
