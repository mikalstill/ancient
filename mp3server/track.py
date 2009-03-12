#!/usr/bin/python


"""Data structure for a single song."""


import copy
import cStringIO
import datetime
import MySQLdb
import re

import database
import gflags
import id3reader
import sys


FLAGS = gflags.FLAGS
gflags.DEFINE_string('audio_path', '/data/mp3',
                     'The directory containing audio files')


_PATH_PARSE_RE = re.compile('.*/([^/]*)/([^/]*)/([0-9]*)([^/]*)\.mp3')


class TrackException(Exception):
  """Raised when something exceptional happens. :P"""


class Track:
  """A song."""

  def __init__(self, db):
    self.db = db
    self.persistant = {}

  def FromMeta(self, artist, album, track_number, song):
    """Populate a track from its meta data."""

    # Cleanup input
    artist = artist.rstrip()
    album = album.rstrip()
    song = song.rstrip()

    self.persistant = self.db.GetOneRow('select * from tracks where '
                                        'artist=%s and album=%s '
                                        'and number=%d and song=%s;'
                                        %(self.db.FormatSqlValue('artist',
                                                                 artist),
                                          self.db.FormatSqlValue('album',
                                                                 album),
                                          track_number,
                                          self.db.FormatSqlValue('song',
                                                                 song)))

    if not self.persistant:
      self.persistant = {}
      self.persistant['artist'] = artist
      self.persistant['album'] = album
      self.persistant['number'] = track_number
      self.persistant['song'] = song
      self.persistant['paths'] = '[]'
      self.persistant['tags'] = '[]'

      self.persistant['plays'] = 0
      self.persistant['skips'] = 0
      self.persistant['creation_time'] = datetime.datetime.now()

  def FromPath(self, path, attemptparse=True):
    """Rehydrate a track from its path."""

    self.persistant = self.db.GetOneRow('select * from tracks where '
                                        'paths like "%%%s%%";'
                                        % self.db.FormatSqlValue('path',
                                                                 path)[1:-2])
    if self.persistant:
      return

    # This is a new track
    self.persistant = {}
    self.persistant['creation_time'] = datetime.datetime.now()

    # Use ID3 if we're missing information
    if not self.persistant.has_key('artist'):
      try:
        id3r = id3reader.Reader(path)
        if id3r.getValue('album'):
          self.persistant['album'] = id3r.getValue('album')
        if id3r.getValue('performer'):
          self.persistant['artist'] = id3r.getValue('performer')
        if id3r.getValue('title'):
          self.persistant['song'] = id3r.getValue('title')
        if id3r.getValue('track'):
          try:
            self.persistant['number'] = int(id3r.getValue('track'))
          except:
            pass
      except:
        pass

    # Attempt to parse the path using Mikal's file naming scheme
    if attemptparse:
      m = _PATH_PARSE_RE.match(path)
      if m:
        try:
          self.persistant['artist'] = m.group(1)
          self.persistant['album'] = m.group(2)
          self.persistant['song'] = m.group(4)
          self.persistant['number'] = int(m.group(3))

        except:
          pass

    # Perhaps we have this MP3 under another path
    try:
      self.FromMeta(self.persistant['artist'],
                    self.persistant['album'],
                    self.persistant['number'],
                    self.persistant['song'])
    except:
      pass

  def AddPath(self, path, attemptparse=False):
    """This song is stored at..."""

    if self.persistant.has_key('paths'):
      p = eval(self.persistant['paths'])
    else:
      p = []

    if not path in p:
      p.append(path)
    self.persistant['paths'] = repr(p)

  def AddTag(self, tag):
    """Add a tag for the track."""

    if not self.persistant:
      return

    if 'tags' in self.persistant and self.persistant['tags']:
      t = eval(self.persistant['tags'])
    else:
      t = []

    if not tag in t:
      t.append(tag)
    self.persistant['tags'] = repr(t)

  def AddPlays(self, plays):
    """This track has been played."""
    self.persistant['plays'] += plays

  def AddSkips(self, skips):
    """This track has been skipped."""
    self.persistant['skips'] += skips

  def Store(self):
    """Write song to database."""

    if not self.persistant:
      return

    try:
      self.db.WriteOneRow('tracks', 'id', self.persistant)
    except MySQLdb.Error, (errno, errstr):
      if errno != 1064:
        raise TrackException(self.db, 'Could not store track %s: %s "%s"'
                             %(self.persistant['paths'], errno, errstr))
    except database.FormatException, e:
      raise e
    except Exception, e:
      raise TrackException(self.db, 'Could not store track: %s: "%s" (%s)'
                           %(self.persistant['paths'], e, type(e)))

  def RenderValues(self):
    """Render a HTML description of this track."""

    return copy.deepcopy(self.persistant)
