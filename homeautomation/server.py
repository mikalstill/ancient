#!/usr/bin/python

# Web interface to my home automation stuff

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import asyncore
import copy
import datetime
import os
import re
import time
import uuid
import urllib
import MySQLdb

import gflags
import mhttp
import mplugin
import substitute

from mpygooglechart import Chart
from mpygooglechart import SimpleLineChart
from mpygooglechart import Axis


FLAGS = gflags.FLAGS
gflags.DEFINE_string('dbuser', 'home', 'DB username')
gflags.DEFINE_string('dbname', 'homeautomation', 'DB name')

MIN_Y = -30.0
MAX_Y = 100.0
MAX_READINGS_PER_GRAPH = 580
POWER_COST = 0.138

DAILY_DIR = '/data/src/stillhq_public/trunk/homeautomation/daily/'
FUNCTION_DIR = '/data/src/stillhq_public/trunk/homeautomation/functions/'
PLUGIN_DIR = '/data/src/stillhq_public/trunk/homeautomation/plugins/'

sensor_names = {}
running = True
uuid = uuid.uuid4()

requests = {}
skips = {}
bytes = 0

one_day = datetime.timedelta(days=1)

class http_server(mhttp.http_server):
  def client_init(self):
    """Do local setup."""
    
    self.http_handler_class = http_handler


