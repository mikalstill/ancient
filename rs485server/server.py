#!/usr/bin/python

import datetime
import serial
import sys
import time

MAX_CLIENTS = 20
CLIENT_PING_DELAY = 1.0

s = serial.Serial(rtscts=True, xonxoff=False)
s.port = sys.argv[1]
s.baudrate = 115200
s.open()

time.sleep(1)
if s.inWaiting() > 0:
  s.read()

clients = {}
client_type = {}
client_cmd = {}

upto = 1
last_ping = 0
last_data = 0

def SendMessage(message, client):
  global client_cmd
  global last_data

  msg = '%s:000:%s\r\n' %(client, message)

  s.setRTS(True)
  cts = s.getCTS()
  while(not cts):
    print '%s: Not cleared to send (%s, %s)' %(datetime.datetime.now(),
                                               s.rtscts, cts)
    cts = s.getCTS()
    time.sleep(0.3)

  s.write(msg)
  s.setRTS(False)

  sys.stdout.write('%s: %s' %(datetime.datetime.now(), msg))
  sys.stdout.flush()
  client_cmd[client] = message
  last_data = time.time()

def QuerySilentClient():
  global clients
  global upto
  global last_ping

  for client in clients:
    if time.time() - clients[client] > 30:
      return client

    if time.time() - clients[client] > 10:
      SendMessage('Values', client)
      return

  if time.time() - last_ping > CLIENT_PING_DELAY:
    if upto > MAX_CLIENTS:
      upto = 1

    client_name = '%03d' % upto
    upto += 1
    if not client_name in clients:
      SendMessage('Hello', client_name)
      last_ping = time.time()
   

# main
input = ''

while True:
  if s.inWaiting() > 0:
    input += s.read()
    if input.endswith('\n'):
      sys.stdout.write('%s: %s' %(datetime.datetime.now(), input))
      sys.stdout.flush()

      elems = input.split(':')
      t = elems[0]
      f = elems[1]
      d = ':'.join(elems[2:])
      clients[f] = time.time()

      if not f in client_cmd:
        print '%s: UNSOLICITED DATA' % datetime.datetime.now()
      elif client_cmd[f] == 'Hello':
        client_type[f] = d

      input = ''
      last_data = time.time()

  if time.time() - last_data > 1.0:
    remove = QuerySilentClient()
    if remove:
      print '%s: Removing silent client %s' %(datetime.datetime.now(), remove)
      del clients[remove]
