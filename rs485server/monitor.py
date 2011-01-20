#!/usr/bin/python

import datetime
import serial
import string
import sys
import time

s = serial.Serial(rtscts=True, xonxoff=False)
s.port = sys.argv[1]
s.baudrate = 9600 
s.open()

input = ''
sys.stdout.write('>> ')

while True:
  if s.inWaiting() > 0:
    d = s.read()
    input += d

    for c in d:
      if c in string.printable and not c in ['\r', '\n']:
        sys.stdout.write('%d (%s) ' %(ord(c), c))
      else:
        sys.stdout.write('%d ' % ord(c))
    sys.stdout.flush()

    if input.endswith('\n'):
      if len(input) > 2:
        sys.stdout.write('\n%s: %s' %(datetime.datetime.now(), input))
        sys.stdout.write('>> ')
        sys.stdout.flush()
      input = ''
