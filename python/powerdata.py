#!/usr/bin/env python

# Simple script that just spits out raw data from the CC128
#
# Format is
# time power temp
#
# time is in seconds since epoch
#
import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import time
import pycurrentcost

cc = pycurrentcost.CurrentCostReader(port="/dev/ttyUSB2")

reading = cc.get_reading()

now = int(time.time())

print '%s %s %s' % (now, reading.channels[1]['watts'], reading.temperature)
