#!/usr/bin/python

"""Read an iTunes XML file and import it into our database."""

import re
import sys
import types


def ExtractValue(t, v):
  if t == 'integer':
    return int(v)
  return '<%s>%s</%s>' %(t, v, t)

def EncodeValue(v):
  if type(v) == types.IntType:
    return '<integer>%s</integer>' % v
  return v


# I don't use the python XML parser, as it is too slow with my 22mb library
f = open(sys.argv[1])

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
                # print '  %s(%s): %s -> %s' %(location, k, songs[location][k],
                #                              songs[location][k] + song[k])
                songs[location][k] += song[k]

          key = None

        elif l.find('<key>Playlists</key>') != -1:
          postamble.append(l.rstrip())

        elif not songs:
          preamble.append(l.rstrip())

  l = f.readline()


for l in preamble[:-1]:
  print l

# We output by key, but stored by location -- build a reverse mapping, as well
# as a map of deleted keys
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

  print '\t\t<key>%s</key>' % songs[location]['keys'][0]
  print '\t\t<dict>'

  for k in songs[location]:
    if k != 'keys':
      print '\t\t\t<key>%s</key>%s' %(k, EncodeValue(songs[location][k]))
 
  print '\t\t</dict>'

print '\t</dict>'
track_id_re = re.compile('.*<key>Track ID</key><integer>([0-9]+)</integer>.*')
for l in postamble:
  m = track_id_re.match(l)
  if not m:
    print l
  else:
    if m.group(1) in old_keys:
      print ('\t\t\t\t\t<key>Track ID</key><integer>%s</integer>'
             % old_keys[m.group(1)])
    else:
      print l
    
