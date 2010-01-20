#!/usr/bin/python

# Compare the BOM Isabella Plains humidity with our calculated humidity

def Requires(unused_cursor, unused_sensor_names):
  return ['BOM Relative humidity %', '=Outside humidity']


def Calculate(inputs, log=None):
  """Determine delta."""

  out = []
    
  while inputs['BOM Relative humidity %']:
    if (inputs['BOM Relative humidity %'][0] and
        inputs['=Outside humidity'][0]):
      bom = int(inputs['BOM Relative humidity %'][0])

      if log:
        log('Considering local humidity reading of: %s'
            % inputs['=Outside humidity'][0])

      try:
        us = int(inputs['=Outside humidity'][0])
      except Exception, e:
        if log:
          log('Int parse error: %s' % e)
        us = None

      if log:
        log('Comparing %s and %s' %(bom, us))

      if us:
        out.append(bom - us)
      else:
        out.append(None)

    else:
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
