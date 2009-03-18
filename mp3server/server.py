#!/usr/bin/python

# Web interface to my MP3 database

import asyncore
import base64
import cStringIO
import datetime
import mad
import os
import re
import socket
import sys
import time
import types
import uuid

import MySQLdb

import coverhunt
import database
import gflags
import track


FLAGS = gflags.FLAGS
gflags.DEFINE_string('ip', '192.168.1.99', 'Bind to this IP')
gflags.DEFINE_integer('port', 12345, 'Bind to this port')

gflags.DEFINE_boolean('showrequest', False,
                      'Show the content of HTTP request')
gflags.DEFINE_boolean('showpost', False, 'Show the content of POST operations')
gflags.DEFINE_boolean('showresponse', False,
                      'Show the content of response headers')

running = True
uuid = uuid.uuid4()

requests = {}
bytes = 0

_CONTENT_TYPES = {'css':  'text/css',
                  'html': 'text/html',
                  'mp3':  'audo/x-mpeg-3',
                  'png':  'image/png',
                  'swf':  'application/x-shockwave-flash',
                  'xml':  'text/xml'
                 }
_SUBST_RE = re.compile('(.*){{([^}]+)}}(.*)', re.MULTILINE | re.DOTALL)


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

    self.cookie = None
    self.extra_headers = []

    self.client_id = 0

  def log(self, msg):
    """Write a log line."""

    print '%s %s %s' %(datetime.datetime.now(), repr(self.addr), msg)

  def handle_read(self):
    rq = self.recv(32 * 1024)
    file = ''
    method = None
    post_data = None
    chunk = None

    for line in rq.split('\n'):
      line = line.rstrip('\r')
      if FLAGS.showrequest:
        self.log('REQUEST %s' % line)

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

      if FLAGS.showpost and method == 'POST' and post_data:
        for l in post_data.split('\r\n'):
          self.log('DATA %s' % l)

    # Implementation of uPnP -- must come before cookie set
    if file.startswith('/getDeviceDesc'):
      self.handleurl_getdevicedesc(file)
    elif file.startswith('/uPnP_Control'):
      self.handleurl_cdscontrol(file, post_data)
    elif file.startswith('/trackedmp3/'):
      # We use the uPnP client's streaming behaviour to track plays
      self.is_tracked = True
      self.streamed_at = datetime.datetime.now()
      self.handleurl_mp3(file[12:], chunk, tracked=True)

    else:
      # Normal clients can handle cookies
      if not self.cookie:
        self.db.ExecuteSql('insert into clients(createtime) values(now());')
        row = self.db.GetOneRow('select last_insert_id();')
        self.cookie = 'id=%s' % row['last_insert_id()']
        self.extra_headers.append('Set-Cookie: id=%s; '
                                  'expires=Sun, 17 Jan 2038 09:00:00 GMT'
                                  % row['last_insert_id()'])

      # Extract the id
      for elem in self.cookie.split('; '):
        if elem.startswith('id='):
          self.client_id = int(elem[3:])

      self.db.ExecuteSql('update clients set requests=0 '
                         'where requests is null;')
      self.db.ExecuteSql('update clients set requests=requests + 1 '
                         'where id=%d;'
                         % self.client_id)
      self.db.ExecuteSql('commit;')

      
      # Top URL
      if file == '/':
        self.handleurl_root(post_data)

      # Implementation of the HTTP player
      elif file == '/play':
        self.handleurl_play()
      elif file.startswith('/mp3/'):
        self.handleurl_mp3(file[5:], chunk)
      elif file.startswith('/local/'):
        self.handleurl_local(file)
      elif file.startswith('/done'):
        self.handleurl_done(file)
      elif file.startswith('/graph'):
        self.handleurl_graph()
      elif file.startswith('/browse'):
        self.handleurl_browse(file, post_data)
      elif file.startswith('/art'):
        self.handleurl_art(file)

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

    if post_data:
      for l in post_data.split('\r\n'):
        if l:
          (name, value) = l.split('=')
          value = value.replace('%2F', '/').replace('%3A', ':')

          if name == 'mp3_source':
            self.db.ExecuteSql('update clients set mp3_source="%s" '
                               'where id=%s;'
                               %(value, self.client_id))
            self.db.ExecuteSql('commit;')
          
    row = self.db.GetOneRow('select * from clients where id=%s;'
                            % self.client_id)
    if not row or not row.has_key('mp3_source'):
      row = {'mp3_source': ''}

    self.sendfile('index.html',
                  subst={'mp3_source': row['mp3_source']
                        })

  def handleurl_play(self):
    """The HTTP playback user interface."""

    self.markskipped()
    rendered = self.picktrack()
    requests[self.addr[0]] = rendered['id']
    self.log('MP3 url is %s' % rendered['mp3_url'])
    if not rendered:
      self.senderror(501, 'Failed to select a track')
      return

    rendered['graph'] = self.playgraph()
    self.sendfile('play.html', subst=rendered)

  def handleurl_graph(self):
    """Return just a page with the history graph on it."""

    rendered = {}
    rendered['graph'] = self.playgraph()

    sql = ('select * from events inner join tracks on '
           'events.track_id = tracks.id order by events.timestamp desc '
           'limit 30;')
    results = self.renderbrowseresults(sql)
    rendered['results'] = '\n'.join(results)

    self.sendfile('graph.html', subst=rendered)

  def handleurl_browse(self, file, post_data):
    """Browse the database."""

    # Parse filters
    filters = {'artist_filter': '',
               'artist_filter_compiled': '.*',
               'album_filter': '',
               'album_filter_compiled': '.*',
               'track_filter': '',
               'track_filter_compiled': '.*'
              }

    # I am sure there is a better way than this
    if post_data:
      for l in post_data.split('\r\n'):
        if len(l) > 0:
          for arg in l.split('&'):
            (name, value) = arg.split('=')
            if value:
              for replacement in [('+', ' '), ('%5E', '^'), ('%24', '$')]:
                (a, b) = replacement
                value = value.replace(a, b)

              filters['%s_filter' % name] = value
              filters['%s_filter_compiled' % name] = value.replace(' ',
                                                                   '[ _+]+')

    sql = ('select * from tracks '
           'where artist rlike "%s" and album rlike "%s" and song rlike "%s" '
           'order by artist, album, number, song limit 1000;'
           %(filters['artist_filter_compiled'],
             filters['album_filter_compiled'],
             filters['track_filter_compiled']))
    self.log('Browse SQL = %s' % sql)

    results = self.renderbrowseresults(sql)
    filters['results'] = '\n'.join(results)
    self.sendfile('browse.html', subst=filters)

  def handleurl_art(self, file):
    """Serve an image for a given album, if we have one."""

    print file
    (_, _, artist, album) = file.split('/')
    self.log('Fetching art for "%s" "%s"' %(artist, album))
    row = self.db.GetOneRow('select art from art where artist=%s and '
                            'album=%s;'
                            %(self.db.FormatSqlValue('artist', artist),
                              self.db.FormatSqlValue('album', album)))
    if not row or not row['art']:
      self.senderror(404, 'No such art')
      return

    data = base64.decodestring(row['art'])
    self.sendheaders('HTTP/1.1 200 OK\r\n'
                     'Content-Type: image/jpeg\r\n'
                     'Content-Length: %s\r\n'            
                     '%s\r\n'
                     %(len(data), '\r\n'.join(self.extra_headers)))
    self.buffer += data

  def renderbrowseresults(self, sql):
    """Paint a table of the results of a SQL statement."""

    f = open('browse_result.html')
    results_template = f.read()
    f.close()

    results = []
    bgcolor = ''

    for row in self.db.GetRows(sql):
      if bgcolor == 'bgcolor="#DDDDDD"':
        bgcolor = ''
      else:
        bgcolor = 'bgcolor="#DDDDDD"'

      row['bgcolor'] = bgcolor
      row['mp3_url'] = self.findMP3(row['id'])
      results.append(self.substitute(results_template, row))

    return results

  def markskipped(self):
    """Mark skipped tracks, if any."""

    if self.addr[0] in requests:
      self.log('Marking %s skipped' % requests[self.addr[0]])
      self.db.ExecuteSql('update tracks set skips = skips + 1, '
                         'last_skipped = NOW(), last_action = NOW() '
                         'where id=%s;'
                         % requests[self.addr[0]])
      self.db.ExecuteSql('insert into events(timestamp, track_id, event) '
                         'values(now(), %s, "skip");'
                         % requests[self.addr[0]])
      self.db.ExecuteSql('commit;')
      del requests[self.addr[0]]

  def markplayed(self, id):
    """Mark a track as played."""
    
    self.log('Marking %s played' % id)
    self.db.ExecuteSql('update tracks set plays = plays + 1, '
                       'last_played = NOW(), last_action = NOW() '
                       'where id=%s;'
                       % id)
    self.db.ExecuteSql('insert into events(timestamp, track_id, event) '
                       'values(now(), %s, "play");'
                       % id)
    self.db.ExecuteSql('commit;')

    if self.addr[0] in requests:
      del requests[self.addr[0]]

  def picktrack(self):
    """Pick a track for this client and make sure it exists."""

    for row in self.db.GetRows('select id, ' 
                               'rand() + (plays * 0.00005) - (skips * 0.01) '
                               'as idx from tracks '
                               'order by idx desc limit 10;'):
      self.log('Considering %d, rank %f' %(row['id'], row['idx']))

      this_track = track.Track(self.db)
      this_track.FromId(row['id'])
      rendered = this_track.RenderValues()
      rendered['mp3_url'] = self.findMP3(rendered['id'])
      if rendered['mp3_url']:
        return rendered

    self.log('Could not find an MP3')
    return {}

  def findMP3(self, id):
    """Find a working MP3 file from a list of paths."""

    client_settings = self.db.GetOneRow('select * from clients where id=%s;'
                                        % self.client_id)
      
    for row in self.db.GetRows('select path from paths where track_id=%d;'
                               % id):
      if row['path'].endswith('.mp3') and os.path.exists(row['path']):
        if client_settings and client_settings.has_key('mp3_source'):
          mp3_url = ('%s/%s' %(client_settings['mp3_source'],
                               row['path'].replace(FLAGS.audio_path, '')))
        else:
          mp3_url = 'mp3/%s' % id

        return mp3_url

    return None

  def playgraph(self):
    """Generate a Google chart API graph of recent play history."""

    play_graph = {}
    skip_graph = {}

    now = datetime.datetime.now()
    one_hour = datetime.timedelta(minutes=60)
    one_hour_ago = now - one_hour

    # Collect data from MySQL
    for row in self.db.GetRows('select song, plays, skips, last_action, '
                               'last_played, last_skipped from tracks '
                               'where last_action is not null and '
                               'last_action > %s '
                               'order by last_action desc;'
                               % self.db.FormatSqlValue('date', one_hour_ago)):
      if row['last_played']:
        delta = now - row['last_played']
        secs = delta.seconds / 60
        if secs < 3600:
          play_graph.setdefault(secs, 0)
          play_graph[secs] += 1

      if row['last_skipped']:
        delta = now - row['last_skipped']
        secs = delta.seconds / 60
        if secs < 3600:
          skip_graph.setdefault(secs, 0)
          skip_graph[secs] += 1

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

    id = file.split('/')[-1]
    self.markplayed(id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

    self.sendfile('done.html')

  def handleurl_mp3(self, file, chunk, tracked=False):
    """Serve MP3 files."""

    self.id = int(file)
    if self.addr[0] in requests:
      # A uPnP pause can look like a skip, but its requesting the same ID
      self.log('Comparing %s(%s) and %s(%s)' %(type(requests[self.addr[0]]),
                                               requests[self.addr[0]],
                                               type(self.id),
                                               self.id))
      if int(requests[self.addr[0]]) == self.id and tracked:
        self.log('This is a resume')
      else:
        self.markskipped()

    for row in self.db.GetRows('select path from paths where track_id=%s;'
                               % self.id):
      if row['path'].endswith('.mp3') and os.path.exists(row['path']):
        requests[self.addr[0]] = self.id
        self.sendfile(row['path'], chunk=chunk)
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
      self.log('uPnP request for object id %s' % object_id)
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
      results = self.substitute(results, rendered)
      
      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': results,
                           'num_returned': 1,
                           'num_matches': 1000
                          })

    else:
      self.senderror(501, 'Unknown object ID')

  def sendredirect(self, path):
    """Send a HTTP 302 redirect."""

    self.sendheaders('HTTP/1.1 302 Found\r\n'
                     'Location: %s\r\n'
                     % path)

  def getstats_ever(self):
    """Return some playback statistics for all time."""

    retval = {}
    row = self.db.GetOneRow('select count(*), max(plays), sum(plays), '
                            'max(skips), sum(skips) from tracks;')
    for key in row:
      retval['ever_%s' %(key.replace('(', '').\
                         replace(')', '').\
                         replace('*', ''))] = row[key]
    return retval

  def getstats_today(self):
    """Return some playback statistics for today."""

    retval = {}
    row = self.db.GetOneRow('select count(*) from tracks '
                            'where day(last_played) = day(now());')
    retval['today_countplays'] = row['count(*)']
    row = self.db.GetOneRow('select count(*) from tracks '
                            'where day(last_skipped) = day(now());')
    retval['today_countskips'] = row['count(*)']
    return retval

  def sendfile(self, path, subst=None, chunk=None):
    """Send a file to the client, including doing the MIME type properly."""

    inset = 0
    if chunk:
      # Format is "bytes=6600100-"
      inset = int(chunk.split('=')[1].split('-')[0])
      self.log('Skipping the first %d bytes' % inset)

    data = ''
    try:
      f = open(path)
      f.read(inset)
      data += f.read(1000000000)
      f.close()

    except:
      self.senderror(404, 'File read error: %s' % path)
      return

    extn = path.split('.')[-1]
    mime_type = _CONTENT_TYPES.get(extn, 'application/octet-stream')
    if mime_type.find('ml') != -1:
      if not subst:
        subst = {}
      subst.update(self.getstats_ever())
      subst.update(self.getstats_today())
      
      data = self.substitute(data, subst)

    self.sendheaders('HTTP/1.1 200 OK\r\n'
                     'Content-Type: %s\r\n'
                     'Content-Length: %s\r\n'            
                     '%s\r\n'
                     %(mime_type, len(data), '\r\n'.join(self.extra_headers)))

    if mime_type == 'text/xml' and FLAGS.showresponse:
      for l in data.split('\n'):
        self.log('REPLY %s' % l)
    
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

    if FLAGS.showresponse:
      for l in headers.split('\r\n'):
        self.log('RESPONSE %s' % l)

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
    print FLAGS

  db = database.Database()
  server = http_server(FLAGS.ip, FLAGS.port, db)

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  last_summary = time.time()
  while running:
    last_event = time.time()
    asyncore.loop(timeout=10.0, count=1)

    if time.time() - last_event > 9.0:
      # We are idle
      print '%s IDLE' % datetime.datetime.now()
      for row in db.GetRows('select path from paths where error is null '
                            'and duration is null limit 1;'):
        try:
          duration = mad.MadFile(row['path']).total_time()
          print '%s %s: %f' %(datetime.datetime.now(), row['path'], duration)

          db.ExecuteSql('update paths set duration=%f where path=%s;'
                        %(duration, db.FormatSqlValue('path', row['path'])))
        except Exception, e:
          db.ExecuteSql('update paths set error=%s where path=%s;'
                        %(db.FormatSqlValue('error', str(e)),
                          db.FormatSqlValue('path', row['path'])))

      for row in db.GetRows('select distinct tracks.artist, tracks.album, '
                            'art.art from tracks left join art on '
                            'tracks.artist = art.artist and '
                            'tracks.album = art.album where '
                            'art.art is null and art.error is null and '
                            'tracks.artist is not null '
                            'and tracks.album is not null '
                            'group by tracks.album, tracks.artist limit 1;'):
        print '%s Fetching art for "%s" "%s"' %(datetime.datetime.now(),
                                                row['artist'], row['album'])
        a = coverhunt.Art(row['artist'], row['album'])
        art = a.Search()
        if not art:
          db.ExecuteSql('insert into art(artist, album, error) values '
                        '(%s, %s, "No art found");'
                        %(db.FormatSqlValue('artist', row['artist']),
                          db.FormatSqlValue('album', row['album'])))
        else:
          db.ExecuteSql('insert into art(artist, album, art) values '
                        '(%s, %s, "%s");'
                        %(db.FormatSqlValue('artist', row['artist']),
                          db.FormatSqlValue('album', row['album']), art))

    if time.time() - last_summary > 60.0:
      print '%s TOTAL BYTES SERVED: %s' %(datetime.datetime.now(),
                                          DisplayFriendlySize(bytes))
      last_summary = time.time()


if __name__ == "__main__":
  main(sys.argv)
