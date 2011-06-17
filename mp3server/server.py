#!/usr/bin/python

# Web interface to my MP3 database

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import asyncore
import base64
import datetime
import os
import re
import time
import uuid
import urllib
import MySQLdb

import mhttp
import substitute

import coverhunt
import business
import database
import gflags
import track


FLAGS = gflags.FLAGS


running = True
uuid = uuid.uuid4()

requests = {}
skips = {}
bytes = 0
db = None
blogic = None


class http_server(mhttp.http_server):
  def client_init(self):
    """Do local setup."""
    
    self.http_handler_class = mp3_http_handler


class mp3_http_handler(mhttp.http_handler):
  def sendfile(self, file, subst={}, chunk=None):
    global db

    if not 'mp3_source' in subst:
      subst['mp3_source'] = business.GetClientSetting(db, self.client_id, 'mp3_source', '')
    if not 'user' in subst:
      subst['user'] = business.GetClientSetting(db, self.client_id, 'user', 'shared')
    if not 'volume' in subst:
      subst['volume'] = business.GetClientSetting(db, self.client_id, 'volume', '50')

    self._sendfile(file, subst)

  def dispatch(self, urlfile, post_data, chunk=None):
    global db
    
    # Implementation of uPnP -- must come before cookie set
    if urlfile.startswith('/getDeviceDesc'):
      self.handleurl_getdevicedesc(urlfile)
    elif urlfile.startswith('/uPnP_Control'):
      self.handleurl_cdscontrol(urlfile, post_data)
    elif urlfile.startswith('/trackedmp3/'):
      # We use the uPnP client's streaming behaviour to track plays
      self.is_tracked = True
      self.streamed_at = datetime.datetime.now()
      self.handleurl_mp3(urlfile[12:], chunk, tracked=True)

    else:
      # Normal clients can handle cookies
      if not self.cookie or 'mp3_server_id' not in self.cookie:
        db.ExecuteSql('insert into clients(createtime) values(now());')
        row = db.GetOneRow('select last_insert_id();')
        self.cookie = 'mp3_server_id=%s' % row['last_insert_id()']
        self.extra_headers.append('Set-Cookie: mp3_server_id=%s; '
                                  'expires=Sun, 17 Jan 2038 09:00:00 GMT'
                                  % row['last_insert_id()'])
        self.log('Created new client id %s' % row['last_insert_id()'])

      # Extract the id
      self.log('Cookie is %s' % self.cookie)
      for elem in self.cookie.split('; '):
        if elem.startswith('mp3_server_id='):
          self.client_id = int(elem[14:])
      self.log('This is client id %s' % self.client_id)

      db.ExecuteSql('update clients set requests=0 '
                    'where requests is null;')
      db.ExecuteSql('update clients set requests=requests + 1 '
                    'where id=%d;'
                    % self.client_id)
      db.ExecuteSql('commit;')

      
      # Top URL
      if urlfile == '/' or urlfile.startswith('/?'):
        self.handleurl_root(urlfile, post_data)

      # Implementation of the HTTP player
      elif urlfile == '/play':
        self.handleurl_play()
      elif urlfile.startswith('/mp3/'):
        self.handleurl_mp3(urlfile[5:], chunk)
      elif urlfile.startswith('/local/'):
        self.handleurl_local(urlfile)
      elif urlfile.startswith('/done'):
        self.handleurl_done(urlfile)
      elif urlfile.startswith('/skipped'):
        self.handleurl_skipped(urlfile)
      elif urlfile.startswith('/art'):
        self.handleurl_art(urlfile)
      elif urlfile.startswith('/merge'):
        self.handleurl_merge(urlfile)
      elif urlfile.startswith('/unmerge'):
        self.handleurl_unmerge(urlfile)
      elif urlfile.startswith('/art'):
        self.handleurl_art(urlfile)
      elif urlfile.startswith('/merge'):
        self.handleurl_merge(urlfile)

      # User interface for the HTTP player
      elif urlfile.startswith('/graph'):
        self.handleurl_graph()
      elif urlfile.startswith('/browse'):
        self.handleurl_browse(urlfile, post_data)
      elif urlfile.startswith('/tags'):
        self.handleurl_tags(urlfile)
      elif urlfile.startswith('/tag/'):
        self.handleurl_tag(urlfile)
      elif urlfile.startswith('/addtag/'):
        self.handleurl_addtag(urlfile)
      elif urlfile.startswith('/deletetag/'):
        self.handleurl_deletetag(urlfile)
      elif urlfile.startswith('/events'):
        self.handleurl_events(urlfile)

      # Needed for the remote ipod sync, think ghetto RPC
      elif urlfile.startswith('/picktracks'):
        self.handleurl_picktracks(urlfile)

      else:
        self.senderror(404, '%s urlfile not found' % urlfile)
        self.close()
  
  def _set_user_var(self, db, name, value):
    """Set a user variable."""
    
    value = value.replace('%2F', '/').replace('%3A', ':')
    if value == '':
      value = None

    self.log('Updating %s to %s' %(name, value))
    db.ExecuteSql('insert ignore into clients(id, createtime) '
                  'values(%s, now());'
                  % self.client_id)
    db.ExecuteSql('update clients set %s="%s" '
                  'where id=%s;'
                  %(name, value, self.client_id))
    db.ExecuteSql('commit;')

  def handleurl_root(self, urlfile, post_data):
    """The top level page."""

    if post_data:
      for line in post_data.split('\r\n'):
       for l in line.split('&'):
         if l:
            (name, value) = l.split('=')
            if name in ['mp3_source', 'user', 'volume']:
              self._set_user_var(db, name, value)

    elif '?' in urlfile:
      v = urlfile.split('?')[1]
      for l in v.split('&'):
        (name, value) = l.split('=')
        if name in ['mp3_source', 'user', 'volume']:
          self._set_user_var(db, name, value)
          
    self.sendfile('index.html')

  def handleurl_play(self):
    """The HTTP playback user interface."""

    global blogic

    self.markskipped(self.client_id)
    skips.setdefault(self.addr[0], 0)
    rendered = blogic.picktrack(client_id=self.client_id,
                                skips=skips[self.addr[0]])
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
           'events.track_id = tracks.id where event in ("play", "skip") '
           'order by events.timestamp desc limit 30;')
    results = self.renderbrowseresults(sql)
    rendered['results'] = '\n'.join(results)

    self.sendfile('graph.html', subst=rendered)

  def handleurl_browse(self, file, post_data):
    """Browse the database."""

    global blogic

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
               'random_filter': 'Random',
               'random_filter_compiled': '',
               'random_checked': 'checked',
              }

    # I am sure there is a better way than this
    if post_data:
      for l in post_data.split('\r\n'):
        if len(l) > 0:
          for arg in l.split('&'):
            (name, value) = arg.split('=')
            if value:
              value = mhttp.urldecode(value.replace('+', ' '))
              filters['%s_filter' % name] = value
              filters['%s_filter_compiled' % name] = value.replace(' ',
                                                                   '[ _+]+')

    recent_sql = ''
    if filters['recent_filter'] == 'Recent':
      recent_sql = 'and (to_days(now()) - to_days(creation_time)) < 15'
      filters['recent_checked'] = 'checked'

    unplayed_sql = ''
    if filters['unplayed_filter'] == 'Unplayed':
      unplayed_sql = ('and last_played=makedate(1970,1) and '
                      'last_skipped=makedate(1970,1)')
      filters['unplayed_checked'] = 'checked'

    random_sql_cols = ''
    random_sql_order = ''
    if filters['random_filter'] == 'Random':
      random_sql_cols = ', %s' % business.GenerateRankSql(2)
      random_sql_order = 'idx desc,'
      filters['random_checked'] = 'checked'

    if (filters['artist_filter'] or
        filters['album_filter'] or
        filters['track_filter']):
      limit_sql = ''
    else:
      limit_sql = 'limit 100'

    sql = ('select *%s from tracks join usersummary on usersummary.user="%s" and usersummary.track_id = id '
           'where artist rlike "%s" and album rlike "%s" and song rlike "%s" '
           '%s %s order by %s artist, song, album, number %s;'
           %(random_sql_cols,
             business.GetClientSetting(db, self.client_id, 'user', 'shared'),
             filters['artist_filter_compiled'],
             filters['album_filter_compiled'],
             filters['track_filter_compiled'],
             unplayed_sql,
             recent_sql,
             random_sql_order,
             limit_sql))
    self.log('Browse SQL = %s' % sql)

    results = self.renderbrowseresults(sql)
    filters['results'] = '\n'.join(results)
    self.sendfile('browse.html', subst=filters)

  def handleurl_tags(self, urlfile):
    """Serve a list of the tags available."""

    tags = []
    for row in db.GetRows('select distinct(tag), count(*) from tags '
                          'group by tag order by tag;'):
      self.log('Found tag: %s (%d tracks)' %(row['tag'], row['count(*)']))
      tags.append('<li><a href="/tag/%s">%s</a> (%d tracks)'
                  %(urllib.urlencode({'id': row['tag']}),
                    row['tag'], row['count(*)']))
      
    self.sendfile('tags.html', subst={'tags': '\n'.join(tags)})

  def handleurl_events(self, urlfile):
    """Show recent events."""

    events = []
    merge_master = None
    merge_slave = None
    merge_slave_tags = None
    merge_slave_paths = None

    # This select is like this because timestamps aren't accurate enough to be
    # ordered correctly unless we take the default sort order. I should fix
    # this by adding a unique counter to these rows
    for row in db.GetRows('select * from events;')[-1000:]:
      if row['event'] == 'merge: before':
        merge_master = row['track_id']
      elif row['event'] == 'merge: deleted':
        merge_slave = row['track_id']
      elif row['event'].startswith('merge: tags'):
        merge_slave_tags = eval(row['details'])
      elif row['event'].startswith('merge: paths'):
        merge_slave_paths = eval(row['details'])
      elif row['event'] == 'merge: after':
        events.append('<tr><td>%s</td><td>%s and %s</td><td>Merge</td>'
                      '<td>Track %s and %s were merged. %s owned tags %s and '
                      'paths %s before the merge</td><tr/>'
                      %(row['timestamp'],
                        merge_master, merge_slave, merge_master, merge_slave,
                        merge_slave, merge_slave_tags, merge_slave_paths))

      else:
        events.append('<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>'
                      %(row['timestamp'], row['track_id'], row['event'],
                        row['details']))

    self.sendfile('events.html',
                  subst={'events': ('<table><tr><td>Timestamp</td>'
                                    '<td>Track Id</td><td>Event</td>'
                                    '<td>Details</td></tr>%s</table>'
                                    % '\n'.join(events))})

  def handleurl_tag(self, urlfile):
    """Show songs with a given tag."""

    # This handles how soundmanager uses null sounds stupidly
    if urlfile.endswith('/null.mp3'):
      self.senderror(404, 'No such file')
      return

    (_, _, tag_encoded) = urlfile.split('/')
    tag = mhttp.urldecode(tag_encoded.split('=')[1])
    sql = ('select * from tags inner join tracks on tags.track_id = tracks.id '
           'where tag="%s";'
           % tag)
    self.log('Tag browse SQL = %s' % sql)
    results = self.renderbrowseresults(sql, includemissing=True)

    tags = {}
    tags['results'] = '\n'.join(results)
    tags['tag'] = tag
    tags['tag_encoded'] = tag_encoded
    self.sendfile('tag.html', subst=tags)

  def handleurl_addtag(self, urlfile):
    """Add this tag."""

    (_, _, tag_encoded, tracks) = urlfile.split('/')
    tag = mhttp.urldecode(tag_encoded.split('=')[1])
    tracks = tracks.split(',')

    for track in tracks:
      db.ExecuteSql('insert ignore into tags(tag, track_id) '
                    'values("%s", %s);'
                    %(tag, track))
      db.ExecuteSql('commit;')
    self.sendfile('done.html')

  def handleurl_deletetag(self, urlfile):
    """Delete this tag."""

    (_, _, tag_encoded, tracks) = urlfile.split('/')
    tag = mhttp.urldecode(tag_encoded.split('=')[1])
    tracks = tracks.split(',')

    self.log('Deleting tag %s from %s' %(tag, repr(tracks)))
    db.ExecuteSql('delete from tags where tag="%s" and '
                  'track_id in (%s);'
                  %(tag, ','.join(tracks)))
    db.ExecuteSql('commit;')
    self.sendfile('done.html')

  def handleurl_art(self, urlfile):
    """Serve an image for a given album, if we have one."""

    (_, _, artist, album) = urlfile.replace('%20', ' ').split('/')
    self.log('Fetching art for "%s" "%s"' %(artist, album))
    row = db.GetOneRow('select art from art where artist=%s and '
                       'album=%s;'
                       %(db.FormatSqlValue('artist', artist),
                         db.FormatSqlValue('album', album)))
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

  def handleurl_merge(self, urlfile):
    """Merge the specified list of tracks."""

    (_, _, tracks) = urlfile.split('/')
    tracks = tracks.split(',')

    self.log('Merging %s' % repr(tracks))
    first_track = track.Track(db)
    first_track.FromId(int(tracks[0]))
    self.log('Before: %s' % repr(first_track.persistant))

    cleanup = []
    for second_track_id in tracks[1:]:
      second_track = track.Track(db)
      second_track.FromId(int(second_track_id))

      first_track.Merge(second_track)
      self.log('After: %s' % repr(first_track.persistant))

      cleanup.append(second_track)

    first_track.Store()
    for second_track in cleanup:
      second_track.Delete()

    self.log('Merge finished')
    self.sendfile('done.html')

  def handleurl_unmerge(self, urlfile):
    """Undo a merge."""

    (_, _, tracks) = urlfile.split('/')
    tracks = tracks.replace(' ', '').replace('%20', '').split(',')

    self.log('Unmerging %s' % repr(tracks))
    first_track = track.Track(db)
    first_track.persistant = eval(db.GetOneRow('select * from events '
                                               'where '
                                               'event = "merge: before" '
                                               'and '
                                               'track_id = %s order by '
                                               'timestamp asc limit 1;'
                                               % tracks[0])['details'])
    self.log('Previous state of %s: %s'
             %(tracks[0], repr(first_track.persistant)))
    first_track.Store()
    
    for t_id in tracks[1:]:
      t = track.Track(db)
      t.persistant = eval(db.GetOneRow('select * from events where '
                                       'event = "merge: deleted" and '
                                       'track_id = %s order by '
                                       'timestamp desc limit 1;'
                                       % t_id)['details'])

      tags = eval(db.GetOneRow('select * from events where '
                               'event = "merge: tags: %s" '
                               'order by timestamp desc limit 1;'
                               % t_id)['details'])
      paths = eval(db.GetOneRow('select * from events where '
                                'event = "merge: paths: %s" '
                                'order by timestamp desc limit 1;'
                                % t_id)['details'])
      self.log('Previous state of %s: %s. Tags %s, Paths %s.'
               %(t_id, repr(t.persistant), tags, paths))
      t.Store()

      for tag in tags:
        try:
          db.ExecuteSql('update tags set track_id=%s where tag="%s";'
                        % (t_id, tag['tag']))
        except:
          pass

      for path in paths:
        try:
          db.ExecuteSql('update paths set track_id=%s where path=%s;'
                        % (t_id, db.FormatSqlValue('path', path['path'])))
        except:
          pass

      db.ExecuteSql('commit;')

  def renderbrowseresults(self, sql, includemissing=False):
    """Paint a table of the results of a SQL statement."""

    global blogic

    f = open('browse_result.html')
    results_template = f.read()
    f.close()

    f = open('missing_result.html')
    missing_template = f.read()
    f.close()

    results = []
    bgcolor = ''

    for row in db.GetRows(sql):
      this_track = track.Track(db)
      this_track.FromId(row['id'])
      rendered = this_track.RenderValues()
      
      (rendered['mp3_url'],
       rendered['mp3_urlfile']) = blogic.findMP3(row['id'],
                                              client_id=self.client_id)
      if not 'creation_time' in rendered:
        rendered['creation_time'] = ''

      if rendered['mp3_url']:
        if bgcolor == 'bgcolor="#DDDDDD"':
          bgcolor = ''
        else:
          bgcolor = 'bgcolor="#DDDDDD"'

        rendered['bgcolor'] = bgcolor
        results.append(substitute.substitute(results_template, rendered))
      else:
        if includemissing:
          results.append(substitute.substitute(missing_template, rendered))
        self.log('Skipping row with no MP3 URL: %s' % repr(rendered))

    return results

  def markskipped(self, client_id):
    """Mark skipped tracks, if any."""

    global blogic

    if self.addr[0] in requests:
      skips.setdefault(self.addr[0], 0)
      skips[self.addr[0]] += 1
      blogic.markskipped(requests[self.addr[0]][1],
                         skips[self.addr[0]], client_id=self.client_id)
      del requests[self.addr[0]]

  def markplayed(self, id, client_id):
    """Mark a track as played."""

    global blogic

    blogic.markplayed(id, client_id=client_id)
    if self.addr[0] in requests:
      skips[self.addr[0]] = 0
      del requests[self.addr[0]]

  def playgraph(self):
    """Generate a Google chart API graph of recent play history."""

    play_graph = {}
    skip_graph = {}

    now = datetime.datetime.now()
    one_hour = datetime.timedelta(minutes=60)
    one_hour_ago = now - one_hour

    # Collect data from MySQL
    for row in db.GetRows('select song, plays, skips, last_action, '
                          'last_played, last_skipped from tracks '
                          'where last_action is not null and '
                          'last_action > %s '
                          'order by last_action desc;'
                          % db.FormatSqlValue('date', one_hour_ago)):
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

  def handleurl_done(self, urlfile):
    """Mark an MP3 as played."""

    id = urlfile.split('/')[-1]
    if id and id != 'nosuch':
      self.markplayed(id, client_id=self.client_id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

    self.sendfile('done.html')

  def handleurl_skipped(self, urlfile):
    """Mark an MP3 as skipped."""

    global blogic

    # I use the blogic layer here, because its possible that the server
    # doesn't know the track currently being played (think browse interface).
    # I also don't do skip length tracking, as it makes no sense to the browse
    # interface

    id = urlfile.split('/')[-1]
    if id and id != 'nosuch':
      blogic.markskipped(id, -1, client_id=self.client_id)

    if self.addr[0] in requests:
      del requests[self.addr[0]]

    self.sendfile('skipped.html')

  def handleurl_mp3(self, urlfile, chunk, tracked=False):
    """Serve MP3 urlfiles."""

    self.id = int(urlfile)
    if self.addr[0] in requests:
      # A uPnP pause can look like a skip, but its requesting the same ID
      self.log('Comparing %s(%s) and %s(%s)' %(type(requests[self.addr[0]][1]),
                                               requests[self.addr[0]][1],
                                               type(self.id),
                                               self.id))
      if int(requests[self.addr[0]][1]) == self.id and tracked:
        self.log('This is a resume')
      else:
        self.markskipped(self.client_id)

    for row in db.GetRows('select path from paths where track_id=%s;'
                          % self.id):
      if row['path'].endswith('.mp3') and os.path.exists(row['path']):
        requests[self.addr[0]] = (datetime.datetime.now(), self.id)
        self.sendfile(row['path'], chunk=chunk)
        self.is_mp3 = True
        return

    self.senderror(500, 'MP3 %s missing' % urlfile)

  def handleurl_local(self, urlfile):
    """Return a local urlfile needed by the user interface."""

    ent = urlfile.split('/')[-1]
    self.sendfile(ent)

  def handleurl_picktracks(self, urlfile):
    """Bulk pick tracks."""

    global blogic
    limit = int(urlfile.split('/')[-1])
    self.log('Limit is %s' % limit)
    self.sendtext(repr(blogic.picktrack(limit=limit,
                                        client_id=self.client_id)))

  # uPnP stuff
  def handleurl_getdevicedesc(self, urlfile):
    """uPnP device discovery."""

    global uuid
    self.sendfile('upnp_devicedesc.xml',
                  subst={'ip': FLAGS.ip,
                         'port': FLAGS.port,
                         'uuid': uuid
                         })

  def handleurl_cdscontrol(self, urlfile, post_data):
    """uPnP CDS endpoint control."""

    global blogic

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
        result = substitute.substitute(results_template, {'title': title})
        self.log('XML %s' % result, console=FLAGS.showresponse)
        results.append(self.xmlsafe(result))        

      self.sendfile('upnp_browseresponse.xml',
                    subst={'result': '\n'.join(results),
                           'num_returned': len(results),
                           'num_matches': len(results)
                          })

    elif object_id == 'All' or object_id == 'Recent':
      skips.setdefault(self.addr[0], 0)
      rendered = blogic.picktrack(recent=(object_id == 'Recent'),
                                    client_id=self.client_id,
                                    skips=skips[self.addr[0]])
      self.log('uPnP track selection %s' % rendered['id'])
      rendered['ip'] = FLAGS.ip
      rendered['port'] = FLAGS.port
      rendered['objectid'] = object_id
      
      f = open('upnp_song.xml')
      results = f.read()
      f.close()

      results = substitute.substitute(results, rendered)
      for l in results.split('\n'):
        self.log('XML %s' % l, console=FLAGS.showresponse)
      results = self.xmlsafe(results)
      
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
                 ('<', '&lt;'), ('>', '&gt;'),
                 ("'", '')]:
      (i, o) = repl
      s = s.replace(i, o)
    return s

  def global_subst(self, subst):
    """Update the subst array with some standard numbers."""

    if not subst:
      subst = {}
    subst.update(self.getstats_ever())
    subst.update(self.getstats_today())
    return subst

  def getstats_ever(self):
    """Return some playback statistics for all time."""

    retval = {}
    row = db.GetOneRow('select count(*), max(plays), sum(plays), '
                       'max(skips), sum(skips) from tracks;')
    for key in row:
      retval['ever_%s' %(key.replace('(', '').\
                         replace(')', '').\
                         replace('*', ''))] = row[key]
    return retval

  def getstats_today(self):
    """Return some playback statistics for today."""

    retval = {}
    row = db.GetOneRow('select count(*) from events '
                       'where date(timestamp) = date(now()) '
                       'and event = "play";')
    retval['today_countplays'] = row['count(*)']
    row = db.GetOneRow('select count(*) from events '
                       'where date(timestamp) = date(now()) '
                       'and event = "skip";')
    retval['today_countskips'] = row['count(*)']
    return retval



