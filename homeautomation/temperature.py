#!/usr/bin/python

import datetime
import re
import time
import urllib

from pygooglechart import Chart
from pygooglechart import SimpleLineChart
from pygooglechart import Axis

SENSOR_LOCATIONS = {'t10fa473500000037': 'Beer Fridge'}

MIN_Y = -20.0
MAX_Y = 50.0
DATA_RE = re.compile('.*<pre>(.*)</pre>.*', re.DOTALL)

values = {}
start_time = time.time()

while True:
  try:
    remote = urllib.urlopen('http://192.168.1.253')
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
          if l.startswith('t'):
            (name, value) = l.split(': ')
            values.setdefault(name, [])
            values[name].append(float(value))

        except Exception, e:
          print '%s: VALUE ERROR: %s' %(datetime.datetime.now(), e)

  except Exception, e:
    print '%s: ERROR %s' %(datetime.datetime.now(), e)
    for name in values:
      values[name].append(None)
  
  print

  # Annoyingly we have to create a new graph each time
  for name in values:
    chart = SimpleLineChart(600, 400, y_range=[MIN_Y, MAX_Y])
    chart.set_title(SENSOR_LOCATIONS.get(name, name))
    chart.add_horizontal_range('E5ECF9', 3.5, 4.0)
    chart.set_colours(['0000FF'])
    chart.set_grid(0, 5, 5, 5)

    left_axis = []
    for v in range(MIN_Y, MAX_Y + 1.0, 5):
      left_axis.append('%s' % v)
    chart.set_axis_labels(Axis.LEFT, left_axis)

    bottom_axis = []
    end_time = time.time()
    for v in range(start_time, end_time + 1,
                   max(1, (end_time - start_time) / 5)):
      tuple = time.localtime(v)
      bottom_axis.append('%d/%d %d:%d' %(tuple[2], tuple[1],
                                         tuple[3], tuple[4]))
    chart.set_axis_labels(Axis.BOTTOM, bottom_axis)

    chart.add_data(values[name])
    print chart.get_url()

  print
  time.sleep(10)
