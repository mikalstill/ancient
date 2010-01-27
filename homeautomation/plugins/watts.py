#!/usr/bin/python

# Munge the watts reading into something which works on the graph scale

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['Watts']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Watts']


def Calculate(inputs, redirects, log=None):
  """Determine minimum."""

  out = []
  while inputs['Watts']:
    if inputs['Watts'][0]:
      out.append(float(inputs['Watts'][0]) / 50)
    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
