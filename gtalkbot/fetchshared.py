#!/usr/bin/python

import feedparser
import os
import re
import shelve
import sys
import unicodedata
import urllib


def Normalize(value):
  normalized = unicodedata.normalize('NFKD', unicode(value))
  normalized = normalized.encode('ascii', 'ignore')
  return normalized


plugins_dir = '%s/plugins' % os.getcwd()
print 'Appending %s to module path' % plugins_dir
sys.path.append(plugins_dir)
import blather

data = shelve.open('fetchshared.slf', writeback=True)
ds = blather.DataStore()

changed = False
for feed in data['feeds']:
  data.setdefault('guids', {})
  data['guids'].setdefault(feed, [])

  print
  print 'Fetching %s' % feed

  remote = urllib.urlopen(feed)
  lines = remote.readlines()
  remote.close()

  simplify_re = re.compile('(.*)<gr:annotation>'
                           '<content type="html">(.*)</content>'
                           '<author .*><name>.*</name></author>'
                           '</gr:annotation>(.*)')

  new_lines = []
  for line in lines:
    m = simplify_re.match(line)
    if m:
      new_lines.append('%s<gr:annotation>%s</gr:annotation>%s'
                       %(m.group(1), m.group(2), m.group(3)))
    else:
      new_lines.append(line)

  d = feedparser.parse(''.join(new_lines))

  # Newest entries are first
  entries = d.entries
  entries.reverse()
  for ent in entries:
    print
    print 'Considering %s' % ent.link
    print '  guid = %s' % ent.guid
    if ent.guid not in data['guids'][feed]:
      print '  ... is new'
      if ent.has_key('gr_annotation'):
        post = ('Mikal shared: <a href="%s">%s</a><br/><ul><i>%s</i></ul>'
                %(ent.link, ent.title.replace('<br />', ' '),
                  ent['gr_annotation']))
        print '  %s' % Normalize(post)
        ds.AddMessage(Normalize(post))
      else:
        print '  ... ignored because no comment found'

      data['guids'][feed].append(ent.guid)
      changed = True
    else:
      print '  ... already processed'

if changed:
  ds.Save()
data.close()
