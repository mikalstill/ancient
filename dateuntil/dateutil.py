#!/usr/bin/python

import datetime
import sys

dt = datetime.datetime(int(sys.argv[1][0:4]), int(sys.argv[1][4:6]), int(sys.argv[1][6:8]))
delta = dt - datetime.datetime.now()

print '%d days until %s' %(delta.days, sys.argv[2])
