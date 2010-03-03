#!/usr/bin/python

# Compare the closest BOM value with our value

def Requires(unused_cursor, unused_sensor_names):
  """What inputs are required?"""
  return ['=BOM Relative humidity %', '=Outside humidity']


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=BOM delta']


UNDEFINED = 999999999
def Calculate(inputs, redirects, step_size=None, log=None):
  """Determine delta."""

  out = []
    
  while inputs['=BOM Relative humidity %']:
    bom_delta = UNDEFINED
    if (inputs['=BOM Relative humidity %'][0] and
        inputs['=Outside humidity'][0]):
      try:
        us = int(inputs['=Outside humidity'][0])

        bom_options = eval(inputs['=BOM Relative humidity %'][0])
        for bom in bom_options:
          try:
            if abs(us - bom) < bom_delta:
              bom_delta = abs(us - bom)

          except Exception, e:
            if log:
              log('Comparison error: %s' % e)
        
      except Exception, e:
        if log:
          log('Int parse error: %s' % e)

    if bom_delta == UNDEFINED:
      out.append(None)
    else:
      out.append(bom_delta)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