def main(argv):
  global running
  global bytes
  global db
  global blogic
  
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  db = database.Database()
  server = http_server(FLAGS.ip, FLAGS.port)
  blogic = business.BusinessLogic(db, server.log)

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  ids = []
  last_summary = time.time()
  while running:
    last_event = time.time()
    asyncore.loop(timeout=10.0, count=1)

    if time.time() - last_event > 9.0:
      # We are idle
      print '%s ...' % datetime.datetime.now()

      # House keeping 
      remove = []
      for ent in requests:
        (t, _) = requests[ent]
        delta = datetime.datetime.now() - t
        if delta.seconds > 3600:
          print '%s Entry %s is too old' %(datetime.datetime.now(), ent)
          remove.append(ent)

      for ent in remove:
        del requests[ent]
      
      # Ensure no null data
      db.ExecuteSql('update tracks set last_played=makedate(1970,1) where '
                    'last_played is null;')
      db.ExecuteSql('update tracks set last_skipped=makedate(1970,1) where '
                    'last_skipped is null;')
      db.ExecuteSql('update tracks set plays=0 where plays is null;')
      db.ExecuteSql('update tracks set skips=0 where skips is null;')
      db.ExecuteSql('commit;')

      # Ensure all users have all ids
      if len(ids) == 0:
        print '%s Fetching track ids' % datetime.datetime.now()
        ids = db.GetRows('select distinct(id) from tracks;')
        ids.reverse()
        users = db.GetRows('select distinct user from usersummary;')

      else:
        for id_row in ids[:100]:
          for row in users:
            if db.ExecuteSql('insert ignore into usersummary(user, track_id, '
                            'skips, plays, updated) values '
                            '("%s", %s, 0, 0, now());'
                            %(row['user'], id_row['id'])) > 0:
              print '%s Added %s for user %s' %(datetime.datetime.now(),
                                                id_row['id'], row['user'])
        ids = ids[100:]

      # Work out play lengths
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
                                          mhttp.DisplayFriendlySize(bytes))
      last_summary = time.time()


if __name__ == "__main__":
  main(sys.argv)
