#!/usr/bin/python

# Web interface to my home automation stuff

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import asyncore
import copy
import datetime
import gflags
import re
import time
import uuid
import urllib
import MySQLdb

import mhttp
import mplugin

from mpygooglechart import Chart
from mpygooglechart import SimpleLineChart
from mpygooglechart import Axis


FLAGS = gflags.FLAGS
MIN_Y = -30.0
MAX_Y = 70.0
MAX_READINGS_PER_GRAPH = 580
POWER_COST = 0.138
PLUGIN_DIR = '/data/src/stillhq_public/trunk/homeautomation/plugins/'

sensor_names = {}
running = True
uuid = uuid.uuid4()

requests = {}
skips = {}
bytes = 0


wiggle = [0]
backwards = range(-30, 0, 1)
forwards = range(30, 0, -1)
while backwards:
  wiggle.append(backwards.pop())
  wiggle.append(forwards.pop())
wiggle.append(None)

bom_wiggle = [0]
backwards = range(-1800, 0, 1)
forwards = range(1800, 0, -1)
while backwards:
  bom_wiggle.append(backwards.pop())
  bom_wiggle.append(forwards.pop())
bom_wiggle.append(None)


class http_server(mhttp.http_server):
  def client_init(self):
    """Do local setup."""
    
    self.http_handler_class = http_handler


