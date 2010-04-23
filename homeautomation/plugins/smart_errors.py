#!/usr/bin/python

# What is the sum of SMART errors reported?

REQUIRED_SENSORS = []
def Requires(cursor, sensor_names):
  """Lookup all SMART error counts."""

  sensors = []
  for k in sensor_names:
    if sensor_names[k].startswith('SMART errors '):
      sensors.append(k)

  out = []
  sql = ('select distinct(sensor) from sensors where '
         'sensor like "SMART errors %%" or sensor in ("%s");'
         % '", "'.join(sensors))

  cursor.execute(sql)
  for row in cursor:
    out.append(sensor_names.get(row['sensor'], row['sensor']))

  REQUIRED_SENSORS = out
  return out


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=SMART errors']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Determine sum."""

  out = []
  if log:
    log('Using inputs: %s' % repr(inputs.keys()))
  key = inputs.keys()[0]

  while inputs[key]:
    m = 0.0
    data_seen = False
    for k in REQUIRED_SENSORS:
      if inputs[k][0] is not None:
        try:
          m += float(inputs[k][0])
          data_seen = True

        except:
          pass

    if not data_seen:
      out.append(None)
    else:
      out.append(m)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
