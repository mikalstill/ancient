#!/usr/bin/python

# Munge the CPU fan RPM numbers from molokai into something which works on the graph

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['fan1']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=CPU Fan']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Munge values to fit graph scale."""

  out = []
  while inputs['fan1']:
    if inputs['fan1'][0]:
      out.append(float(inputs['fan1'][0]) / 50)
    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
