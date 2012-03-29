#!/usr/bin/python

# Skip all tracks match a path for all users

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

Mark a skip for all tracks matching a path for all users.
"""

  print '\n\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


def ProcessDirectory(db, path):
  """Process a directory. Called recursively."""

  for row in db.GetRows('select * from paths where path like "%s";' % path):
      db.ExecuteSql('update usersummary set skips=skips + 50 where '
                    'track_id=%d;' % row['track_id'])
      print row['path']
  db.ExecuteSql('commit')


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
