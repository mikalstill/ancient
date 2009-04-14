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


def Progress(database, track, i, total):
    Log('Copying to iPod %d of %d: %s' % (i, total, track))


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
  ipod_db = gpod.Database(argv[1])

  for t in ipod_db:
    print t['title']
    #print '  user: %s' % t['userdata']
    #print '  skips: %d' % t['skipcount']
    #print '  plays: %d' % t['playcount']

    track_id = None
    if 'mp3server_track_id' in t['userdata']:
      track_id = t['userdata']['mp3server_track_id']

    if t['skipcount'] > 0 and track_id:
      blogic.markskipped(track_id)
    if t['playcount'] > 0 and track_id:
      blogic.markplayed(track_id)

    ipod_db.remove(t)
    print

  ids = []
  for i in xrange(10):
    while len(ids) < i * 100:
      print
      print 'Found %d tracks' % len(ids)
      rendered = blogic.picktrack()

      if not rendered['id'] in ids:
        try:
          t = ipod_db.new_Track(filename=rendered['mp3_file'], podcast=False)
          t['userdata']['mp3server_track_id'] = rendered['id']
          ids.append(rendered['id'])
        except Exception, e:
          print 'Error: %s' % e
          db.ExecuteSql('update paths set error=1 where path=%s;'
                        % db.FormatSqlValue('path', rendered['mp3_file']))
          db.ExecuteSql('insert into events(timestamp, track_id, event, '
                        'details) values(now(), %d, "error: ipod sync", '
                        '%s);'
                        %(rendered['id'], db.FormatSqlValue('details', e)))
          db.ExecuteSql('commit;')
                  
    print
    ipod_db.copy_delayed_files(callback=Progress)

  ipod_db.close()
