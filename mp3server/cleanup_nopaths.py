#!/usr/bin/python

"""Cleanup tracks which have no data files."""


import os
import re
import sys

import database
import gflags
import track


FLAGS = gflags.FLAGS


def Usage():
  """Print a usage message and exit."""
  print """Unknown command line. Try one of:'

Cleanup the database, finding tracks with no data files and trying to find
other tracks they should be merged with.
"""

  print '\n\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


_SONG_RE = re.compile('x*[0-9]*(.*)')

if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)
  except gflags.FlagsError, e:
    Usage()

  db = database.Database()
  for row in db.GetRows('select * from tracks where paths="[]";'):
    song = row['song']
    m = _SONG_RE.match(song)
    if m:
      song = m.group(1)

    ids = []
    try:
      for subrow in db.GetRows('select id from tracks where song '
                               'rlike "^x*[0-9]*%s$";'
                               % song):
        ids.append(subrow['id'])
    except:
      pass
    
    try:
      for subrow in db.GetRows('select id from tracks where song '
                               'rlike "^x*[0-9]*%s$";'
                               % song.replace(' ', '_')):
        ids.append(subrow['id'])
    except:
      pass
    print '%s: %d' %(song, len(ids))

    if len(ids) == 2:
      # The simple merge case. We should just be able to combine the two.
      first_song = track.Track(db)
      first_song.FromId(ids[0])

      second_song = track.Track(db)
      second_song.FromId(ids[1])

      print repr(first_song.persistant)
      print repr(second_song.persistant)
      print

      first_song.Merge(second_song)
      print repr(first_song.persistant)

      try:
        first_song.Store()
        second_song.Delete()
      except Exception, e:
        print 'Failed to store: %s' % e

      print
