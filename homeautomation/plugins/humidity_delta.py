#!/usr/bin/python

# Work out how much more humid it is in the house compared with outside

def Requires(unused_cursor, unused_sensor_names):
  """What inputs are required?"""
  return ['=Inside humidity', '=Outside humidity']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Humidity delta']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Determine delta."""

  out = []
    
  while inputs['=Inside humidity']:
    if inputs['=Inside humidity'][0] and inputs['=Outside humidity'][0]:
      try:
        inside = int(inputs['=Inside humidity'][0])
        outside = int(inputs['=Outside humidity'][0])
        out.append(inside - outside)
        
      except Exception, e:
        if log:
          log('Int parse error: %s' % e)
        out.append(None)

    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
