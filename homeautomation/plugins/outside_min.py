#!/usr/bin/python

# What was the minimum outside temperature reported?

def Requires(cursor, sensor_names):
  """Lookup all outside temperature sensors."""

  outside_sensors = []
  for k in sensor_names:
    if sensor_names[k].startswith('Outside '):
      outside_sensors.append(k)

  out = []
  sql = ('select distinct(sensor) from sensors where '
         'sensor like "Outside %%" or sensor in ("%s");'
         % '", "'.join(outside_sensors))

  cursor.execute(sql)
  for row in cursor:
    out.append(sensor_names.get(row['sensor'], row['sensor']))

  return out


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Outside min']


UNDEFINED = 99999999.0
def Calculate(inputs, redirects, log=None):
  """Determine minimum."""

  out = []
  if log:
    log('Using inputs: %s' % repr(inputs.keys()))
  key = inputs.keys()[0]

  while inputs[key]:
    m = UNDEFINED
    for k in inputs:
      if inputs[k][0]:
        try:
          m = min(m, float(inputs[k][0]))
        except:
          pass

    if m == UNDEFINED:
      out.append(None)
    else:
      out.append(m)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
