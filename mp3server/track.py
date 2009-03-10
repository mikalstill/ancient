#!/usr/bin/python


"""Data structure for a single song."""


import copy
import cStringIO
import MySQLdb

import database
import gflags
import id3reader
import sys


FLAGS = gflags.FLAGS
gflags.DEFINE_string('audio_path', '/data/mp3',
                     'The directory containing audio files')


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

  def FromPath(self, path):
    """Rehydrate a track from its path."""

    self.persistant = self.db.GetOneRow('select * from tracks where '
                                        'paths like "%%%s%%";'
                                        % self.db.FormatSqlValue('path',
                                                                 path)[1:-2])
    if not self.persistant:
      print 'Lookup of %s failed' % path

  def AddPath(self, path):
    """This song is stored at..."""
      
    # Use ID3 if we're missing information
    if not self.persistant['artist']:
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

    p = eval(self.persistant['paths'])
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

    print ('Storing: "%s" "%s" %d "%s" %d plays and %d skips'
           %(self.persistant['artist'],
             self.persistant['album'],
             self.persistant['number'],
             self.persistant['song'],
             self.persistant['plays'],
             self.persistant['skips']))

    try:
      self.db.ExecuteSql('insert into tracks (artist, album, number, song, '
                         'paths, plays, skips, tags) '
                         'values (%s, %s, %d, %s, %s, %d, %d, %s);'
                         %(self.db.FormatSqlValue('artist',
                                                  self.persistant['artist']),
                           self.db.FormatSqlValue('album',
                                                  self.persistant['album']),
                           self.persistant['number'],
                           self.db.FormatSqlValue('song',
                                                  self.persistant['song']),
                           self.db.FormatSqlValue('paths',
                                                  self.persistant['paths']),
                           self.persistant['plays'],
                           self.persistant['skips'],
                           self.db.FormatSqlValue('tags',
                                                  self.persistant['tags'])))
      self.db.ExecuteSql('commit;')

    except MySQLdb.Error, (errno, errstr):
      if errno == 1062:
        # Integrity error
        print 'Update'
        self.db.ExecuteSql('update tracks set paths=%s, plays=%d, skips=%d, '
                           'tags=%s where '
                           'artist=%s and album=%s and number=%d and '
                           'song=%s;'
                         %(self.db.FormatSqlValue('paths',
                                                  self.persistant['paths']),
                           self.persistant['plays'],
                           self.persistant['skips'],
                           self.db.FormatSqlValue('tags',
                                                  self.persistant['tags']),
                           self.db.FormatSqlValue('artist',
                                                  self.persistant['artist']),
                           self.db.FormatSqlValue('album',
                                                  self.persistant['album']),
                           self.persistant['number'],
                           self.db.FormatSqlValue('song',
                                                  self.persistant['song'])))
        self.db.ExecuteSql('commit;')

      else:
        raise Exception('MySQL %d: %s' %(errno, errstr))

  def RenderValues(self):
    """Render a HTML description of this track."""

    return copy.deepcopy(self.persistant)
