#!/usr/bin/python

# Munge the watts reading into something which works on the graph scale

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['Power AC']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Power AC']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Munge values to fit graph scale."""

  out = []
  while inputs['Power AC']:
    if inputs['Power AC'][0]:
      out.append(float(inputs['Power AC'][0]) / 50)
    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
