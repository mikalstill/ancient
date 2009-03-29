#!/usr/bin/python

"""Sync an iPod with our database."""


import gpod
import sys

import business
import database
import gflags
import track


FLAGS = gflags.FLAGS
gflags.DEFINE_boolean('parse_path', True,
                      'Attempt to parse the path to obtain metadata')


def Usage():
  """Print a usage message and exit."""

  print 'Unknown command line. Please pass a mount point for the iPod.'
  print '\n\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


def Log(msg, console=True):
  if console:
    print msg


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)
  except gflags.FlagsError, e:
    Usage()

  if len(argv) < 2:
    Usage()

  db = database.Database()
  blogic = business.BusinessLogic(db, Log)
  ipod_db = gpod.Database(mountpoint=argv[1])

  rendered = blogic.picktrack()
  t = gpod.Track(filename=rendered['mp3_file'])
  ipod_db.add(t)
  
  ipod_db.close()