class http_handler(mhttp.http_handler):
  def dispatch(self, urlpath, post_data, chunk=None):
    if urlpath == '/':
      now_tuple = datetime.datetime.now().timetuple()
      self.sendredirect('/dashboard')

    elif urlpath.startswith('/sensor/'):
      self.sendredirect(urlpath.replace('/sensor/', '/chart/'))

    elif urlpath.startswith('/chart'):
      self.handleurl_chart(urlpath, post_data)

    elif urlpath.startswith('/flash'):
      self.handleurl_flash(urlpath, post_data)

    elif urlpath.startswith('/image'):
      self.handleurl_image(urlpath, post_data)

    elif urlpath.startswith('/table'):
      self.handleurl_table(urlpath, post_data)

    elif urlpath.startswith('/csv'):
      self.handleurl_csv(urlpath, post_data)

    elif urlpath.startswith('/json'):
      self.handleurl_json(urlpath, post_data)

    elif urlpath.startswith('/chilltime'):
      self.handleurl_chilltime(urlpath, post_data)

    elif urlpath.startswith('/dashboard'):
      self.handleurl_dashboard(urlpath, post_data)

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

    elif day_field == ['yesterday']:
      yesterday = datetime.datetime.now() - one_day
      day_field = yesterday.timetuple()[0:3]
      self.log('Time field is %s, day field is %s' %(time_field, day_field))

    # Convert the day fields into ints
    int_day_field = []
    for df in day_field:
      int_day_field.append(int(df))

    day = datetime.datetime(*int_day_field)
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
    (start_epoch, end_epoch, day_field) = self.timewindow(args[2].split(','))

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

    self.sendfile('index.html', subst={'head': '',
                                       'data': '\n'.join(data),
                                       'refresh': '3600'})

  def getchilltime(self, start_epoch, end_epoch):
    """Get a summary of the chiller time for a given period."""

    self.log('Fetching chiller usage from %s to %s' %(start_epoch, end_epoch))
    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
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

  def AvailableSensors(self, cursor, range):
    """Return a HTML encoded list of the available sensors for a given day."""

    global sensor_names

    sensors = []
    cursor.execute('select distinct(sensor), max(epoch_seconds) from sensors '
                   'where epoch_seconds > %d and '
                   'epoch_seconds < %d group by sensor order by epoch_seconds;'
                   %(range[0] - 1, range[1] + 1))
    for row in cursor:
      t = datetime.datetime.utcfromtimestamp(row['max(epoch_seconds)'])
      sensors.append('<li><a href="/chart/%d.%d.%d/%s">%s</a> '
                     '(last reading at %s)'
                     % (int(range[2][0]), int(range[2][1]),
                        int(range[2][2]), row['sensor'],
                        sensor_names.get(row['sensor'], row['sensor']), t))

    for ent in os.listdir(PLUGIN_DIR):
      if ent.endswith('.py'):
        plugin = mplugin.LoadPlugin(PLUGIN_DIR, ent[:-3], log=self.log)
        try:
          for out in plugin.Returns(cursor):
            sensors.append('<li><a href="/chart/%d.%d.%d/%s">%s</a>'
                           % (int(range[2][0]), int(range[2][1]),
                              int(range[2][2]), out, out))
        except Exception, e:
          self.log('Exception determining return values for plugin %s: %s'
                   %(ent, e))

    sensors.sort()
    return ['<h1>Available sensors</h1><ul>',
            '\n'.join(sensors),
            '</ul>']
  
  def handleurl_chart(self, urlpath, post_data):
    """Pretty graphs wrapped in HTML."""

    global sensor_names

    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    links = [self.time_link(urlpath, args[2], ranges[0], -one_day, '&lt;&lt;'),
             self.time_link(urlpath, args[2], ranges[0], one_day, '&gt;&gt;'),
             '<a href="/flash/%s">Flash</a>' % '/'.join(args[2:]),
             '<a href="/table/%s">Table</a>' % '/'.join(args[2:]),
             '<a href="/csv/%s">CSV</a>' % '/'.join(args[2:]),
             '<a href="/json/%s">JSON</a>' % '/'.join(args[2:])]

    self.log('Time ranges: %s' % repr(ranges))

    data = []
    if len(args) < 4:
      data = self.AvailableSensors(cursor, ranges[0])

    else:
      sensors = mhttp.urldecode(args[3]).split(',')
      data = ['<h1>%s</h1><ul>' % ', '.join(sensors),
              '<img src="%s">' % urlpath.replace('chart', 'image'),
              '</ul>']

    self.sendfile('index.html', subst={'head': '',
                                       'data': '\n'.join(data),
                                       'links': ' '.join(links),
                                       'refresh': '60'})

  def handleurl_flash(self, urlpath, post_data):
    """Pretty graphs done with flash wrapped in HTML."""

    global sensor_names

    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    links = [self.time_link(urlpath, args[2], ranges[0], -one_day, '&lt;&lt;'),
             self.time_link(urlpath, args[2], ranges[0], one_day, '&gt;&gt;'),
             '<a href="/chart/%s">Static</a>' % '/'.join(args[2:]),
             '<a href="/table/%s">Table</a>' % '/'.join(args[2:]),
             '<a href="/csv/%s">CSV</a>' % '/'.join(args[2:]),
             '<a href="/json/%s">JSON</a>' % '/'.join(args[2:])]

    self.log('Time ranges: %s' % repr(ranges))

    data = []
    head = []
    if len(args) < 4:
      data = self.AvailableSensors(cursor, ranges[0])

    else:
      sensors = mhttp.urldecode(args[3]).split(',')
      data = ['<h1>%s</h1><ul>' % ', '.join(sensors),
              '<div id="my_chart"></div>',
              '</ul>']
      
      f = open('ofc-helper.html')
      template = f.read()
      f.close()
      head = substitute.substitute(template,
                                   subst={'data_file': urlpath.replace('flash',
                                                                       'json')
                                          })

    self.sendfile('index.html', subst={'head': head,
                                       'data': '\n'.join(data),
                                       'links': ' '.join(links),
                                       'refresh': '3600'
                                       })

  def handleurl_image(self, urlpath, post_data):
    """Pretty graphs with the HTML."""

    global sensor_names

    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    self.log('Time ranges: %s' % repr(ranges))

    # TODO(mikal): add options parsing here
    sensors = mhttp.urldecode(args[3]).split(',')
    wideinterp = True
    size = (600, 400)
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'nowideinterpolation':
        wideinterp = False

      elif param.startswith('size'):
        x, y = param.split('=')[1].split('x')
        size = (int(x), int(y))

    self.log('Data fetch parameters: wideinterpolation = %s' % wideinterp)

    # Build a chart
    chart = SimpleLineChart(size[0], size[1], y_range=[MIN_Y, MAX_Y])
    chart.set_title('Sensors')
    chart.set_colours(['0000FF', '00FF00', 'FF0000',
                       'dd5500', 'ee11ff', '88ddff',
                       '44cc00', 'bb0011', '11aaff'])
    chart.set_grid(0, 20, 5, 5)

    # Chart axes
    left_axis = []
    right_axis = []
    for v in range(int(MIN_Y), int(MAX_Y + 1.0), 5):
      left_axis.append('%s' % v)
      right_axis.append('%.01fk' %((v * 50.0) / 1000))
    chart.set_axis_labels(Axis.LEFT, left_axis)
    chart.set_axis_labels(Axis.RIGHT, right_axis)

    bottom_axis = []
    for v in range(ranges[0][0], ranges[0][1] + 1,
                   max(1, (ranges[0][1] - ranges[0][0]) / 5)):
      tuple = time.localtime(v)
      if len(ranges) == 1:
        bottom_axis.append('%d/%d %02d:%02d' %(tuple[2], tuple[1],
                                               tuple[3], tuple[4]))
      else:
        bottom_axis.append('%02d:%02d' %(tuple[3], tuple[4]))
    chart.set_axis_labels(Axis.BOTTOM, bottom_axis)

    # Determine how many values will be on the graph
    returned = []
    for sensor in sensors:
      for item in self.ResolveWouldReturn(cursor, sensor, ranges[0][0],
                                          ranges[0][1]):
        if not item in returned:
          returned.append(item)
    self.log('Calculations will return: %s' % repr(returned))

    # Fetch those values
    step_size = ((ranges[0][1] - ranges[0][0]) /
                 (MAX_READINGS_PER_GRAPH / (len(returned) * len(ranges))))
    self.log('%d time series, each pixel is %d seconds'
             %(len(returned) * len(ranges), step_size))

    # Put the values on the chart
    legend = []
    for r in ranges:
      values = {}
      redirects = {}

      for sensor in sensors:
        (values, redirects) = self.ResolveSensor(values, redirects, cursor,
                                                 sensor, r[0], r[1],
                                                 step_size, wideinterp)
      self.log('Resolved values: %s' % values.keys())

      for value in returned:
        self.log('Adding %s' % value)
        chart.add_data(values[value])

        if len(ranges) == 1:
          legend.append(value)
        else:
          legend.append('%s %s' %(value, times[0]))
      times = times[1:]

    chart.set_legend(legend)

    # Add markers
    cursor.execute('select * from events where '
                   'epoch_seconds > %d and epoch_seconds < %d '
                   'order by epoch_seconds asc;'
                   %(ranges[0][0], ranges[0][1]))
    for row in cursor:
      chart.add_marker(0, (row['epoch_seconds'] - ranges[0][0]) / step_size,
                       'o', '00ff00', 10)

    self.sendredirect(chart.get_url())

  def ParseTimeField(self, tf):
    """Parse the time field from the URL."""

    ranges = []
    times = []

    for time_field in tf.split('!')[0].split(';'):
      # Each range is (start_epoch, end_epoch, day_field)
      ranges.append(self.timewindow(time_field.split(',')))
      times.append(time_field)

    step_size = None
    try:
      step_size = tf.split(';')[0].split('!')[1]
      if step_size == 'daily':
        step_size = 24 * 3600
      elif step_size == 'hourly':
        step_size = 3600
      else:
        step_size = int(step_size)

    except:
      pass

    return (ranges, times, step_size)

  def FetchTableData(self, args):
    """Fetch a table's worth of data."""

    global sensor_names

    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    if not step_size:
      step_size = 60

    self.log('Time ranges: %s' % repr(ranges))

    wideinterp = True
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'nowideinterpolation':
        wideinterp = False
    self.log('Data fetch parameters: wideinterpolation = %s' % wideinterp)
  
    sensors = mhttp.urldecode(args[3]).split(',')
    data = ['<h1>%s</h1><ul>' % ', '.join(sensors)]

    values = {}
    returned = []
    max_window_size = ranges[0][1] - ranges[0][0]

    # Fetch data points for the table
    for sensor in sensors:
      for r in ranges:
        self.log('Fetching values for %s between %s and %s (%s seconds)'
                 %(sensor, r[0], r[1], r[1] - r[0]))
        max_window_size = r[1] - r[0]
        (t_values, t_redirects) = self.ResolveSensor(values, {}, cursor,
                                                     sensor, r[0], r[1],
                                                     step_size,
                                                     wideinterp)
        self.log('Resolved values: %s' % t_values.keys())

        if len(ranges) == 1:
          for unique_sensor in t_values.keys():
            values[unique_sensor] = t_values[unique_sensor]
            returned.append(unique_sensor)
            self.log('Creating simple value for %s' % unique_sensor)

        else:
          t = times[ranges.index(r)]
          k = '%s %s' %(sensor, t)
          values[k] = t_values[sensor]
          returned.append(k)
          self.log('Creating meta value %s from %s at %s' %(k, sensor, t))

    return (int(ranges[0][0]), int(ranges[0][0] + max_window_size), step_size,
            returned, values)

  def handleurl_table(self, urlpath, post_data):
    """A table of data."""

    global sensor_names

    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    links = [self.time_link(urlpath, args[2], ranges[0], -one_day, '&lt;&lt;'),
             self.time_link(urlpath, args[2], ranges[0], one_day, '&gt;&gt;'),
             '<a href="/flash/%s">Flash</a>' % '/'.join(args[2:]),
             '<a href="/chart/%s">Chart</a>' % '/'.join(args[2:]),
             '<a href="/csv/%s">CSV</a>' % '/'.join(args[2:]),
             '<a href="/json/%s">JSON</a>' % '/'.join(args[2:])]

    data = []

    incomplete = True
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'noincomplete':
        incomplete = False
    self.log('Table parameters: incomplete = %s' % incomplete)

    if len(args) < 4:
      data = self.AvailableSensors(cursor, ranges[0])

    else:
      (start_epoch, end_epoch, step_size,
       returned, values) = self.FetchTableData(args)

      # Tell people what we fetched
      data.append('<i>To generate this table, we fetched the following values:'
                  ' %s</i><br/><br/>'
                  % ', '.join(values.keys()))

      # The table
      data.append('<table width=100%><tr><td><b>Time</b></td>')
      for sensor in returned:
        data.append('<td><b>%s</b></td>' % sensor_names.get(sensor, sensor))
      data.append('</tr>')
      bgcolor = 'FFFFFF'

      if incomplete:
        target = 1
      else:
        target = len(returned)

      self.log('Displayed data is from %s to %s (%s seconds)'
               %(start_epoch, end_epoch, end_epoch - start_epoch))
      for i in range(start_epoch, end_epoch, step_size):
        row = ('<tr bgcolor="#%s"><td>%s</td>'
               %(bgcolor, datetime.datetime.fromtimestamp(i)))
        non_null = 0
        for sensor in returned:
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
      
    self.sendfile('index.html', subst={'head': '',
                                       'data': '\n'.join(data),
                                       'links': ' '.join(links),
                                       'refresh': '3600'})

  def handleurl_csv(self, urlpath, post_data):
    """A table of csv formatted data."""

    global sensor_names

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    data = []

    incomplete = True
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'noincomplete':
        incomplete = False
    self.log('Table parameters: incomplete = %s' % incomplete)

    if len(args) < 4:
      data = self.AvailableSensors(cursor, ranges[0])

    else:
      (start_epoch, end_epoch, step_size,
       returned, values) = self.FetchTableData(args)

      # The table
      header = ['timestamp']
      for sensor in returned:
        header.append('"%s"' % sensor_names.get(sensor, sensor))
      data.append(','.join(header))

      if incomplete:
        target = 1
      else:
        target = len(returned)

      for i in range(start_epoch, end_epoch, step_size):
        row = '%s' %(datetime.datetime.fromtimestamp(i).\
                       strftime('%Y%m%d%H%M'))
        non_null = 0
        for sensor in returned:
          if values[sensor][0]:
            row += ',%s' % values[sensor][0]
          else:
            row += ','

          if not values[sensor][0] is None:
            non_null += 1
          values[sensor] = values[sensor][1:]

        if non_null >= target:
          data.append(row)
      
    self.senddata('\n'.join(data))

  def handleurl_json(self, urlpath, post_data):
    """A table of JSON formatted data."""

    # If terse is set, we drop repeated values and assume the renderer
    # can handle that with a horizontal line. This is true for the flash
    # graphing engine for example

    global sensor_names

    args = urlpath.split('/')
    (ranges, times, step_size) = self.ParseTimeField(args[2])
    data = []

    incomplete = True
    terse = False
    for param in args[1].split(','):
      self.log('Considering table param: %s' % param)
      if param == 'noincomplete':
        incomplete = False
      if param == 'terse':
        terse = True

    self.log('Table parameters: incomplete = %s, terse = %s'
             %(incomplete, terse))

    if len(args) < 4:
      data = self.AvailableSensors(cursor, ranges[0])
      self.senddata('\n'.join(data))

    else:
      (start_epoch, end_epoch, step_size,
       returned, values) = self.FetchTableData(args)
    
      if len(ranges) == 1:
        xformat = '#date:j/n g:i#'
      else:
        xformat = '#date: g:i#'

      f = open('data.json.element')
      template = f.read()
      f.close()

      colors = ['0000FF', '00FF00', 'FF0000', '88ddff', 'bb0011', '11aaff',
                '9DAF72', '566047', '562F32', '462D44', '859731', '640E27',
                'AD0066', 'D13D94', 'F5851F', 'CADA2A', '81A4B9', 'CFE8F6']

      y_max = 0
      elements = []

      for r in returned:
        upto = ranges[0][0]
        float_values = []

        previous = None
        for v in values[r]:
          try:
            current = float(v)
            if not terse:
              float_values.append('{"x": %d, "y": %.03f}'
                                  %(upto, current))
            else:
              if current != previous:
                if previous is not None:
                  float_values.append('{"x": %d, "y": %.03f}'
                                      %(upto, previous))

                float_values.append('{"x": %d, "y": %.03f}'
                                    %(upto, current))
            previous = current

            if current > y_max:
              y_max = current
          except:
            pass

          upto += step_size

        elements.append(substitute.substitute(template,
                                              subst={'name': sensor_names.get(r, r),
                                                     'values': ', '.join(float_values),
                                                     'color': colors[returned.index(r)]
                                                     }))

      elements.reverse()
      self.sendfile('data.json',
                    subst={'y_max': y_max,
                           'y_step': y_max / 15,
                           'elements': ', '.join(elements),
                           'x_min': ranges[0][0],
                           'x_max': ranges[0][1],
                           'x_step': (ranges[0][1] - ranges[0][0]) / 10,
                           'x_format': xformat
                          })

  def handleurl_dashboard(self, urlpath, post_data):
    """A simple dashboard."""

    global sensor_names
    
    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)
    cursor2 = db.cursor(MySQLdb.cursors.DictCursor)

    now = []
    cursor.execute('select distinct(sensor), hostname from sensors;')
    for row in cursor:
      cursor2.execute('select * from sensors where sensor="%s" and hostname="%s" '
                      'and epoch_seconds > %d '
                      'order by epoch_seconds desc limit 1;'
                      %(row['sensor'], row['hostname'],
                        time.time() - (24 * 60 * 60)))
      for row2 in cursor2:
        t = datetime.datetime.fromtimestamp(row2['epoch_seconds'])
        now.append('<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>'
                   %(sensor_names.get(row2['sensor'], row2['sensor']),
                     row2['hostname'], row2['value'], t))

    self.sendfile('dashboard.html', subst={'now': ('<table>%s</table>'
                                                   % '\n'.join(now))})

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

    global sensor_names

    if sensor.startswith('='):
      plugin = mplugin.LoadPlugin(PLUGIN_DIR, sensor[1:], log=self.log)
      if not plugin:
        self.log('No plugin matching %s' % sensor[1:])
        return values

      db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
      cursor = db.cursor(MySQLdb.cursors.DictCursor)

      out = plugin.Returns(cursor)
      self.log('Resolving %s would return %s' %(sensor, repr(out)))
      return out

    else:
      sensors = [sensor]
      for s in sensor_names:
        if sensor_names[s] == sensor:
          sensors.append(s)
      
      hostnames = []
      cursor.execute('select distinct(hostname) from sensors where '
                     'sensor in ("%s") and epoch_seconds > %s and '
                     'epoch_seconds < %s;'
                     %('", "'.join(sensors), start_epoch - 1, end_epoch + 1))
      for row in cursor:
        hostnames.append(row['hostname'])

      if len(hostnames) == 1:
        self.log('Resolving %s would return %s' %(sensors, sensor))
        return [sensor]

      out = []
      for hostname in hostnames:
        out.append('%s (%s)' %(sensor, hostname))
      self.log('Resolving %s would return %s' %(sensor, repr(out)))
      return out

  def ResolveSensor(self, values, redirects, cursor, sensor, start_epoch,
                    end_epoch, step_size, wideinterp):
    """Resolve a time series of this sensor value. This could be recursive."""

    self.log('Values for %s between %s and %s' %(sensor, start_epoch,
                                                     end_epoch))

    plugin = None
    plugin_is_function = False
    if sensor.startswith('='):
      try:
        plugin = mplugin.LoadPlugin(PLUGIN_DIR, sensor[1:], log=self.log)
      except:
        pass

      if not plugin:
        try:
          plugin = mplugin.LoadPlugin(FUNCTION_DIR, sensor[1:].split('(')[0],
                                      log=self.log)
        except:
          pass
        plugin_is_function = True

      if not plugin:
        self.log('No plugin matching %s' % sensor[1:])
        return values

      db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
      cursor = db.cursor(MySQLdb.cursors.DictCursor)
      
      # Functions have explicit inputs
      if not plugin_is_function:
        inputs = plugin.Requires(cursor, sensor_names)
      else:
        inputs = sensor[1:].split('(')[1].split(')')[0].split(',')
      self.log('Calculation requires: %s' % repr(inputs))

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
      if not plugin_is_function:
        values[sensor] = plugin.Calculate(input_values, redirects,
                                          step_size=step_size, log=self.log)
      else:
        values[sensor] = plugin.Calculate(inputs, input_values, redirects,
                                          step_size=step_size, log=self.log)

    else:
      sensors = self.ExpandSensorName(sensor)
      
      hostnames = []
      cursor.execute('select distinct(hostname) from sensors where '
                     'sensor in ("%s") and epoch_seconds > %s and '
                     'epoch_seconds < %s;'
                     %('", "'.join(sensors), start_epoch - 1, end_epoch + 1))
      for row in cursor:
        hostnames.append(row['hostname'])
      self.log('Found hostnames: %s' % repr(hostnames))

      for hostname in hostnames:
        if len(hostnames) == 1:
          name = sensor
        else:
          name = '%s (%s)' %(sensor, hostname)
          redirects.setdefault(sensor, [])
          redirects[sensor].append(name)

        readings = self.GetData(cursor, sensors, hostname,
                                start_epoch, end_epoch)
        values[name] = self.GetChartPoints(start_epoch, end_epoch,
                                           step_size, readings, sensor,
                                           wideinterp=wideinterp)
        self.log('Found %d chart points for %s' %(len(values[name]), name))

    self.log('Resolved values: %s and redirects: %s' %(repr(values),
                                                       repr(redirects)))
    return (values, redirects)

  def ExpandSensorName(self, sensor):
    """Turn a sensor name into all possible sensor entries."""

    global sensor_names

    target_sensors = [sensor]
    for s in sensor_names:
      if sensor_names[s] == sensor:
        target_sensors.append(s)

    return target_sensors

  def GetData(self, cursor, sensors, hostname, start_epoch, end_epoch):
    """Grab the readings from the DB."""

    readings = {}
    sql = ('select epoch_seconds, value from sensors where '
           'sensor in ("%s") and hostname="%s" and epoch_seconds > %d and '
           'epoch_seconds < %d order by epoch_seconds;'
           %('", "'.join(sensors), hostname, start_epoch - 1, end_epoch + 1))
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

    wiggle = [0]
    if not wideinterp:
      backwards = range(-150, 0, 1)
      forwards = range(150, 0, -1)
    elif sensor.startswith('BOM '):
      backwards = range(-1800, 0, 1)
      forwards = range(1800, 0, -1)
    else:
      backwards = range(-(step_size / 2), 0, 1)
      forwards = range(step_size / 2, 0, -1)

    while backwards:
      wiggle.append(backwards.pop())
      wiggle.append(forwards.pop())
    wiggle.append(None)

    values = []
    offsets = []

    if step_size < 1:
      step_size = 1

    for t in range(int(start_epoch), int(end_epoch), step_size):
      for offset in wiggle:
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

  def time_link(self, urlpath, time_arg, first_time_tuple, delta, link):
    first_time_arg = time_arg.split(';')[0]
    self.log('Convert %s in %s (%s) to offset time'
             %(repr(first_time_tuple), first_time_arg, urlpath))
    first_time = datetime.datetime(int(first_time_tuple[2][0]),
                                   int(first_time_tuple[2][1]),
                                   int(first_time_tuple[2][2]))
    first_time += delta
    new_url = urlpath.replace(first_time_arg, first_time.strftime('%Y.%m.%d'))
    return '<a href="%s">%s</a>' %(new_url, link)


