#!/usr/bin/python

# Web interface to my MP3 database

import asyncore
import cStringIO
import datetime
import os
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
h = open('template.html')
_TEMPLATE = ''.join(h.readlines())
h.close()


requests = {}


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

    if file:
      print '%s %s GET %s' %(datetime.datetime.now(), repr(self.addr),
                             file)
    if file == '/':
      self.buffer += 'HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n'

      for row in self.db.GetRows('select paths from tracks '
                                 'where paths <> "[]" '
                                 'order by rand() limit 10;'):
        paths = eval(row['paths'])
        this_track = track.Track(self.db)
        this_track.FromPath(paths[0])
        rendered = this_track.RenderValues()

        if os.path.exists(rendered['mp3_path']):
          if self.addr[0] in requests:
            print '%s %s Marking %s skipped' %(datetime.datetime.now(),
                                               repr(self.addr),
                                               requests[self.addr[0]])
            self.db.ExecuteSql('update tracks set skips = skips + 1, '
                               'last_skipped = NOW(), last_action = NOW() '
                               'where id=%s;'
                               % requests[self.addr[0]])
            self.db.ExecuteSql('commit;')

            del requests[self.addr[0]]

          requests[self.addr[0]] = rendered['id']

          play_graph = {}
          skip_graph = {}
          for row in self.db.GetRows('select song, plays, skips, last_action, '
                                     '(time_to_sec(now()) - '
                                     ' time_to_sec(last_played)) as lp, '
                                     '(time_to_sec(now()) - '
                                     ' time_to_sec(last_skipped)) as ls, '
                                     '(time_to_sec(now()) - '
                                     ' time_to_sec(last_action)) as secs '
                                     'from tracks '
                                     'where last_action is not null and '
                                     '(time_to_sec(now()) - '
                                     ' time_to_sec(last_action)) < 3600 '
                                     'order by last_action asc;'):

            if row['lp']:
              play_graph.setdefault(int(row['lp'] / 60), 0)
              play_graph[int(row['lp'] / 60)] += 1

            if row['ls']:
              skip_graph.setdefault(int(row['ls'] / 60), 0)
              skip_graph[int(row['ls'] / 60)] += 1

          play = ''
          skip = ''
          for i in range(60):
            play += '%d,' % play_graph.get(i, 0)
            skip += '%d,' % skip_graph.get(i, 0)
          
          rendered['graph'] = ('http://chart.apis.google.com/chart?cht=bvg&'
                               'chbh=a&chds=0,10,0,10&chs=800x125&chd=t:%s|%s&'
                               'chco=00FF00,FF0000'
                               %(play[:-1], skip[:-1]))

          
          
          self.buffer += _TEMPLATE % rendered
          break

        else:
          print '%s %s Missing MP3 %s' %(datetime.datetime.now(),
                                         repr(self.addr), paths[0])
          if self.addr[0] in requests:
            del requests[self.addr[0]]

    elif file.startswith('/mp3/'):
      self.buffer += 'HTTP/1.1 200 OK\r\nContent-Type: audio/x-mpeg-3\r\n\r\n'
      for row in self.db.GetRows('select paths from tracks '
                                 'where id=%s;'
                                 % file[5:]):
        paths = eval(row['paths'])
        for path in paths:
          if path.endswith('.mp3') and os.path.exists(path):
            print '%s %s Serving %s' %(datetime.datetime.now(),
                                       repr(self.addr), path)
            f = open(path)
            self.buffer += f.read(1000000000)
            f.close()
            break

    elif file.startswith('/local/player'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: application/x-shockwave-flash \r\n\r\n')
      try:
        f = open('player_mp3_js.swf')
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s %d player serving error: %s' %(datetime.datetime.now(),
                                                 repr(self.addr), e)

    elif file.startswith('/local/style.css'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: text/css \r\n\r\n')
      try:
        f = open('style.css')
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s %s css serving error: %s' %(datetime.datetime.now(),
                                              repr(self.addr), e)

    elif file.startswith('/local/control_'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: image/png \r\n\r\n')
      try:
        f = open(file.split('/')[-1])
        self.buffer += f.read(1000000000)
        f.close()

      except Exception, e:
        print '%s %s image serving error: %s' %(datetime.datetime.now(),
                                                repr(self.addr), e)

    elif file.startswith('/done'):
      self.buffer += ('HTTP/1.1 200 OK\r\n'
                      'Content-Type: text/html \r\n\r\n'
                      '<html><head><title>...</title></head>'
                      '<body></body></html>')

      path = '/'.join(file.split('/')[3:])
      
      if self.addr[0] in requests:
        print '%s %s Marking %s played' %(datetime.datetime.now(),
                                          repr(self.addr),
                                          requests[self.addr[0]])
        self.db.ExecuteSql('update tracks set plays = plays + 1, '
                           'last_played = NOW(), last_action = NOW() '
                           'where id=%s;'
                           % requests[self.addr[0]])
        self.db.ExecuteSql('commit;')
        
        del requests[self.addr[0]]

    else:
      self.buffer += ('HTTP/1.1 404 OK\r\nContent-Type: text/html\r\n\r\n'
                      '<html><head><title>MP3 server</title></head>'
                      '<body>File not found</body>')

  def writable(self):
    return len(self.buffer) > 0

  def handle_write(self):
    try:
      sent = self.send(self.buffer)
      self.buffer = self.buffer[sent:]
      if len(self.buffer) == 0:
        self.close()
    except:
      pass

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
