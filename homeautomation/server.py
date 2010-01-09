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
MIN_Y = -30.0
MAX_Y = 70.0
MAX_READINGS_PER_GRAPH = 580
POWER_COST = 0.138


sensor_names = {}
running = True
uuid = uuid.uuid4()

requests = {}
skips = {}
bytes = 0


class http_server(mhttp.http_server):
  def client_init(self):
    """Do local setup."""
    
    self.http_handler_class = http_handler


class http_handler(mhttp.http_handler):
  def dispatch(self, urlpath, post_data):
    if urlpath == '/':
      now_tuple = datetime.datetime.now().timetuple()
      self.sendredirect('/sensor/today,3,0/Beer%20fridge,Outside%20deck,'
                        'Roof%20cavity%20rear,Inside%20rear')

    elif urlpath.startswith('/sensor'):
      self.handleurl_sensor(urlpath, post_data)

    elif urlpath.startswith('/chilltime'):
      self.handleurl_chilltime(urlpath, post_data)

    # TODO(mikal): this should be in mhttp
    elif urlpath.startswith('/local/'):
        self.handleurl_local(urlpath, post_data)

    else:
      self.senderror(404, '%s file not found' % urlpath)
      self.close()

  def handleurl_local(self, file, post_data):
    """Return a local file needed by the user interface."""

    ent = file.split('/')[-1]
    self.sendfile(ent)

  def timewindow(self, time_field):
    """Given a time arguement, work out what epoch second window to use."""

    day_field = time_field[0].split('.')
    self.log('Time field is %s, day field is %s' %(time_field, day_field))

    if day_field == ['today']:
      day_field = datetime.datetime.now().timetuple()[0:3]
      self.log('Time field is %s, day field is %s' %(time_field, day_field))

    day = datetime.datetime(int(day_field[0]), int(day_field[1]),
                            int(day_field[2]), 0, 0, 0)
    day_tuple = day.timetuple()
    start_epoch = time.mktime(day_tuple)
    end_epoch = start_epoch + (60 * 60 * 24)
    
    self.log('Day started at epoch seconds: %d' % start_epoch)

    # They _might_ have passed a time window in the time_field
    if len(time_field) > 1:
      start_epoch -= int(time_field[1]) * (60 * 60 * 24)
      end_epoch += int(time_field[2]) * (60 * 60 * 24)
    self.log('Event window is: %d to %d' %(start_epoch, end_epoch))
    
    return (start_epoch, end_epoch)

  def summarizingjoin(self, space, skip, values):
    """space.join(values) with skip instead of repeated values."""

    out = []
    previous = None

    for v in values:
      if v != previous:
        out.append(v)
      elif out[-1] != skip:
        out.append(skip)
      previous = v
      
    return space.join(out)

  def handleurl_chilltime(self, urlpath, post_data):
    """Analyse the expense of running a fridge."""

    db = MySQLdb.connect(user = 'root', db = 'home')
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    time_field = args[2].split(',')
    (start_epoch, end_epoch) = self.timewindow(time_field)

    data = ['<h1>Chiller use summary</h1><ul>',
            ('<table><tr><td>When</td><td>Use seconds</td>'
             '<td></td><td>Cost</td></tr>')]
    cursor.execute('select * from sensors where sensor="Chilltime" and '
                   'epoch_seconds > %d and epoch_seconds < %d '
                   'order by epoch_seconds;'
                   %(start_epoch - 1, end_epoch - 1))

    total = 0.0
    previous = 0
    preamble = []
    for row in cursor:
      preamble.append(str(previous))
      if int(row['value']) < previous:
        cost = previous * 85.0 / 1000 / 3600 * POWER_COST
        total += cost

        when = time.localtime(row['epoch_seconds'])
        data.append('<tr><td>%s.%s.%s</td><td>%s</td><td>'
                    '<a href="javascript:void(0);" '
                    'onmouseover="Tip(\'%s\')" '
                    'onmouseout="UnTip()">Why</a></td>'
                    '<td>$%.02f</td></tr>'
                    %(when[0], when[1], when[2], previous,
                      self.summarizingjoin(' ', '...', preamble),
                      cost))
        preamble = []

      previous = int(row['value'])

    cost = previous * 85.0 / 1000 / 3600 * POWER_COST
    total += cost

    when = time.localtime(row['epoch_seconds'])
    data.append('<tr><td>%s.%s.%s</td><td>%s</td><td>'
                '<a href="javascript:void(0);" '
                'onmouseover="Tip(\'%s\')" '
                'onmouseout="UnTip()">Why</a></td>'
                '<td>$%.02f</td></tr>'
                %(when[0], when[1], when[2], previous,
                  self.summarizingjoin(' ', '...', preamble),
                  cost))

    data.append('<tr><td></td><td></td><td>Total</td><td>$%.02f</td></tr>'
                % total)
    data.append('</table>')

    self.sendfile('index.html', subst={'data': '\n'.join(data)})
  
  def handleurl_sensor(self, urlpath, post_data):
    """The top level page."""

    global sensor_names

    db = MySQLdb.connect(user = 'root', db = 'home')
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    time_field = args[2].split(',')
    (start_epoch, end_epoch) = self.timewindow(time_field)

    data = []
    if len(args) < 4:
      data = ['<h1>Available sensors</h1><ul>']
      cursor.execute('select distinct(sensor) from sensors where '
                     'epoch_seconds > %d and '
                     'epoch_seconds < %d order by epoch_seconds;'
                     %(start_epoch - 1, end_epoch + 1))
      for row in cursor:
        data.append('<li><a href="/sensor/%d.%d.%d/%s">%s</a>'
                    % (int(day_field[0]), int(day_field[1]),
                       int(day_field[2]), row['sensor'],
                       sensor_names.get(row['sensor'], row['sensor'])))
      data.append('</ul>')

    else:
      sensors = mhttp.urldecode(args[3]).split(',')
      data = ['<h1>%s</h1><ul>' % ', '.join(sensors)]

      # Build a chart
      chart = SimpleLineChart(600, 400, y_range=[MIN_Y, MAX_Y])
      chart.set_title('Temperature sensors')
      chart.set_colours(['0000FF', '00FF00', 'FF0000',
                         'dd5500', 'ee11ff', '88ddff',
                         '44cc00', 'bb0011', '11aaff'])
      chart.set_grid(0, 20, 5, 5)

      min_time = 9999999999
      max_time = 0
      readings = {}

      # Determine what data is available for requested sensors
      for sensor in sensors:
        self.log('Collecting data for sensor %s' % sensor)
        readings[sensor] = self.GetData(cursor, sensor, start_epoch, end_epoch)

        if readings[sensor]:
          times = readings[sensor].keys()
          times.sort()
          self.log('Data window for %s is: %d to %d (%d seconds)'
                   %(sensor, times[0], times[-1], times[-1] - times[0]))

          if not times:
            self.senderror(405, 'No data for %s' % args[3])
            return

          if min_time > times[0]:
            min_time = times[0]
          if max_time < times[-1]:
            max_time = times[-1]
      self.log('Final data window is %d to %d (%d seconds)'
               %(min_time, max_time, max_time - min_time))

      # Chart axes
      left_axis = []
      for v in range(MIN_Y, MAX_Y + 1.0, 5):
        left_axis.append('%s' % v)
      chart.set_axis_labels(Axis.LEFT, left_axis)

      bottom_axis = []
      for v in range(min_time, max_time + 1,
                     max(1, (max_time - min_time) / 5)):
        tuple = time.localtime(v)
        bottom_axis.append('%d/%d %02d:%02d' %(tuple[2], tuple[1],
                                               tuple[3], tuple[4]))
      chart.set_axis_labels(Axis.BOTTOM, bottom_axis)

      legend = []
      step_size = ((max_time - min_time) /
                   (MAX_READINGS_PER_GRAPH / len(sensors)))
      for sensor in sensors:
        self.log('Values for %s' % sensor)
        legend.append(sensor_names.get(sensor, sensor))
        values = self.GetChartPoints(start_epoch, end_epoch, min_time,
                                     max_time, step_size, readings[sensor])
        chart.add_data(values)

      # Legend
      chart.set_legend(legend)

      # Add markers
      cursor.execute('select * from events where '
                     'epoch_seconds > %d and epoch_seconds < %d '
                     'order by epoch_seconds asc;'
                     %(min_time, max_time))
      for row in cursor:
        chart.add_marker(0, (row['epoch_seconds'] - min_time) / step_size,
                         'o', '00ff00', 10)

      data.append('<img src="%s">' % chart.get_url())
      data.append('</ul>')
      
    self.sendfile('index.html', subst={'data': '\n'.join(data)})


  def GetData(self, cursor, sensor, start_epoch, end_epoch):
    """Grab the readings from the DB."""

    global sensor_names

    target_sensors = [sensor]
    for s in sensor_names:
      if sensor_names[s] == sensor:
        target_sensors.append(s)

    readings = {}
    sql = ('select epoch_seconds, value from sensors where '
           'sensor in ("%s") and epoch_seconds > %d and '
           'epoch_seconds < %d order by epoch_seconds;'
           %('", "'.join(target_sensors), start_epoch - 1, end_epoch + 1))
    self.log('Data select sql: %s' % sql)
    cursor.execute(sql)
    for row in cursor:
      try:
        readings[int(row['epoch_seconds'])] = float(row['value'])

      except:
        # This isn't a float? Let's try and use a meaningful value instead...
        if row['value'] == 'on':
          readings[int(row['epoch_seconds'])] = 10.0
        elif row['value'] == 'off':
          readings[int(row['epoch_seconds'])] = -10.0
        elif row['value'] == 'yes':
          readings[int(row['epoch_seconds'])] = 5.0
        else:
          readings[int(row['epoch_seconds'])] = None
          self.log('Could not convert %s to a float' % row['value'])

    return readings

  def GetChartPoints(self, start_epoch, end_epoch, min_time, max_time,
                     step_size, readings):
    """Grab an array of values for a chart."""

    values = []
    offsets = []
    for t in range(min_time, max_time, step_size):
      for offset in [0, -1, 1, -2, 2, -3, 3, -4, 4, -5, 5, -6, 6, -7, 7,
                     -8, 8, -9, 9, -10, 10, -11, 11, -15, 15, None]:
        if offset is None:
          # self.log('t: %d, data missing' % t)
          offsets.append(None)
          values.append(None)
          break

        if t + offset in readings:
          offsets.append(offset)
          values.append(readings[t + offset])
          break

    # self.log('Offsets: %s' % repr(offsets))
    return values


def main(argv):
  global running
  global bytes
  global sensor_names
  
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  db = MySQLdb.connect(user = 'root', db = 'home')
  cursor = db.cursor(MySQLdb.cursors.DictCursor)

  cursor.execute('select * from sensor_names;')
  for row in cursor:
    sensor_names[row['sensor']] = row['name']
    print '%s is called %s' %(row['sensor'], row['name'])
  
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
