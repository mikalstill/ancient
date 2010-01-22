#!/usr/bin/python

# Helpful web server stub

import asyncore
import base64
import cStringIO
import datetime
import gflags
import os
import re
import socket
import sys
import time
import traceback
import types
import urllib

import MySQLdb


FLAGS = gflags.FLAGS
gflags.DEFINE_string('ip', '192.168.1.13', 'Bind to this IP')
gflags.DEFINE_integer('port', 12345, 'Bind to this port')

gflags.DEFINE_boolean('showrequest', False,
                      'Show the content of HTTP request')
gflags.DEFINE_boolean('showpost', False,
                      'Show the content of POST operations')
gflags.DEFINE_boolean('showresponse', False,
                      'Show the content of response headers')

running = True

requests = {}
skips = {}
bytes = 0

_CONTENT_TYPES = {'css':  'text/css',
                  'html': 'text/html',
                  'mp3':  'audo/x-mpeg-3',
                  'png':  'image/png',
                  'swf':  'application/x-shockwave-flash',
                  'xml':  'text/xml'
                 }
_SUBST_RE = re.compile('(.*){{([^}]+)}}(.*)', re.MULTILINE | re.DOTALL)


def htc(m):
  return chr(int(m.group(1),16))


def urldecode(url):
  url = url.replace('+', ' ')
  rex = re.compile('%([0-9a-hA-H][0-9a-hA-H])',re.M)
  return rex.sub(htc,url)


class http_server(asyncore.dispatcher):
  """Listen for new client connections, which are then handed off to
     another class
  """

  def __init__(self, ip, port):
    self.ip= ip
    self.port = port
    self.logfile = open('log', 'a')

    self.client_init()
    
    asyncore.dispatcher.__init__(self)
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.bind((ip, port))
    self.listen(5)

  def client_init(self):
    """Called to give the client a chance to do their own setup."""

    self.http_handler_class = http_handler

  def log(self, msg, console=True):
    """Write a log line."""

    l = '%s %s %s\n' %(datetime.datetime.now(), repr(self.addr), msg)
    
    if console:
      sys.stdout.write(l)

    self.logfile.write(l)
    self.logfile.flush()

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
    handler = self.http_handler_class(conn, addr, self.log)


