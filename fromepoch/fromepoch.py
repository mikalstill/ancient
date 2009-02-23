#!/usr/bin/python

import datetime
import sys

l = sys.stdin.readline()
while l:
  l = l.rstrip()
  t = l.split(' ')[0]
  print '%s %s' %(datetime.datetime.fromtimestamp(float(t)),
                  ' '.join(l.split(' ')[1:]))
  l = sys.stdin.readline()