def FetchSensorNames():
  """Update the cache of sensor names."""

  print '%s: Updating sensor name cache' % datetime.datetime.now()
  db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
  cursor = db.cursor(MySQLdb.cursors.DictCursor)

  cursor.execute('select * from sensor_names;')
  out = {}
  for row in cursor:
    out[row['sensor']] = row['name']
    print '%s: %s is called %s' %(datetime.datetime.now(), row['sensor'],
                                  row['name'])
  return out


def InitializeCleanup():
  """Ensure that all sensor names are covered by the cleanup table."""

  print '%s: Preparing to cleanup old data' % datetime.datetime.now()
  db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  cursor2 = db.cursor(MySQLdb.cursors.DictCursor)

  # This query is slow, so don't do it too often
  cursor.execute('select distinct(sensor), hostname from sensors;')
  for row in cursor:
    cursor2.execute('select * from cleanup where sensor="%s" and hostname="%s";'
                    %(row['sensor'], row['hostname']))
    if cursor2.rowcount > 0:
      continue
    
    print ('%s: Finding the minimum entry for %s at %s'
           %(datetime.datetime.now(), row['sensor'], row['hostname']))
    cursor2.execute('select min(epoch_seconds) from sensors where sensor="%s" '
                    'and hostname="%s";'
                    %(row['sensor'], row['hostname']))
    row2 = cursor2.fetchone()
    print '%s: --> %s' %(datetime.datetime.now(), repr(row2))
    start_seconds = row2['min(epoch_seconds)']
    cursor2.execute('insert ignore into cleanup(sensor, hostname, upto) values '
                    '("%s", "%s", %s);'
                    %(row['sensor'], row['hostname'], start_seconds))
    if cursor2.rowcount > 0:
      print ('%s Initialized cleanup for %s at %s to %s'
             %(datetime.datetime.now(), row['sensor'], row['hostname'],
               start_seconds))
    cursor2.execute('commit;')


