#!/usr/bin/python

# How much did we generate?

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['kWh']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Generated kWh']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Try to predict power output."""

  out = []
  while inputs['kWh']:
    try:
      out.append(float(inputs['kWh'][1]) - float(inputs['kWh'][0]))

    except:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
