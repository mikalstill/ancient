#!/usr/bin/python

"""Import a directory into our database."""


import os
import re
import sys

import database
import gflags
import track


FLAGS = gflags.FLAGS
gflags.DEFINE_boolean('parse_path', True,
                      'Attempt to parse the path to obtain metadata')


def Usage():
  """Print a usage message and exit."""
  print """Unknown command line. Try one of:'

Import a local directory, adding only the new files.
"""

  print '\n\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


def ProcessDirectory(db, path):
  """Process a directory. Called recursively."""

  for ent in os.listdir(path):
    entpath = '%s/%s' %(path, ent)
    if os.path.isdir(entpath):
      ProcessDirectory(db, entpath)

    elif entpath.endswith('.mp3'):
      try:
        this_track = track.Track(db)
        this_track.FromPath(entpath)
        if this_track.AddPath(entpath):
          print entpath

        this_track.Store()

      except:
        print 'Failed to store: %s' % entpath


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)
  except gflags.FlagsError, e:
    Usage()

  if len(argv) < 2:
    Usage()

  print
  db = database.Database()
  for path in argv[1:]:
    ProcessDirectory(db, path)
