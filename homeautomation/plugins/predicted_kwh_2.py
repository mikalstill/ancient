#!/usr/bin/python

# Predict kWh Solar output based on BOM solar exposure numbers
#
# The BOM Solar reading is MJ per square meter over a day, and I have
# approximately 15 square meters of panels installed. 3.6 MegaJoules
# is equivalent of 1 kWh.

import copy

def Requires(cursor, sensor_names):
  """Required inputs."""
  return ['BOM Solar','=Generated kWh']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Predicted kWh 2']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Try to predict power output."""

  out = []
  while inputs['BOM Solar']:
    if inputs['BOM Solar'][0]:
      out.append(inputs['BOM Solar'][0] * 15.319 / 3.6 * 0.135)

    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
