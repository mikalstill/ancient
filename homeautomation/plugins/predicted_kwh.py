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
  return ['=Predicted kWh']


def average(l):
  """Find the average of a list."""

  t = 0
  for i in l:
    t += i
  return t / len(l)


def mrange(start, end, step):
  """Work through a range with smaller steps than xrange."""
  i = start
  while i < end:
    yield i
    i += step


def storefloat(f):
  """Turn a float into something with reliable precision."""

  # Yes, python will store 12.03 as 12.03000000001. That screws us up
  return '%.01f' % f


def Calculate(inputs, redirects, step_size=None, log=None):
  """Try to predict power output."""

  history = {}
  min_ever = '100000.0'
  inputs_copy = copy.copy(inputs)
  while inputs_copy['BOM Solar']:
    if inputs_copy['BOM Solar'][0] and inputs_copy['=Generated kWh'][0]:
      v = storefloat(inputs_copy['BOM Solar'][0])
      if float(v) < float(min_ever):
        min_ever = v

      history.setdefault(v, [])
      history[v].append(inputs_copy['=Generated kWh'][0])
      print 'History: %s = %s' %(inputs_copy['BOM Solar'][0],
                                 inputs_copy['=Generated kWh'][0])

    for input in inputs_copy:
      inputs_copy[input] = inputs_copy[input][1:]

  print 'Generation history: %s' % repr(history)

  out = []
  while inputs['BOM Solar']:
    if inputs['BOM Solar'][0]:
      v = storefloat(inputs['BOM Solar'][0])
      if v in history:
        out.append(average(history[v]))

      else:
        l = []
        window = 0.1
        while not l and (inputs['BOM Solar'][0] - window) > 0:
          print 'Lookup for %.02f with window %02f' %(inputs['BOM Solar'][0],
                                                      window)
          for val in mrange(inputs['BOM Solar'][0] - window,
                            inputs['BOM Solar'][0],
                            0.01):
            if storefloat(val) in history:
              for i in history[storefloat(val)]:
                l.append(i)

          window += 1

        if not l:
          for val in history[min_ever]:
            ratio = float(min_ever) / float(inputs['BOM Solar'][0])
            l.append(val * ratio)

        if not l:
          out.append(None)
        else:
          out.append(average(l))

    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
