#!/usr/bin/python

# Web interface to my MP3 database

import asyncore
import cStringIO
import datetime
import socket
import sys
import time
import thread
import threading
import traceback
import types

import MySQLdb

import database
import gflags
import track


FLAGS = gflags.FLAGS
gflags.DEFINE_string('ip', '0.0.0.0', 'Bind to this IP')
gflags.DEFINE_integer('port', 12345, 'Bind to this port')


running = True

# Read in a default HTML header
h = open('header.html')
_HEADER = ''.join(h.readlines())
h.close()


class http_server(asyncore.dispatcher):
  """Listen for new client connections, which are then handed off to
     another class
  """

  def __init__(self, ip, port, db):
    self.ip= ip
    self.port = port
    self.db = db
    
    asyncore.dispatcher.__init__(self)
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.bind((ip, port))
    self.listen(5)

  def writable(self):
    return 0

  def handle_read(self):
    pass

  def readable(self):
    return self.accepting

  def handle_connect(self):
    pass

  def handle_accept(self):
    conn, addr = self.accept()
    print '%s %s New connection' %(datetime.datetime.now(), repr(addr))
    handler = http_handler(conn, addr, self.db)


class http_handler(asyncore.dispatcher):
  """Handle a single connection"""

  def __init__(self, conn, addr, db):
    asyncore.dispatcher.__init__(self, sock=conn)
    self.addr = addr
    self.db = db

    self.buffer = ''

  def handle_read(self):
    global cstart
    global history
    global persistant
    
    rq = self.recv(1024)
    file = ''
    for line in rq.split('\n'):
      line = line.rstrip('\r')

      if line.startswith('GET'):
        (_, file, _) = line.split(' ')

    print '%s GET %s' %(datetime.datetime.now(), file)
    if file == '/':
      self.buffer += ('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n'
                      '%s' % _HEADER)

      for row in self.db.GetRows('select paths from tracks '
                                 'where paths <> "[]" '
                                 'order by rand() limit 1;'):
        paths = eval(row['paths'])
        this_track = track.Track(self.db)
        this_track.FromPath(paths[0])
        self.buffer += '%s' % this_track.RenderHtml()

      self.buffer += '</body></html'

    elif file.startswith('/mp3/'):
      self.buffer += 'HTTP/1.1 200 OK\r\nContent-Type: audio/x-mpeg-3\r\n\r\n'
      try:
        f = open('%s%s' %(FLAGS.audio_path, file))
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s MP3 serving error: %s %s' %(datetime.datetime.now(),
                                              file, e)

    elif file.startswith('/local/player'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: application/x-shockwave-flash \r\n\r\n')
      try:
        f = open('player_mp3_js.swf')
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s player serving error: %s' %(datetime.datetime.now(), e)

    elif file.startswith('/local/style.css'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: text/css \r\n\r\n')
      try:
        f = open('style.css')
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s css serving error: %s' %(datetime.datetime.now(), e)

    else:
      self.buffer += ('HTTP/1.1 404 OK\r\nContent-Type: text/html\r\n\r\n'
                      '<html><head><title>MP3 server</title</head>'
                      '<body>File not found</body>')

  def writable(self):
    return len(self.buffer) > 0

  def handle_write(self):
    sent = self.send(self.buffer)
    self.buffer = self.buffer[sent:]
    if len(self.buffer) == 0:
      self.close()

  def handle_close(self):
    pass


def main(argv):
  global running
  
  # Parse flags
  try:
    argv = FLAGS(argv)
  except gflags.FlagsError, e:
    print FLAGS

  db = database.Database()
  server = http_server(FLAGS.ip, FLAGS.port, db)

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  while running:
    asyncore.loop(timeout=10.0, count=1)


if __name__ == "__main__":
  main(sys.argv)