class http_handler(mhttp.http_handler):
  def dispatch(self, urlpath, post_data):
    if urlpath == '/':
      now_tuple = datetime.datetime.now().timetuple()
      self.sendredirect('/chart/today,3,0/Beer%20fridge,Outside%20deck,'
                        'Roof%20cavity%20rear,Inside%20rear')

    elif urlpath.startswith('/sensor/'):
      self.sendredirect(urlpath.replace('/sensor/', '/chart/'))

    elif urlpath.startswith('/chart'):
      self.handleurl_chart(urlpath, post_data)

    elif urlpath.startswith('/table'):
      self.handleurl_table(urlpath, post_data)

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
    
    return (start_epoch, end_epoch, day_field)

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

    args = urlpath.split('/')
    time_field = args[2].split(',')
    (start_epoch, end_epoch, day_field) = self.timewindow(time_field)

    data = ['<h1>Chiller use summary</h1><ul>',
            ('<table><tr><td>When</td><td>Use seconds</td>'
             '<td></td><td>Max outdoor temperature</td><td>Cost</td></tr>')]
    total = 0.0

    t = start_epoch
    while t < end_epoch:
      when = time.localtime(t)
      (total_seconds, summary, cost, max_temp) = self.getchilltime(t,
           t + 24 * 60 * 60)
      data.append('<tr><td>%s.%s.%s</td><td>%s</td><td>'
                  '<a href="javascript:void(0);" '
                  'onmouseover="Tip(\'%s\')" '
                  'onmouseout="UnTip()">Why</a></td><td>%s</td>'
                  '<td>$%.02f</td></tr>'
                  %(when[0], when[1], when[2], total_seconds, summary,
                    max_temp, cost))
      total += cost
      t += 24 * 60 * 60

    data.append('<tr><td></td><td></td><td>Total</td><td>$%.02f</td></tr>'
                % total)
    data.append('</table>')

    self.sendfile('index.html', subst={'data': '\n'.join(data),
                                       'refresh': '3600'})

  def getchilltime(self, start_epoch, end_epoch):
    """Get a summary of the chiller time for a given period."""

    self.log('Fetching chiller usage from %s to %s' %(start_epoch, end_epoch))
    db = MySQLdb.connect(user = 'root', db = 'home')
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    cursor.execute('select * from sensors where sensor="Chilltime" and '
                   'epoch_seconds > %d and epoch_seconds < %d '
                   'order by epoch_seconds;'
                   %(start_epoch - 1, end_epoch - 1))

    total_cost = 0.0
    total_seconds = 0
    start_cost = None
    start_seconds = 0
    start_deducted = False
    previous = 0
    preamble = []
    row = None

    for row in cursor:
      if not start_cost:
        start_cost = int(row['value']) * 85.0 / 1000 / 3600 * POWER_COST
        start_seconds = int(row['value'])
        self.log('Start cost = %s, seconds = %s' %(start_cost, start_seconds))

      else:
        preamble.append(str(previous))
        
      if int(row['value']) < previous:
        cost = previous * 85.0 / 1000 / 3600 * POWER_COST
        seconds = previous

        if not start_deducted:
          cost -= start_cost
          seconds -= start_seconds
          start_deducted = True

        total_cost += cost
        total_seconds += seconds

      previous = int(row['value'])

    cost = previous * 85.0 / 1000 / 3600 * POWER_COST
    total_cost += cost
    total_seconds += previous

    if row:
      self.log('End seconds = %s' % row['value'])

    cursor.execute('select * from sensors where '
                   'sensor="t105912dd010800fd" and '
                   'epoch_seconds > %d and epoch_seconds < %d '
                   'order by epoch_seconds;'
                   %(start_epoch - 1, end_epoch - 1))
    max_temp = 0.0
    for row in cursor:
      if float(row['value']) > max_temp:
        max_temp = float(row['value'])
      
    return (total_seconds, self.summarizingjoin(' ', '...', preamble),
            total_cost, max_temp)
  
  def handleurl_chart(self, urlpath, post_data):
    """Pretty graphs."""

    global sensor_names

    db = MySQLdb.connect(user = 'root', db = 'home')
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    time_field = args[2].split(',')
    (start_epoch, end_epoch, day_field) = self.timewindow(time_field)

    data = []
    if len(args) < 4:
      # TODO(mikal): move this to a generic function
      data = ['<h1>Available sensors</h1><ul>']
      cursor.execute('select distinct(sensor) from sensors where '
                     'epoch_seconds > %d and '
                     'epoch_seconds < %d order by epoch_seconds;'
                     %(start_epoch - 1, end_epoch + 1))
      for row in cursor:
        data.append('<li><a href="/chart/%d.%d.%d/%s">%s</a>'
                    % (int(day_field[0]), int(day_field[1]),
                       int(day_field[2]), row['sensor'],
                       sensor_names.get(row['sensor'], row['sensor'])))
      data.append('</ul>')

    else:
      sensors = mhttp.urldecode(args[3]).split(',')
      data = ['<h1>%s</h1><ul>' % ', '.join(sensors)]

      # TODO(mikal): add options parsing here
      wideinterp = True

      # Build a chart
      chart = SimpleLineChart(600, 400, y_range=[MIN_Y, MAX_Y])
      chart.set_title('Temperature sensors')
      chart.set_colours(['0000FF', '00FF00', 'FF0000',
                         'dd5500', 'ee11ff', '88ddff',
                         '44cc00', 'bb0011', '11aaff'])
      chart.set_grid(0, 20, 5, 5)

      # Chart axes
      left_axis = []
      for v in range(MIN_Y, MAX_Y + 1.0, 5):
        left_axis.append('%s' % v)
      chart.set_axis_labels(Axis.LEFT, left_axis)

      bottom_axis = []
      for v in range(start_epoch, end_epoch + 1,
                     max(1, (end_epoch - start_epoch) / 5)):
        tuple = time.localtime(v)
        bottom_axis.append('%d/%d %02d:%02d' %(tuple[2], tuple[1],
                                               tuple[3], tuple[4]))
      chart.set_axis_labels(Axis.BOTTOM, bottom_axis)

      # Determine how many values will be on the graph
      returned = []
      for sensor in sensors:
        for item in self.ResolveWouldReturn(cursor, sensor, start_epoch,
                                            end_epoch):
          if not item in returned:
            returned.append(item)
      
      # Fetch those values
      values = {}
      redirects = {}

      step_size = ((end_epoch - start_epoch) /
                   (MAX_READINGS_PER_GRAPH / len(returned)))
      self.log('%d time series, each pixel is %d seconds' %(len(returned),
                                                            step_size))
      for sensor in sensors:
        (values, redirects) = self.ResolveSensor(values, redirects, cursor,
                                                 sensor, start_epoch,
                                                 end_epoch, step_size,
                                                 wideinterp)
        self.log('Resolved values: %s' % values.keys())

      # Put the values on the chart
      legend = []
      for value in returned:
        self.log('Adding %s' % value)
        chart.add_data(values[value])
        legend.append(value)
      chart.set_legend(legend)

      # Add markers
      cursor.execute('select * from events where '
                     'epoch_seconds > %d and epoch_seconds < %d '
                     'order by epoch_seconds asc;'
                     %(start_epoch, end_epoch))
      for row in cursor:
        chart.add_marker(0, (row['epoch_seconds'] - start_epoch) / step_size,
                         'o', '00ff00', 10)

      data.append('<img src="%s">' % chart.get_url())
      data.append('</ul>')
      
    self.sendfile('index.html', subst={'data': '\n'.join(data),
                                       'refresh': '60'})

  def handleurl_table(self, urlpath, post_data):
    """A table of data."""

    global sensor_names

    db = MySQLdb.connect(user = 'root', db = 'home')
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')

    wideinterp = True
    incomplete = True
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'nowideinterpolation':
        wideinterp = False
      elif param == 'noincomplete':
        incomplete = False

    self.log('Table parameters: wideinterpolation = %s, incomplete = %s'
             %(wideinterp, incomplete))
    
    time_field = args[2].split(',')
    (start_epoch, end_epoch, day_field) = self.timewindow(time_field)

    data = []
    if len(args) < 4:
      # TODO(mikal): move this to a generic function
      data = ['<h1>Available sensors</h1><ul>']
      cursor.execute('select distinct(sensor) from sensors where '
                     'epoch_seconds > %d and '
                     'epoch_seconds < %d order by epoch_seconds;'
                     %(start_epoch - 1, end_epoch + 1))
      for row in cursor:
        data.append('<li><a href="/table/%d.%d.%d/%s">%s</a>'
                    % (int(day_field[0]), int(day_field[1]),
                       int(day_field[2]), row['sensor'],
                       sensor_names.get(row['sensor'], row['sensor'])))
      data.append('</ul>')

    else:
      sensors = mhttp.urldecode(args[3]).split(',')
      data = ['<h1>%s</h1><ul>' % ', '.join(sensors)]

      values = {}
      redirects = {}
      step_size = 60

      # Fetch data points for the table
      for sensor in sensors:
        (values, redirects) = self.ResolveSensor(values, redirects, cursor,
                                                 sensor, start_epoch,
                                                 end_epoch, step_size,
                                                 wideinterp)
        self.log('Resolved values: %s' % values.keys())

      # Tell people what we fetched
      data.append('<i>To generate this table, we fetched the following values:'
                  ' %s</i><br/><br/>'
                  % ', '.join(values.keys()))

      # The table
      data.append('<table width=100%><tr><td><b>Time</b></td>')
      for sensor in sensors:
        data.append('<td><b>%s</b></td>' % sensor_names.get(sensor, sensor))
      data.append('</tr>')
      bgcolor = 'FFFFFF'

      if incomplete:
        target = 1
      else:
        target = len(sensors)

      # Squelch redirects
      for key in redirects:
        values[key] = self.HandleRedirects(values, redirects, key)

      for i in range(start_epoch, end_epoch, step_size):
        row = ('<tr bgcolor="#%s"><td>%s</td>'
               %(bgcolor, datetime.datetime.fromtimestamp(i)))
        non_null = 0
        for sensor in sensors:
          row += '<td>%s</td>' % values[sensor][0]

          if not values[sensor][0] is None:
            non_null += 1
          values[sensor] = values[sensor][1:]
        row += '</tr>'

        if non_null >= target:
          data.append(row)
          if bgcolor == 'FFFFFF':
            bgcolor = 'DDDDDD'
          else:
            bgcolor = 'FFFFFF'

      data.append('</table>')
      
    self.sendfile('index.html', subst={'data': '\n'.join(data),
                                       'refresh': '3600'})

  def HandleRedirects(self, values, redirects, name):
    """Lookup a value which might have a redirect."""

    if not name in redirects:
      return values[name]

    else:
      self.log('Redirects for %s: %s' %(name, redirects[name]))
      self.log('Values available: %s' % repr(values.keys()))
      return values[redirects[name][0]]

  def ResolveWouldReturn(self, cursor, sensor, start_epoch, end_epoch):
    """What would we get back if we resolved this sensor over this period?"""

    if sensor.startswith('='):
      plugin = mplugin.LoadPlugin(PLUGIN_DIR, sensor[1:], log=self.log)
      if not plugin:
        self.log('No plugin matching %s' % sensor[1:])
        return values

      db = MySQLdb.connect(user = 'root', db = 'home')
      cursor = db.cursor(MySQLdb.cursors.DictCursor)

      out = plugin.Returns(cursor)
      self.log('Resolving %s would return %s' %(sensor, repr(out)))
      return out

    else:
      ips = []
      cursor.execute('select distinct(ip) from sensors where '
                     'sensor in ("%s") and epoch_seconds > %s and '
                     'epoch_seconds < %s;'
                     %(sensor, start_epoch - 1, end_epoch + 1))
      for row in cursor:
        ips.append(row['ip'])

      if len(ips) == 1:
        self.log('Resolving %s would return %s' %(sensor, sensor))
        return [sensor]

      out = []
      for ip in ips:
        out.append('%s (%s)' %(sensor, ip))
      self.log('Resolving %s would return %s' %(sensor, repr(out)))
      return out

  def ResolveSensor(self, values, redirects, cursor, sensor, start_epoch,
                    end_epoch, step_size, wideinterp):
    """Resolve a time series of this sensor value. This could be recursive."""

    self.log('Values for %s between %s and %s' %(sensor, start_epoch,
                                                     end_epoch))

    if sensor.startswith('='):
      plugin = mplugin.LoadPlugin(PLUGIN_DIR, sensor[1:], log=self.log)
      if not plugin:
        self.log('No plugin matching %s' % sensor[1:])
        return values

      db = MySQLdb.connect(user = 'root', db = 'home')
      cursor = db.cursor(MySQLdb.cursors.DictCursor)

      inputs = plugin.Requires(cursor, sensor_names)

      # Fetch missing values
      for input in inputs:
        self.log('Plugin requires: %s' % input)
        if not input in values and input not in redirects:
          (values, redirects) = self.ResolveSensor(values, redirects, cursor,
                                                   input, start_epoch,
                                                   end_epoch, step_size,
                                                   wideinterp)

      # Make redirect handling optional
      input_values = copy.deepcopy(values)
      for input in inputs:
        if input in redirects:
          input_values[input] = copy.deepcopy(self.HandleRedirects(values,
                                                                   redirects,
                                                                   input))

      # Calculate
      self.log('Calculating derived values with inputs: %s'
               % input_values.keys())
      values[sensor] = plugin.Calculate(input_values, redirects, log=self.log)

    else:
      sensors = self.ExpandSensorName(sensor)
      
      ips = []
      cursor.execute('select distinct(ip) from sensors where '
                     'sensor in ("%s") and epoch_seconds > %s and '
                     'epoch_seconds < %s;'
                     %('", "'.join(sensors), start_epoch - 1, end_epoch + 1))
      for row in cursor:
        ips.append(row['ip'])
      self.log('Found IPs: %s' % repr(ips))

      for ip in ips:
        if len(ips) == 1:
          name = sensor
        else:
          name = '%s (%s)' %(sensor, ip)
          redirects.setdefault(sensor, [])
          redirects[sensor].append(name)

        readings = self.GetData(cursor, sensors, ip, start_epoch, end_epoch)
        values[name] = self.GetChartPoints(start_epoch, end_epoch,
                                           step_size, readings, sensor,
                                           wideinterp=wideinterp)
        self.log('Found %d chart points for %s' %(len(values[name]), name))

    return (values, redirects)

  def ExpandSensorName(self, sensor):
    """Turn a sensor name into all possible sensor entries."""

    global sensor_names

    target_sensors = [sensor]
    for s in sensor_names:
      if sensor_names[s] == sensor:
        target_sensors.append(s)

    return target_sensors

  def GetData(self, cursor, sensors, ip, start_epoch, end_epoch):
    """Grab the readings from the DB."""

    readings = {}
    sql = ('select epoch_seconds, value from sensors where '
           'sensor in ("%s") and ip="%s" and epoch_seconds > %d and '
           'epoch_seconds < %d order by epoch_seconds;'
           %('", "'.join(sensors), ip, start_epoch - 1, end_epoch + 1))
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

  def GetChartPoints(self, start_epoch, end_epoch, step_size, readings,
                     sensor, wideinterp=True):
    """Grab an array of values for a chart."""

    global wiggle
    global bom_wiggle

    values = []
    offsets = []

    if step_size < 1:
      step_size = 1

    if not wideinterp:
      wig = wiggle
    elif sensor.startswith('BOM '):
      wig = bom_wiggle
    else:
      wig = wiggle
    
    for t in range(start_epoch, end_epoch, step_size):
      for offset in wig:
        if offset is None:
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