class http_handler(asyncore.dispatcher):
  """Handle a single connection"""

  def __init__(self, conn, addr, log):
    asyncore.dispatcher.__init__(self, sock=conn)
    self.addr = addr
    self.log = log

    self.buffer = ''

    # Used to track uPnP plays
    self.is_mp3 = False
    self.is_tracked = False
    self.streamed_at = None
    self.id = None

    self.cookie = None
    self.extra_headers = []

    self.client_id = 0

  def handle_read(self):
    rq = self.recv(32 * 1024)
    file = ''
    method = None
    post_data = None
    chunk = None

    for line in rq.split('\n'):
      line = line.rstrip('\r')
      self.log('REQUEST %s' % line, console=FLAGS.showrequest)

      if line.startswith('GET'):
        (_, file, _) = line.split(' ')
        method = 'GET'

      if line.startswith('POST'):
        (_, file, _) = line.split(' ')
        method = 'POST'

      if line.startswith('Range: '):
        chunk = line[7:]
      if line.startswith('Cookie: '):
        self.cookie = line[8:]

      if post_data is not None:
        post_data += '%s\r\n' % line

      if len(line) == 0 and method == 'POST' and post_data is None:
        post_data = ''

    if file:
      self.log('%s %s' %(method, file))

      if method == 'POST' and post_data:
        for l in post_data.split('\r\n'):
          self.log('DATA %s' % l, console=FLAGS.showpost)

    try:
      self.dispatch(file, post_data)
    except Exception, e:
      self.sendtraceback()

    self.log('%d bytes queued' % len(self.buffer))

  def dispatch(self, file, post_data):
    if file == '/':
      self.handleurl_root(post_data)

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
          self.log('MP3 request complete')
          delta = datetime.datetime.now() - self.streamed_at
          if delta.seconds < 30:
            self.log('MP3 streamed too fast')
          else:
            self.markplayed(self.id)

        self.close()

    except:
      pass

  def handle_close(self):
    pass

  def handleurl_root(self, post_data):
    """The top level page."""

    self.buffer += ('<html><head><title>Doh!</title></head><body>'
                    'You need to implement a root URL!'
                    '</body></html>')

  def sendredirect(self, path):
    """Send a HTTP 302 redirect."""

    self.sendheaders('HTTP/1.1 302 Found\r\n'
                     'Location: %s\r\n'
                     % path)

  def global_subst(self, subst):
    """A chance to update the substitutions with global values."""
    return subst

  def sendfile(self, path, subst=None, chunk=None):
    """Send a file to the client, including doing the MIME type properly."""

    subst = self.global_subst(subst)
    inset = 0
    if chunk:
      # Format is "bytes=6600100-"
      inset = int(chunk.split('=')[1].split('-')[0])
      self.log('Skipping the first %d bytes' % inset)

    data = ''
    try:
      f = open(path)
      f.read(inset)
      data += f.read()
      f.close()

    except Exception, e:
      self.senderror(404, 'File read error: %s (%s)' % (path, e))
      return

    extn = path.split('.')[-1]
    mime_type = _CONTENT_TYPES.get(extn, 'application/octet-stream')
    if mime_type.find('ml') != -1:
      if not subst:
        subst = {}
      
      data = self.substitute(data, subst)

    self.sendheaders('HTTP/1.1 200 OK\r\n'
                     'Content-Type: %s\r\n'
                     'Content-Length: %s\r\n'            
                     '%s\r\n'
                     %(mime_type, len(data), '\r\n'.join(self.extra_headers)))

    if mime_type == 'text/xml':
      for l in data.split('\n'):
        self.log('REPLY %s' % l, console=FLAGS.showresponse)
    
    self.buffer += data

  def substitute(self, data, subst):
    """Perform template substitution."""

    m = _SUBST_RE.match(data)
    while m:
      data = '%s%s%s' %(m.group(1),
                        subst.get(m.group(2), '<i>%s missing</i>'
                                  % m.group(2)),
                        m.group(3))
      m = _SUBST_RE.match(data)

    return data

  def sendtraceback(self):
    exc = None
    msg = ['Exception:']

    try:
      exc = sys.exc_info()
      for tb in traceback.format_exception(exc[0], exc[1], exc[2]):
        msg.append(tb)
        del tb

    finally:
      del exc

    # Headers have almost certainly already been sent at this point
    self.buffer += ('<br/><br/><font color=red><b>%s</b></font>'
                    % '<br/>'.join(msg))
    self.log(' ** '.join(msg))

  def senderror(self, number, msg):
    self.sendheaders('HTTP/1.1 %d %s\r\n'
                     'Content-Type: text/html\r\n\r\n'
                     %(number, msg))
    self.buffer += ('<html><head><title>MP3 server</title></head>'
                     '<body>%s</body>'
                     % msg)
    self.log('Sent %d error' % number)

  def sendheaders(self, headers):
    """Send HTTP response headers."""

    for l in headers.split('\r\n'):
      self.log('RESPONSE %s' % l, console=FLAGS.showresponse)

    self.buffer += headers


def DisplayFriendlySize(bytes):
  """Turn a number of bytes into a nice string"""

  t = type(bytes)
  if t != types.LongType and t != types.IntType and t != decimal.Decimal:
    return 'NotANumber(%s=%s)' %(t, bytes)

  if bytes < 1024:
    return '%d bytes' % bytes

  if bytes < 1024 * 1024:
    return '%d kb (%d bytes)' %((bytes / 1024), bytes)

  if bytes < 1024 * 1024 * 1024:
    return '%d mb (%d bytes)' %((bytes / (1024 * 1024)), bytes)

  return '%d gb (%d bytes)' %((bytes / (1024 * 1024 * 1024)), bytes)


def main(argv):
  global running
  global bytes
  
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  server = http_server(FLAGS.ip, FLAGS.port)

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  last_summary = time.time()
  while running:
    last_event = time.time()
    asyncore.loop(timeout=10.0, count=1)

    if time.time() - last_event > 9.0:
      # We are idle
      print '%s ...' % datetime.datetime.now()

    if time.time() - last_summary > 60.0:
      print '%s TOTAL BYTES SERVED: %s' %(datetime.datetime.now(),
                                          DisplayFriendlySize(bytes))
      last_summary = time.time()


if __name__ == "__main__":
  main(sys.argv)
