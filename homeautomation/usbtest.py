#!/usr/bin/python

# Determine what this USB serial device is by trying to talk to it

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import pycurrentcost

def test():
  # Is this a currentcost?
  try:
    cc = pycurrentcost.CurrentCostReader(port=sys.argv[1])
    reading = cc.get_reading()
    if int(reading.channels[1]['watts']) > -1:
      print 'currentcost'
      return

  except:
    pass

  # Then this must be the inverter
  print 'inverter'

test()
