#!/usr/bin/python

"""Sync an iPod with the MP3 server, possibly remotely."""

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import cookielib
import gpod
import os
import urllib2

import gflags


FLAGS = gflags.FLAGS
gflags.DEFINE_boolean('parse_path', True,
                      'Attempt to parse the path to obtain metadata')
gflags.DEFINE_boolean('record', True,
                      'Record plays and skips directly to the database')

gflags.DEFINE_string('mp3server', 'http://localhost:12345',
                     'Top level URL for the mp3server')
gflags.DEFINE_string('mp3cache', 'http://localhost/mp3',
                     'Top level URL for the mp3cache')
gflags.DEFINE_string('cookie_file', '~/.mp3server_cookies',
                     'A file to store local cookies in')
gflags.DEFINE_string('user', 'shared', 'The user of this iPod')


def readurl(url):
  remote = urllib2.urlopen(url)
  out = remote.readlines()
  remote.close()

  return ''.join(out)


def Usage():
  """Print a usage message and exit."""

  print 'Unknown command line. Please pass a mount point for the iPod'
  print '(for example /mnt where /dev/sdh2 is mounted there).'
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

  # Handle cookies
  jar = cookielib.LWPCookieJar()
  cookies = FLAGS.cookie_file.replace('~', os.environ['HOME'])
  if os.path.isfile(cookies):
    jar.load(cookies)

  opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(jar))
  urllib2.install_opener(opener)

  # Initialize the client
  readurl('%s/?user=%s&mp3_source=%s'
          %(FLAGS.mp3server, FLAGS.user,
            FLAGS.mp3cache.replace('/', '%2F').replace(':', '%3A')))

  # Empty the iPod
  ipod_db = gpod.Database(argv[1])

  delete_passes = 0
  total_skips = 0
  total_plays = 0

  while len(ipod_db) > 0:
    delete_passes += 1
    print '%d tracks on iPod' % len(ipod_db)

    for t in ipod_db:
      total_skips += t['skipcount']
      total_plays += t['playcount']
      print 'Pass %d, %d skips, %d plays: %s' %(delete_passes, total_skips,
                                                total_plays, t['title'])

      track_id = None
      if 'mp3server_track_id' in t['userdata']:
        track_id = t['userdata']['mp3server_track_id']
        if FLAGS.record and track_id:
          for i in range(t['skipcount']):
            readurl('%s/skipped/%s' %(FLAGS.mp3server, track_id))
          for i in range(t['playcount']):
            readurl('%s/done/%s' %(FLAGS.mp3server, track_id))

      ipod_db.remove(t)

  ipod_db.close()
  ipod_db = gpod.Database(argv[1])

  ids = []
  while len(ids) < 2000:
    print
    print 'Found %d tracks' % len(ids)
    rendered_tracks = eval(readurl('%s/picktracks' % FLAGS.mp3server))

    for rendered in rendered_tracks:
      if not rendered['id'] in ids:
        try:
          t = ipod_db.new_Track(filename=rendered['mp3_file'], podcast=False)
          t['userdata']['mp3server_track_id'] = rendered['id']
          t['skipcount'] = 0
          t['playcount'] = 0
          ids.append(rendered['id'])

        except Exception, e:
          print 'Error: %s' % e

  print
  ipod_db.copy_delayed_files(callback=Progress)
  ipod_db.close()


  # Persist our cookies
  print 'These are the cookies we have received so far :'
  for index, cookie in enumerate(jar):
    print index, '  :  ', cookie
  jar.save(cookies)
