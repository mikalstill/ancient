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
import business
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
    self.logfile = open('log', 'a')

    self.business = business.BusinessLogic(self.db, self.log)
    
    asyncore.dispatcher.__init__(self)
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.bind((ip, port))
    self.listen(5)

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
    handler = http_handler(conn, addr, self.db, self.business, self.log)


class http_handler(asyncore.dispatcher):
  """Handle a single connection"""

  def __init__(self, conn, addr, db, business, log):
    asyncore.dispatcher.__init__(self, sock=conn)
    self.addr = addr
    self.db = db
    self.business = business
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
      elif file.startswith('/skipped'):
        self.handleurl_skipped(file)
      elif file.startswith('/graph'):
        self.handleurl_graph()
      elif file.startswith('/browse'):
        self.handleurl_browse(file, post_data)
      elif file.startswith('/art'):
        self.handleurl_art(file)
      elif file.startswith('/merge'):
        self.handleurl_merge(file)

      else:
        self.senderror(404, '%s file not found' % file)
        self.close()

    self.log('%d bytes queued' % len(self.buffer))

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
          if value == '':
            value = None

          if name == 'mp3_source':
            self.log('Updating MP3 source to %s' % value)
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
    rendered = self.business.picktrack(client_id=self.client_id)
    requests[self.addr[0]] = (datetime.datetime.now(), rendered['id'])
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
               'track_filter_compiled': '.*',
               'recent_filter': '',
               'recent_filter_compiled': '',
               'recent_checked': '',
               'unplayed_filter': '',
               'unplayed_filter_compiled': '',
               'unplayed_checked': '',
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

    recent_sql = ''
    if filters['recent_filter'] == 'Recent':
      recent_sql = 'and (to_days(now()) - to_days(creation_time)) < 15'
      filters['recent_checked'] = 'checked'

    unplayed_sql = ''
    if filters['unplayed_filter'] == 'Unplayed':
      unplayed_sql = 'and last_played=makedate(1970,1) and last_skipped=makedate(1970,1)'
      filters['unplayed_checked'] = 'checked'

    sql = ('select * from tracks '
           'where artist rlike "%s" and album rlike "%s" and song rlike "%s" '
           '%s %s order by artist, song, album, number limit 100;'
           %(filters['artist_filter_compiled'],
             filters['album_filter_compiled'],
             filters['track_filter_compiled'],
             recent_sql, unplayed_sql))
    self.log('Browse SQL = %s' % sql)

    results = self.renderbrowseresults(sql)
    filters['results'] = '\n'.join(results)
    self.sendfile('browse.html', subst=filters)

  def handleurl_art(self, file):
    """Serve an image for a given album, if we have one."""

    (_, _, artist, album) = file.replace('%20', ' ').split('/')
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

  def handleurl_merge(self, file):
    """Merge the specified list of tracks."""

    (_, _, tracks) = file.split('/')
    tracks = tracks.split(',')

    self.log('Merging %s' % repr(tracks))
    first_track = track.Track(self.db)
    first_track.FromId(int(tracks[0]))
    self.log('Before: %s' % repr(first_track.persistant))

    cleanup = []
    for second_track_id in tracks[1:]:
      second_track = track.Track(self.db)
      second_track.FromId(int(second_track_id))

      first_track.Merge(second_track)
      self.log('After: %s' % repr(first_track.persistant))

      cleanup.append(second_track)

    first_track.Store()
    for second_track in cleanup:
      second_track.Delete()

    self.log('Merge finished')

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
      (row['mp3_url'],
       row['mp3_file']) = self.business.findMP3(row['id'],
                                                client_id=self.client_id)

      if row['mp3_url']:
        results.append(self.substitute(results_template, row))
      else:
        self.log('Skipping row with no MP3 URL')

    return results

  def markskipped(self):
    """Mark skipped tracks, if any."""

    if self.addr[0] in requests:
      self.business.markskipped(requests[self.addr[0]][1])
      del requests[self.addr[0]]

  def markplayed(self, id):
    """Mark a track as played."""

    self.business.markplayed(id)
    if self.addr[0] in requests:
      del requests[self.addr[0]]

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

    return ('cht=bvg&'
            'chbh=a&chds=0,10,0,10&chd=t:%s|%s&'
            'chco=00FF00,FF0000'
            %(play[:-1], skip[:-1]))

  def handleurl_done(self, file):
    """Mark an MP3 as played."""

    id = file.split('/')[-1]
    if id and id != 'nosuch':
      self.markplayed(id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

    self.sendfile('done.html')

  def handleurl_skipped(self, file):
    """Mark an MP3 as skipped."""

    id = file.split('/')[-1]
    if id and id != 'nosuch':
      self.markskipped(id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

    self.sendfile('skipped.html')

  def handleurl_mp3(self, file, chunk, tracked=False):
    """Serve MP3 files."""

    self.id = int(file)
    if self.addr[0] in requests:
      # A uPnP pause can look like a skip, but its requesting the same ID
      self.log('Comparing %s(%s) and %s(%s)' %(type(requests[self.addr[0]][1]),
                                               requests[self.addr[0]][1],
                                               type(self.id),
                                               self.id))
      if int(requests[self.addr[0]][1]) == self.id and tracked:
        self.log('This is a resume')
      else:
        self.markskipped()

    for row in self.db.GetRows('select path from paths where track_id=%s;'
                               % self.id):
      if row['path'].endswith('.mp3') and os.path.exists(row['path']):
        requests[self.addr[0]] = (datetime.datetime.now(), self.id)
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
      results_template = f.read()
      f.close()

      results = []
      for title in ['All', 'Recent']:
        result = self.substitute(results_template, {'title': title})
        results.append(self.xmlsafe(result))

      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': '\n'.join(results),
                           'num_returned': len(results),
                           'num_matches': len(results)
                          })

    elif object_id == 'All' or object_id == 'Recent':
      rendered = self.business.picktrack(recent=(object_id == 'Recent'),
                                         client_id=self.client_id)
      rendered['ip'] = FLAGS.ip
      rendered['port'] = FLAGS.port
      rendered['objectid'] = object_id
      
      f = open('upnp_song.xml')
      results = f.read()
      f.close()

      results = self.xmlsafe(self.substitute(results, rendered))
      
      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': results,
                           'num_returned': 1,
                           'num_matches': 1000
                          })

    else:
      self.senderror(501, 'Unknown object ID')

  def xmlsafe(self, s):
    """Return an XML safe version of a string."""
    
    for repl in [('&', '&amp;'), ('"', '&quot;'),
                 ('<', '&lt;'), ('>', '&gt;')]:
      (i, o) = repl
      s = s.replace(i, o)
    return s

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
    row = self.db.GetOneRow('select count(*) from events '
                            'where date(timestamp) = date(now()) '
                            'and event = "play";')
    retval['today_countplays'] = row['count(*)']
    row = self.db.GetOneRow('select count(*) from events '
                            'where date(timestamp) = date(now()) '
                            'and event = "skip";')
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
      print '%s ...' % datetime.datetime.now()
      
      remove = []
      for ent in requests:
        (t, _) = requests[ent]
        delta = datetime.datetime.now() - t
        if delta.seconds > 3600:
          print '%s Entry %s is too old' %(datetime.datetime.now(), ent)
          remove.append(ent)

      for ent in remove:
        del requests[ent]
      
      db.ExecuteSql('update tracks set last_played=makedate(1970,1) where '
                    'last_played is null;')
      db.ExecuteSql('update tracks set last_skipped=makedate(1970,1) where '
                    'last_skipped is null;')
      db.ExecuteSql('commit;')

      for row in db.GetRows('select path from paths where error is null '
                            'and duration is null limit 1;'):
        try:
          duration = mad.MadFile(row['path']).total_time()
          print '%s MP3 length %s: %f ms' %(datetime.datetime.now(),
                                            row['path'], duration)

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
