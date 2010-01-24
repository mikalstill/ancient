#!/usr/bin/python

# There is more than one relative humidity value... Grab the most reasonable.

def Requires(unused_cursor, unused_sensor_names):
  """What inputs are required?"""
  return ['BOM Relative humidity %']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=BOM Relative humidity %']


def Calculate(inputs, redirects, log=None):
  """Determine delta."""

  out = []
  names = redirects.get('BOM Relative humidity %', ['BOM Relative humidity %'])
    
  while inputs['BOM Relative humidity %']:
    l = []
    non_null = 0
    for name in names:
      l.append(inputs[name][0])
      if not inputs[name][0] is None:
        non_null += 1

    if non_null > 0:
      out.append(repr(l))
    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
