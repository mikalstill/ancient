#!/usr/bin/python

"""Test ranking algorithm."""


import sys

import database
import gflags
import track


FLAGS = gflags.FLAGS


def Usage():
  """Print a usage message and exit."""
  print 'Unknown command line'
  print '\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)
  except gflags.FlagsError, e:
    Usage()

  db = database.Database()
  for i in range(1000):
    row = db.GetOneRow('select song, plays, skips, rand() + plays * 0.0001 - skips * 0.01 as idx from tracks order by idx desc limit 1;')
    print repr(row)
