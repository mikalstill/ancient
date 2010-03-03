#!/usr/bin/python

# What was the minimum inside temperature reported?

def Requires(cursor, sensor_names):
  """Lookup all inside temperature sensors."""

  inside_sensors = []
  for k in sensor_names:
    if sensor_names[k].startswith('Inside '):
      inside_sensors.append(k)

  out = []
  sql = ('select distinct(sensor) from sensors where '
         'sensor like "Inside %%" or sensor in ("%s");'
         % '", "'.join(inside_sensors))

  cursor.execute(sql)
  for row in cursor:
    out.append(sensor_names.get(row['sensor'], row['sensor']))

  return out


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Inside min']


UNDEFINED = 99999999.0
def Calculate(inputs, redirects, step_size=None, log=None):
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