def Cleanup():
  """Perform regular down sampling."""

  cleaned = False
  db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  cursor2 = db.cursor(MySQLdb.cursors.DictCursor)

  # Downsample data older than a week to being within one minute accuracy
  cursor.execute('select * from cleanup where upto < %d '
                 'order by upto desc limit 1;'
                 % (time.time() - (3 * 24 * 60 * 60)))
  for row in cursor:
    t = datetime.datetime.fromtimestamp(row['upto'])
    print ('%s: Cleaning %s at %s (from %s)'
           %(datetime.datetime.now(), row['sensor'], row['hostname'], t))

    cursor2.execute('select * from sensors where sensor="%s" and hostname="%s" and '
                    'epoch_seconds > %d and epoch_seconds < %d '
                    'order by epoch_seconds desc;'
                    %(row['sensor'], row['hostname'], row['upto'] - 1,
                      row['upto'] + 301))
    timestamps = []
    for row2 in cursor2:
      if not timestamps:
        print '%s: **> %s' %(datetime.datetime.now(), repr(row2))
      else:
        print '%s: --> %s' %(datetime.datetime.now(), repr(row2))
      timestamps.append(str(row2['epoch_seconds']))

    if not timestamps:
      # We have found a gap in the data, skip forward
      cursor2.execute('select min(epoch_seconds) from sensors '
                      'where sensor="%s" and hostname="%s" and epoch_seconds > %s;'
                      %(row['sensor'], row['hostname'], row['upto']))
      new_upto = cursor2.fetchone()['min(epoch_seconds)']

      if not new_upto:
        print '%s: No more data for %s at %s' %(datetime.datetime.now(),
                                                row['sensor'], row['hostname'])
        new_upto = time.time()

      print ('%s: Found a gap for %s at %s, skipping from %s to %s'
             %(datetime.datetime.now(), row['sensor'], row['hostname'], row['upto'],
               new_upto))
      cursor2.execute('update cleanup set upto=%s where sensor="%s" and '
                      'hostname="%s";'
                      %(new_upto, row['sensor'], row['hostname']))
      cursor2.execute('commit;')
      cleaned = True

    timestamps = timestamps[1:]
    print '%s: Remove %s' %(datetime.datetime.now(), repr(timestamps))

    if timestamps:
      cursor2.execute('delete from sensors where sensor="%s" and hostname="%s" and '
                      'epoch_seconds in (%s);'
                      %(row['sensor'], row['hostname'], ', '.join(timestamps)))
      print '%s: %d rows removed' %(datetime.datetime.now(),
                                    cursor2.rowcount)
      cursor2.execute('commit;')

    cursor2.execute('update cleanup set upto=upto + 300 '
                    'where sensor="%s" and hostname="%s";'
                    %(row['sensor'], row['hostname']))
    cursor2.execute('commit;')
    cleaned  = True
    
  return cleaned


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

  sensor_names = FetchSensorNames()
  sensor_names_age = time.time()
  server = http_server(FLAGS.ip, FLAGS.port)

  # Start the web server, which takes over this thread
  print '%s: Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  timeout = 0.2
  last_summary = time.time()
  while running:
    last_event = time.time()
    asyncore.loop(timeout=timeout, count=1)

    if time.time() - last_event > timeout:
      # We are idle
      print '%s ...' % datetime.datetime.now()

      db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
      cursor = db.cursor(MySQLdb.cursors.DictCursor)
      for ent in os.listdir(DAILY_DIR):
        if ent.endswith('.py'):
          print '%s: Updating daily %s' %(datetime.datetime.now(), ent)
          try:
            plugin = mplugin.LoadPlugin(DAILY_DIR, ent[:-3], log=None)
            plugin.Daily(cursor)
          except Exception, e:
            print '%s: Exception: %s' %(datetime.datetime.now(), e)

      if time.time() - sensor_names_age > 1800:
        sensor_names = FetchSensorNames()
        InitializeCleanup()
        sensor_names_age = time.time()

      else:
        if Cleanup():
          timeout = 0.2
        else:
          timeout = 10

    if time.time() - last_summary > 60.0:
      print '%s TOTAL BYTES SERVED: %s' %(datetime.datetime.now(),
                                          mhttp.DisplayFriendlySize(bytes))
      last_summary = time.time()


if __name__ == "__main__":
  main(sys.argv)
