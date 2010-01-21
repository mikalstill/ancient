#!/usr/bin/python

# Calculate the outside humdity based on known inputs

OUTSIDE_TEMP = '=Outside min'

def Requires(unused_cursor, unused_sensor_names):
  return ['HS1101 cycles', OUTSIDE_TEMP]


def Calculate(inputs, log=None):
  """Calculated humidity."""

  out = []
    
  while inputs['HS1101 cycles']:
    if inputs['HS1101 cycles'][0] and inputs[OUTSIDE_TEMP][0]:
      raw = 557.1 - (int(inputs['HS1101 cycles'][0]) * 0.0758) + 16
      temp = inputs[OUTSIDE_TEMP][0]
      corr = (1.0 + (0.001 * (temp - 25))) * raw
      if log:
        log('Cycles = %s, raw = %.02f, temp = %.02f, corr = %.02f'
            %(inputs['HS1101 cycles'][0], raw, temp, corr))
      out.append(corr)

    else:                  
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
