#!/usr/bin/python

# Calculate cumulative kilo watt hours

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['Watts']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=WattHours']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Count kWh."""

  out = []
  total = 0.0

  while inputs['Watts']:
    if inputs['Watts'][0]:
      total += float(inputs['Watts'][0]) * step_size / 3600 / 1000

    out.append(total)
    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
