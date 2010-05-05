#!/usr/bin/python

import serial
import sys
import time

s = serial.Serial()
s.port = sys.argv[1]
s.baudrate = 38400
s.open()

class TeeFile(object):
  def open(self, fname, mode):
    self.f = open(fname, mode)

  def write(self, string):
    self.f.write(string)
    self.f.flush()
    sys.stdout.write(string)
    sys.stdout.flush()

  def close(self):
    self.f.close()

f = TeeFile()
f.open('log.%s.txt' % time.time(), 'w')

# Reset the device
s.write('ATZ\r\n')
f.write(s.read().replace('\r', '\n'))
time.sleep(1)

# Set the display mode to include headers
s.write('ATH1\r\n')
f.write(s.read().replace('\r', '\n'))

# I _think_ I want 125kbaud CAN
s.write('AT SP B\r\n')
f.write(s.read().replace('\r', '\n'))

# Start monitoring activity in a big loop
s.write('ATMA\r\n')
l = ''
while True:
  v = s.read().replace('\r', '\n')
  f.write(v)

  if l.startswith('BUFFER') or l.startswith('?'):
    s.write('\nATMA\r\n')
    l= ''

  if v != '\n':
    l += v
  else:
    l = ''
