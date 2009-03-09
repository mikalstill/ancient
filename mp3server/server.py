#!/usr/bin/python

# Web interface to my MP3 database

import asyncore
import cStringIO
import datetime
import os
import re
import socket
import sys
import time
import thread
import threading
import traceback
import types
import uuid

import MySQLdb

import database
import gflags
import track


FLAGS = gflags.FLAGS
gflags.DEFINE_string('ip', '192.168.1.99', 'Bind to this IP')
gflags.DEFINE_integer('port', 12345, 'Bind to this port')


running = True
uuid = uuid.uuid4()

requests = {}
bytes = 0

_CONTENT_TYPES = {'css':  'text/css',
                  'html': 'text/html',
                  'mp3':  'audo/x-mpeg-3',
                  'png':  'image/png',
                  'swf':  'application/x-shockwave-flash'
                 }


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

    # Used to track uPnP plays
    self.is_mp3 = False
    self.is_tracked = False
    self.streamed_at = None
    self.id = None

  def handle_read(self):
    rq = self.recv(32 * 1024)
    file = ''
    method = None
    post_data = None

    for line in rq.split('\n'):
      line = line.rstrip('\r')

      if line.startswith('GET'):
        (_, file, _) = line.split(' ')
        method = 'GET'

      if line.startswith('POST'):
        (_, file, _) = line.split(' ')
        method = 'POST'

      if post_data is not None:
        post_data += '%s\r\n' % line

      if len(line) == 0 and method == 'POST' and post_data is None:
        post_data = ''

    if file:
      print '%s %s %s %s' %(datetime.datetime.now(), repr(self.addr),
                            method, file)

      if method == 'POST' and post_data:
        for l in post_data.split('\r\n'):
          print '%s %s DATA %s' %(datetime.datetime.now(), repr(self.addr),
                                  l)

    # Implementation of the HTTP player
    if file == '/':
      self.handleurl_root()
    elif file.startswith('/mp3/'):
      self.handleurl_mp3(file[5:])
    elif file.startswith('/local/'):
      self.handleurl_local(file)
    elif file.startswith('/done'):
      self.handleurl_done(file)

    # Implementation of uPnP
    elif file.startswith('/getDeviceDesc'):
      self.handleurl_getdevicedesc(file)
    elif file.startswith('/uPnP_Control'):
      self.handleurl_cdscontrol(file, post_data)
    elif file.startswith('/trackedmp3/'):
      # We use the uPnP client's streaming behaviour to track plays
      self.is_tracked = True
      self.streamed_at = datetime.datetime.now()
      self.handleurl_mp3(file[12:])
    
    else:
      self.senderror(404, '%s file not found' % file)
      self.close()

  def writable(self):
    return len(self.buffer) > 0

  def handle_write(self):
    global bytes
    
    try:
      sent = self.send(self.buffer)
      bytes += sent

      self.buffer = self.buffer[sent:]
      if len(self.buffer) == 0:
        if self.is_mp3 and self.is_tracked and self.id:
          print '%s %s MP3 request complete' %(datetime.datetime.now(),
                                               repr(self.addr))
          delta = datetime.datetime.now() - self.streamed_at
          if delta.seconds < 30:
            print '%s %s MP3 streamed too fast' %(datetime.datetime.now(),
                                                  repr(self.addr))
          else:
            self.markplayed(self.id)

        self.close()

    except:
      pass

  def handle_close(self):
    pass

  def handleurl_root(self):
    """The top level of the user interface."""

    self.markskipped()
    rendered = self.picktrack()
    if not rendered:
      self.senderror(501, 'Failed to select a track')
      return

    requests[self.addr[0]] = rendered['id']
    rendered['graph'] = self.playgraph()
    self.sendfile('template.html', subst=rendered)

  def markskipped(self):
    """Mark skipped tracks, if any."""

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

  def markplayed(self, id):
    """Mark a track as played."""
    
    print '%s %s Marking %s played' %(datetime.datetime.now(),
                                      repr(self.addr), id)
    self.db.ExecuteSql('update tracks set plays = plays + 1, '
                       'last_played = NOW(), last_action = NOW() '
                       'where id=%s;'
                       % id)
    self.db.ExecuteSql('commit;')

  def picktrack(self):
    """Pick a track for this client and make sure it exists."""

    for row in self.db.GetRows('select paths from tracks '
                               'where paths <> "[]" '
                               'order by rand() limit 10;'):
      paths = eval(row['paths'])
      this_track = track.Track(self.db)
      this_track.FromPath(paths[0])
      rendered = this_track.RenderValues()

      if os.path.exists(rendered['mp3_path']):
        print '%s %s Selected %s' %(datetime.datetime.now(), repr(self.addr),
                                    rendered['mp3_path'])
        return rendered

    print '%s %s Could not find an MP3' %(datetime.datetime.now(),
                                          repr(self.addr))
    return {}

  def playgraph(self):
    """Generate a Google chart API graph of recent play history."""

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

    return ('http://chart.apis.google.com/chart?cht=bvg&'
            'chbh=a&chds=0,10,0,10&chs=800x125&chd=t:%s|%s&'
            'chco=00FF00,FF0000'
            %(play[:-1], skip[:-1]))

  def handleurl_done(self, file):
    """Mark an MP3 as played."""

    self.buffer += ('HTTP/1.1 200 OK\r\n'
                    'Content-Type: text/html \r\n\r\n'
                    '<html><head><title>...</title></head>'
                    '<body></body></html>')

    id = file.split('/')[-1]
    self.markplayed(id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

  def handleurl_mp3(self, file):
    """Serve MP3 files."""

    self.id = file
    for row in self.db.GetRows('select paths from tracks '
                               'where id=%s;'
                               % self.id):
      paths = eval(row['paths'])
      for path in paths:
        if path.endswith('.mp3') and os.path.exists(path):
          self.sendfile(path)
          self.is_mp3 = True
          return

    self.senderror(500, 'MP3 %s missing' % file)

  def handleurl_local(self, file):
    """Return a local file needed by the user interface."""

    ent = file.split('/')[-1]
    self.sendfile(ent)

  def handleurl_getdevicedesc(self, file):
    """uPnP device discovery."""

    global uuid

    self.sendfile('upnp_devicedesc.xml',
                  subst={'ip': FLAGS.ip,
                         'port': FLAGS.port,
                         'uuid': uuid
                         })

  def handleurl_cdscontrol(self, file, post_data):
    """uPnP CDS endpoint control."""

    object_id = None

    object_id_re = re.compile('<ObjectID>(.*)</ObjectID>')
    for l in post_data.split('\r\n'):
      m = object_id_re.match(l)
      if m:
        object_id = m.group(1)

    if object_id:
      print '%s %s uPnP request for object id %s' %(datetime.datetime.now(),
                                                    repr(self.addr),
                                                    object_id)
    if object_id == '0':
      f = open('upnp_results.xml')
      results = f.read()
      f.close()

      for repl in [('&', '&amp;'), ('"', '&quot;'), ('<', '&lt;'),
                   ('>', '&gt;')]:
        (i, o) = repl
        results = results.replace(i, o)

      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': results,
                           'num_returned': 1,
                           'num_matches': 1
                          })

    elif object_id == 'Music':
      rendered = self.picktrack()
      rendered['ip'] = FLAGS.ip
      rendered['port'] = FLAGS.port
      
      f = open('upnp_song.xml')
      results = f.read()
      f.close()

      for repl in [('&', '&amp;'), ('"', '&quot;'), ('<', '&lt;'),
                   ('>', '&gt;')]:
        (i, o) = repl
        results = results.replace(i, o)
      results = results % rendered
      
      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': results,
                           'num_returned': 1,
                           'num_matches': 1000
                          })

    else:
      self.senderror(501, 'Unknown object ID')
                    
  def sendfile(self, path, subst=None):
    """Send a file to the client, including doing the MIME type properly."""

    data = ''
    try:
      f = open(path)
      data += f.read(1000000000)
      f.close()
    except:
      senderror(404, 'File read error: %s' % path)
      return

    if subst:
      data = data % subst

    extn = path.split('.')[-1]
    self.buffer += ('HTTP/1.1 200 OK\r\n'
                    'Content-Type: %s\r\n'
                    'Content-Length: %s\r\n\r\n'
                    '%s'
                    %(_CONTENT_TYPES.get(extn, 'application/octet-stream'),
                      len(data), data))
    print '%s %s Sent %s (%d bytes)' %(datetime.datetime.now(),
                                       repr(self.addr),
                                       path, len(data))

  def senderror(self, number, msg):
    self.buffer += ('HTTP/1.1 %d OK\r\nContent-Type: text/html\r\n\r\n'
                    '<html><head><title>MP3 server</title></head>'
                    '<body>%s</body>'
                    %(number, msg))
    print '%s %s Sent %d error' %(datetime.datetime.now(),
                                  repr(self.addr), number)


def main(argv):
  global running
  global bytes
  
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

  last_summary = datetime.datetime.now()
  while running:
    asyncore.loop(timeout=10.0, count=1)

    delta = datetime.datetime.now() - last_summary
    if delta.seconds > 60:
      print '%s TOTAL BYTES SERVED: %d' %(datetime.datetime.now(), bytes)
      last_summary = datetime.datetime.now()


if __name__ == "__main__":
  main(sys.argv)
