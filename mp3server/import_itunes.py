#!/usr/bin/python

"""Read an iTunes XML file and import it into our database."""


import datetime
import re
import sys
import types

import database
import gflags


FLAGS = gflags.FLAGS


_DATE_RE = re.compile('([0-9][0-9][0-9][0-9])-([0-9][0-9])-([0-9][0-9])T'
                      '([0-9][0-9]):([0-9][0-9]):([0-9][0-9])Z')
def ExtractValue(t, v):
  if t == 'integer':
    return int(v)
  
  if t == 'date':
    m = _DATE_RE.match(v)
    if m:
      year = int(m.group(1))
      month = int(m.group(2))
      day = int(m.group(3))
      hour = int(m.group(4))
      minute = int(m.group(5))
      second = int(m.group(6))

      # This is actually a UTC time, but I can't be bothered converting it
      # to the local timezone at the moment
      return datetime.datetime(year, month, day, hour, minute, second)
  
  if t == 'string':
    return v
  
  return '<%s>%s</%s>' %(t, v, t)


def Usage():
  """Print a usage message and exit."""
  print """Unknown command line. Try one of:'

This is a helper script for importing exported iTunes libraries. Export your
iTunes library, and then run this command on the resulting XML file. The XML
file is passed on the command line.
"""

  print '\n\nAdditionally, you can use these global flags:%s' % FLAGS
  sys.exit(1)


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)
  except gflags.FlagsError, e:
    Usage()

  if len(argv) < 2:
    Usage()

  db = database.Database()


  # I don't use the python XML parser, as it is too slow with my 22mb library
  f = open(argv[1])

  ## <key>9585</key>
  ## <dict>
  ##         <key>Track ID</key><integer>9585</integer>
  ##         <key>Name</key><string>02Love_Me_Two_Times</string>
  ##         <key>Kind</key><string>MPEG audio file</string>
  ##         <key>Size</key><integer>5138206</integer>
  ##         <key>Total Time</key><integer>195291</integer>
  ##         <key>Date Modified</key><date>2007-06-20T15:24:19Z</date>
  ##         <key>Date Added</key><date>2006-02-13T06:17:03Z</date>
  ##         <key>Bit Rate</key><integer>210</integer>
  ##         <key>Sample Rate</key><integer>44100</integer>
  ##         <key>Skip Count</key><integer>1</integer>
  ##         <key>Skip Date</key><date>2007-10-12T16:39:14Z</date>
  ##         <key>Normalization</key><integer>4908</integer>
  ##         <key>Persistent ID</key><string>79025B48C3EC9D04</string>
  ##         <key>Track Type</key><string>File</string>
  ##         <key>Location</key><string>file://localhost/Volumes/data/mp3/...
  ##              symlinks/aerosmith/young_lust_the_aerosmith_ant/...
  #               02love_me_two_times.mp3</string>
  ##         <key>File Folder Count</key><integer>-1</integer>
  ##         <key>Library Folder Count</key><integer>-1</integer>
  ## </dict>

  songkey_re = re.compile('[ \t]+<key>([0-9]+)</key>$')
  string_re = re.compile('[ \t]+<key>(.*)</key><(string)>(.*)</string>')
  int_re = re.compile('[ \t]+<key>(.*)</key><(integer)>(.*)</integer>')
  date_re = re.compile('[ \t]+<key>(.*)</key><(date)>(.*)</date>')

  l = f.readline()
  key = None
  song = {}

  preamble = []
  songs = {}
  postamble = []

  while l:
    handled = False

    if postamble:
      postamble.append(l.rstrip())
      handled = True

    else:
        m = songkey_re.match(l)
        if m:
          key = m.group(1)
          song = {}
          handled = True

        elif key:
          for r in [string_re, int_re, date_re]:
            m = r.match(l)
            if m:
              song[m.group(1)] = ExtractValue(m.group(2), m.group(3))
              handled = True
              break

        if not handled:
          if l.find('</dict>') != -1 and key:

            location = song['Location']
            if not songs.has_key(location):
              song['keys'] = [key]
              songs[location] = song
            else:
              songs[location]['keys'].append(key)
              for k in ['Play Count', 'Skip Count']:
                if k in song:
                  songs[location].setdefault(k, 0)
                  songs[location][k] += song[k]

            key = None

          elif l.find('<key>Playlists</key>') != -1:
            postamble.append(l.rstrip())

          elif not songs:
            preamble.append(l.rstrip())

    l = f.readline()

  # We output by key, but stored by location -- build a reverse mapping,
  # as well as a map of deleted keys
  key_map = {}
  old_keys = {}
  for location in songs:
    key_map[int(songs[location]['keys'][0])] = location
    for key in songs[location]['keys'][1:]:
      old_keys[key] = songs[location]['keys'][0]

  keys = key_map.keys()
  keys.sort()

  for key in keys:
    location = key_map[key]

    for k in songs[location]:
      if k != 'keys':
        print '%s: %s' %(k, songs[location][k])

    print
