#!/usr/bin/python

import datetime
import serial
import sys
import time

s = serial.Serial(rtscts=True, xonxoff=False)
s.port = sys.argv[1]
s.baudrate = 115200
s.open()

input = ''
while True:
  if s.inWaiting() > 0:
    input += s.read()
    if input.endswith('\n'):
      if len(input) > 2:
        sys.stdout.write('%s: %s' %(datetime.datetime.now(), input))
        sys.stdout.flush()
      input = ''
