#!/usr/bin/python

# Munge the watts reading into something which works on the graph scale, and
# provide a SAMPLE_SIZE sample rolling average

SAMPLE_SIZE = 10

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['Watts']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Watts average']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Determine SAMPLE_SIZE sample rolling average"""

  out = []
  previous = [None for _ in range(SAMPLE_SIZE - 1)]
  while inputs['Watts']:
    previous.append(inputs['Watts'][0])
    out.append(CalculateAverage(previous))
    previous = previous[1:]

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out[SAMPLE_SIZE / 2:]


def CalculateAverage(input):
  count = 0
  sum = 0

  for i in input:
    if i:
      count += 1
      sum += float(i)

  if count == 0:
    return None
  return sum / count / 50.0
