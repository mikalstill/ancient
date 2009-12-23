#!/usr/bin/python

# Web interface to my home automation stuff

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import asyncore
import datetime
import gflags
import re
import time
import uuid
import urllib
import MySQLdb

import mhttp

from mpygooglechart import Chart
from mpygooglechart import SimpleLineChart
from mpygooglechart import Axis


FLAGS = gflags.FLAGS
MIN_Y = -20.0
MAX_Y = 50.0
MAX_READINGS_PER_GRAPH = 100

SENSOR_LOCATIONS = {'t10fa473500000037': 'Beer Fridge'}


running = True
uuid = uuid.uuid4()

requests = {}
skips = {}
bytes = 0
db = None
cursor = None


class http_server(mhttp.http_server):
  def client_init(self):
    """Do local setup."""
    
    self.http_handler_class = http_handler


class http_handler(mhttp.http_handler):
  def dispatch(self, urlpath, post_data):
    if urlpath == '/':
      now_tuple = datetime.datetime.now().timetuple()
      self.sendredirect('/sensor/%d.%d.%d'
                        %(now_tuple[0], now_tuple[1], now_tuple[2]))

    elif urlpath.startswith('/sensor'):
      self.handleurl_sensor(urlpath, post_data)

    else:
      self.senderror(404, '%s file not found' % urlpath)
      self.close()
  
  def handleurl_sensor(self, urlpath, post_data):
    """The top level page."""

    global cursor

    args = urlpath.split('/')
    day_field = args[2].split('.')
    self.log('Day field is %s' % day_field)
    day = datetime.datetime(int(day_field[0]), int(day_field[1]),
                            int(day_field[2]), 0, 0, 0)
    day_tuple = day.timetuple()
    day_epoch = time.mktime(day_tuple)

    self.log('Day started at epoch seconds: %d' % day_epoch)

    data = []
    if len(args) < 4:
      data = ['<h1>Available sensors</h1><ul>']
      cursor.execute('select distinct(sensor) from sensors where '
                     'epoch_seconds > %d and '
                     'epoch_seconds < %d order by epoch_seconds;'
                     %(day_epoch - 1, day_epoch + (60 * 60 * 24) + 1))
      for row in cursor:
        data.append('<li><a href="/sensor/%d.%d.%d/%s">%s</a>'
                    % (int(day_field[0]), int(day_field[1]),
                       int(day_field[2]), row['sensor'], row['sensor']))
      data.append('</ul>')

    else:
      data = ['<h1>%s</h1><ul>' % args[3]]
      readings = {}

      # Grab the readings from the DB
      cursor.execute('select epoch_seconds, value from sensors where '
                     'sensor = "%s" and epoch_seconds > %d and '
                     'epoch_seconds < %d order by epoch_seconds;'
                     %(args[3], day_epoch - 1, day_epoch + (60 * 60 * 24) + 1))
      for row in cursor:
        readings[int(row['epoch_seconds'])] = float(row['value'])

      times = readings.keys()
      times.sort()
      self.log('Available samples: %s' % times)

      if not times:
        self.senderror(405, 'No data')

      # Build a list of the values to chart
      values = []
      for t in range(times[0], times[-1],
                     (times[-1] - times[0]) / MAX_READINGS_PER_GRAPH):
        for offset in [0, -1, 1, -2, 2, -3, 3, -4, 4, -5, 5, None]:
          if offset is None:
            self.log('t: %d, data missing' % t)
            values.append(None)
            break

          if t + offset in readings:
            self.log('t: %d, offset %d' %(t, offset))
            values.append(readings[t + offset])
            break

      # Build a chart
      chart = SimpleLineChart(600, 400, y_range=[MIN_Y, MAX_Y])
      chart.set_title('Temperature sensors')
      chart.add_horizontal_range('E5ECF9', 3.5, 4.0)
      chart.set_colours(['0000FF'])
      chart.set_grid(0, 5, 5, 5)

      left_axis = []
      for v in range(MIN_Y, MAX_Y + 1.0, 5):
        left_axis.append('%s' % v)
      chart.set_axis_labels(Axis.LEFT, left_axis)

      bottom_axis = []
      for v in range(times[0], times[-1] + 1,
                     max(1, (times[-1] - times[0]) / 5)):
        tuple = time.localtime(v)
        bottom_axis.append('%d/%d %d:%d' %(tuple[2], tuple[1],
                                           tuple[3], tuple[4]))
      chart.set_axis_labels(Axis.BOTTOM, bottom_axis)

      legend = []
      legend.append(SENSOR_LOCATIONS.get(args[3], args[3]))
      chart.set_legend(legend)

      chart.add_data(values)

      data.append('<img src="%s">' % chart.get_url())
      data.append('</ul>')
      
    self.sendfile('index.html', subst={'data': '\n'.join(data)})


def main(argv):
  global running
  global bytes
  global db
  global cursor
  
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  db = MySQLdb.connect(user = 'root', db = 'home')
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  server = http_server(FLAGS.ip, FLAGS.port)

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  last_summary = time.time()
  while running:
    last_event = time.time()
    asyncore.loop(timeout=10.0, count=1)

    if time.time() - last_event > 9.0:
      # We are idle
      print '%s ...' % datetime.datetime.now()

    if time.time() - last_summary > 60.0:
      print '%s TOTAL BYTES SERVED: %s' %(datetime.datetime.now(),
                                          mhttp.DisplayFriendlySize(bytes))
      last_summary = time.time()


if __name__ == "__main__":
  main(sys.argv)
