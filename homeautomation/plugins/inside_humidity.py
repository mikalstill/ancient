#!/usr/bin/python

# Calculate the inside humdity based on known inputs. Note this is the same
# code as the outside humidity, except for the humidity and temperature
# sources. One day I should work out inheritance for plugins.

INSIDE_TEMP = 'Inside lounge TV'
INSIDE_HS1101 = 'HS1101 cycles'

def Requires(unused_cursor, unused_sensor_names):
  """What inputs are required?"""
  return [INSIDE_HS1101, INSIDE_TEMP]


def Returns(unused_cursor):
  """What values would be returned?"""
  return ['=Inside humidity']


def Calculate(inputs, redirects, step_size=None, log=None):
  """Calculated humidity."""

  out = []

  while inputs[INSIDE_HS1101]:
    if inputs[INSIDE_HS1101][0] and inputs[INSIDE_TEMP][0]:
      raw = (557.1 -
             (int(inputs[INSIDE_HS1101][0]) * 0.0758) + 16)
      temp = inputs[INSIDE_TEMP][0]
      corr = (1.0 + (0.001 * (temp - 25))) * raw

      #if log:
      #  log('Cycles = %s, raw = %.02f, temp = %.02f, corr = %.02f'
      #      %(inputs['HS1101 cycles'][0], raw, temp, corr))

      out.append(corr)

    else:                  
      out.append(None)

    for input in inputs:
      inputs[input] = inputs[input][1:]

  return out
