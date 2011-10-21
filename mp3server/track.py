#!/usr/bin/python

"""Data structure for a single song."""

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import copy
import cStringIO
import datetime
import MySQLdb
import re

import database
import gflags
import id3reader

import sql


FLAGS = gflags.FLAGS
gflags.DEFINE_string('audio_path', '/data/mp3',
                     'The directory containing audio files')

gflags.DEFINE_boolean('verbose', False, 'Be verbose')
gflags.DEFINE_boolean('verbose_writes', False, 'Be verbose just about writes')


_PATH_PARSE_RE = re.compile('.*/+([^/]*)/+([^/]*)/+([0-9]*)([^/]*)\.mp3')


class TrackException(Exception):
  """Raised when something exceptional happens. :P"""


class Track:
  """A song."""

  def __init__(self, db):
    self.db = db
    self.persistant = {}

  def FromId(self, id):
    """Populate a track from its id number."""

    self.persistant = self.db.GetOneRow('select * from tracks where '
                                        'id=%d;'
                                        % id)

  def FromMeta(self, artist, album, track_number, song):
    """Populate a track from its meta data."""

    # Cleanup input
    artist = artist.rstrip()
    album = album.rstrip()
    song = song.rstrip()

    self.persistant = self.db.GetOneRow('select * from tracks where '
                                        'artist=%s and album=%s '
                                        'and number=%d and song=%s;'
                                        %(sql.FormatSqlValue('artist',
                                                                 artist),
                                          sql.FormatSqlValue('album',
                                                                 album),
                                          track_number,
                                          sql.FormatSqlValue('song',
                                                                 song)))

    if not self.persistant:
      self.persistant = {}
      self.persistant['artist'] = artist
      self.persistant['album'] = album
      self.persistant['number'] = track_number
      self.persistant['song'] = song

      self.persistant['plays'] = 0
      self.persistant['skips'] = 0
      self.persistant['creation_time'] = datetime.datetime.now()

  def FromPath(self, path, attemptparse=True):
    """Rehydrate a track from its path."""

    new_track = False
    updated = False
    
    id_row = self.db.GetOneRow('select track_id from paths where path = "%s";'
                               % path)
    if id_row:
      id = id_row['track_id']
      self.persistant = self.db.GetOneRow('select * from tracks where id=%d;'
                                          % id)

    else:
      # This is a new track
      self.persistant = {}
      self.persistant['creation_time'] = datetime.datetime.now()
      new_track = True

    # There was a bug a while ago where we left underscores in the database.
    # Clean that up...
    for key in 'artist', 'album', 'song':
      before = self.persistant[key]
      after = self.persistant[key].replace('_', ' ')
      if before != after:
        self.persistant[key] = after
        updated = True

    # Attempt to parse the path using Mikal's file naming scheme. 30 chars is
    # is the field length for ID3, and might indicate the tags have been
    # truncated
    if attemptparse:
      if (not 'artist' in self.persistant or
          len(self.persistant.get('song', ' ')) == 30 or
          len(self.persistant.get('album', ' ')) == 30):
        m = _PATH_PARSE_RE.match(path)
        if m:
          try:
            self.persistant['artist'] = m.group(1).replace('_', ' ')
            self.persistant['album'] = m.group(2).replace('_', ' ')
            self.persistant['song'] = m.group(4).replace('_', ' ')
            self.persistant['number'] = int(m.group(3)).replace('_', ' ')
            updated = True

          except:
            pass

    # Use ID3 if we're missing information. This isn't as good as file parsing
    # as the ID3 fields have maximum lengths we sometimes hit.
    if not self.persistant.has_key('artist'):
      try:
        id3r = id3reader.Reader(path)
        if id3r.getValue('album'):
          self.persistant['album'] = id3r.getValue('album')
          updated = True
        if id3r.getValue('performer'):
          self.persistant['artist'] = id3r.getValue('performer')
          updated = True
        if id3r.getValue('title'):
          self.persistant['song'] = id3r.getValue('title')
          updated = True
        if id3r.getValue('track'):
          try:
            self.persistant['number'] = int(id3r.getValue('track'))
            updated = True
          except:
            pass
      except:
        pass

    # Perhaps we have this MP3 under another path
    if not self.persistant.has_key('artist'):
      try:
        self.FromMeta(self.persistant['artist'],
                      self.persistant['album'],
                      self.persistant['number'],
                      self.persistant['song'])
        updated = True
      except:
        pass

    if new_track:
      # Now write this to the database to get an ID
      self.db.ExecuteSql('insert into tracks(artist, album, song, number) '
                         'values(%s, %s, %s, %d);'
                         %(sql.FormatSqlValue('artist',
                                                  self.persistant['artist']),
                           sql.FormatSqlValue('album',
                                                  self.persistant['album']),
                           sql.FormatSqlValue('song',
                                                  self.persistant['song']),
                           self.persistant.get('number', 0)))
      id = self.db.GetOneRow('select last_insert_id();')['last_insert_id()']
      self.persistant['id'] = id

    if updated:
      print 'Updated track %s' % self.persistant['id']
      self.Store()
    

  def AddPath(self, path):
    """This song is stored at..."""

    if not self.db.GetOneRow('select * from paths where path="%s";'
                             % path):
      self.db.ExecuteSql('insert into paths(path, track_id) values("%s", %d);'
                         %(path, self.persistant['id']))
      self.db.ExecuteSql('commit;')
      return True

    elif FLAGS.verbose:
      print 'Path already stored'

    return False

  def AddTag(self, tag):
    """Add a tag for the track."""

    if not self.persistant:
      return

    self.db.ExecuteSql('insert into tags(tag, track_id) values("%s", %d);'
                       %(tag, self.persistant['id']))
    self.db.ExecuteSql('commit;')

  def AddPlays(self, plays):
    """This track has been played."""
    self.persistant['plays'] += plays

  def AddSkips(self, skips):
    """This track has been skipped."""
    self.persistant['skips'] += skips

  def Store(self):
    """Write song to database."""

    if FLAGS.verbose or FLAGS.verbose_writes:
      print 'Writing track:'
      for key in sorted(self.persistant):
        print '  %s = %s' %(key, self.persistant[key])

    if not self.persistant:
      return
    
    try:
      self.db.WriteOneRow('tracks', 'id', self.persistant)
    except MySQLdb.Error, (errno, errstr):
      if errno != 1064:
        raise TrackException(self.db, 'Could not store track %s: %s "%s"'
                             %(self.persistant['id'], errno, errstr))
    except database.FormatException, e:
      raise e
    except Exception, e:
      raise TrackException(self.db, 'Could not store track: %s: "%s" (%s)'
                           %(self.persistant['id'], e, type(e)))

  def Delete(self):
    """Remove this song from the database."""

    self.db.ExecuteSql('delete from tracks where id=%d;'
                       % self.persistant['id'])
    self.db.ExecuteSql('commit;')

  def Merge(self, other):
    """Merge another track with this one."""

    # Logging just in case
    self.db.ExecuteSql('insert into events(timestamp, track_id, event, '
                       'details) values (now(), %d, "merge: before", %s);'
                       %(self.persistant['id'],
                         sql.FormatSqlValue('details',
                                                repr(self.persistant))))
    self.db.ExecuteSql('insert into events(timestamp, track_id, event, '
                       'details) values (now(), %d, "merge: deleted", %s);'
                       %(other.persistant['id'], 
                         sql.FormatSqlValue('details',
                                                repr(other.persistant))))

    # Fields which can be summed
    for f in ['plays', 'skips']:
      self.persistant[f] = (self.persistant.get(f, 0) +
                            other.persistant.get(f, 0))

    # Date fields where we take the newest
    for f in ['last_played', 'last_skipped', 'last_action']:
      a = self.persistant.get(f, datetime.datetime(1970, 1, 1))
      b = other.persistant.get(f, datetime.datetime(1970, 1, 1))
      if a > b:
        v = a
      else:
        v = b
      if v != datetime.datetime(1970, 1, 1):
        self.persistant[f] = v

    # Date fields where we take the oldest
    for f in ['creation_time']:
      a = self.persistant.get(f, datetime.datetime(1970, 1, 1))
      b = other.persistant.get(f, datetime.datetime(1970, 1, 1))
      if a < b:
        v = a
      else:
        v = b
      if v != datetime.datetime(1970, 1, 1):
        self.persistant[f] = v

    # Fields where we only clobber ours if we don't have a value
    for f in ['artist', 'album', 'song']:
      if not self.persistant.has_key(f) or not self.persistant[f]:
        self.persistant[f] = other.persistant.get(f, None)

    # Sometimes the number is a placeholder
    if self.persistant.has_key('number') and self.persistant['number'] == -1:
      self.persistant['number'] = other.persistant.get('number', -1)
    if not self.persistant.has_key('number'):
      self.persistant['number'] = other.persistant.get('number', -1)

    # Update the id in the tags table
    tags = self.db.GetRows('select tag from tags where track_id=%d;'
                           % other.persistant['id'])
    self.db.ExecuteSql('insert into events(timestamp, track_id, event, '
                       'details) values (now(), %d, "merge: tags: %d", %s);'
                       %(self.persistant['id'], other.persistant['id'],
                         sql.FormatSqlValue('details', repr(tags))))

    try:
      self.db.ExecuteSql('update tags set track_id=%d where track_id=%d;'
                         %(self.persistant['id'], other.persistant['id']))
      self.db.ExecuteSql('commit;')
    except:
      # This can happen if the is already a matching tag for the first track
      pass

    # Update the id in the paths table
    paths = self.db.GetRows('select path from paths where track_id=%d;'
                           % other.persistant['id'])
    self.db.ExecuteSql('insert into events(timestamp, track_id, event, '
                       'details) values (now(), %d, "merge: paths: %d", %s);'
                       %(self.persistant['id'], other.persistant['id'],
                         sql.FormatSqlValue('details', repr(paths))))
    
    self.db.ExecuteSql('update paths set track_id=%d where track_id=%d;'
                       %(self.persistant['id'], other.persistant['id']))
    self.db.ExecuteSql('commit;')

    self.db.ExecuteSql('insert into events(timestamp, track_id, event, '
                       'details) values (now(), %d, "merge: after", %s);'
                       %(self.persistant['id'],
                         sql.FormatSqlValue('details',
                                                repr(self.persistant))))
    self.db.ExecuteSql('commit;')

  def RenderValues(self):
    """Render a HTML description of this track."""

    retval = copy.deepcopy(self.persistant)

    tags = []
    for row in self.db.GetRows('select tag from tags where track_id=%d;'
                               % self.persistant['id']):
      tags.append(row['tag'])

    retval['tags'] = tags

    paths = []
    for row in self.db.GetRows('select path from paths where track_id=%d;'
                               % self.persistant['id']):
      paths.append(row['path'])

    retval['paths'] = paths 

    return retval
