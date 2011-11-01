#!/usr/bin/python

import datetime
import feedparser
import os
import re
import shelve
import sys
import unicodedata
import urllib2


def Normalize(value):
  normalized = unicodedata.normalize('NFKD', unicode(value))
  normalized = normalized.encode('ascii', 'ignore')
  return normalized


plugins_dir = '%s/plugins' % os.getcwd()
print 'Appending %s to module path' % plugins_dir
sys.path.append(plugins_dir)
import blather

data = shelve.open('fetchshared.slf', writeback=True)

# Load passwords
password_mgr = urllib2.HTTPPasswordMgrWithDefaultRealm()
for feed in data['feeds']:
  if feed in data['usernames']:
    password_mgr.add_password(None, feed, 
                              data['usernames'][feed],
                              data['passwords'][feed])
    print 'Added authentication for %s' % feed

handler = urllib2.HTTPBasicAuthHandler(password_mgr)
opener = urllib2.build_opener(handler)
urllib2.install_opener(opener)

for feed in data['feeds']:
  data.setdefault('guids', {})
  data['guids'].setdefault(feed, [])

  print
  print 'Fetching %s' % feed

  remote = urllib2.urlopen(feed)
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
  t = data['type'].get(feed, 'post')

  # Newest entries are first
  entries = d.entries
  entries.reverse()
  for ent in entries:
    print
    print 'Considering %s' % ent.link
    print '  guid = %s' % ent.guid
    if ent.guid not in data['guids'][feed]:
      print '  ... is new'
      if ent.has_key('gr_annotation') or t in ['status', 'search']:
        if t == 'post':
          post = ('Mikal shared: <a href="%s">%s</a><br/><ul><i>%s</i></ul>'
                  %(ent.link, ent.title.replace('<br />', ' '),
                    ent['gr_annotation']))
        elif ent.has_key('author'):
          post = ('%s replied: <a href="%s">%s<a/>' %(ent['author'], ent.link,
                                                      ent.title))
        elif ent.has_key('link'):
          post = '<a href="%s">%s</a>' %(ent.link, ent.title)
          if ent.has_key('category'):
            post += ' (%s)' % ent['category']

        else:
          post = ent.title

        print '  %s' % Normalize(post)
        dt = datetime.datetime(ent.updated_parsed[0],
                               ent.updated_parsed[1],
                               ent.updated_parsed[2],
                               ent.updated_parsed[3],
                               ent.updated_parsed[4])
        print '  %s --> %s' %(ent.updated_parsed, dt)

        ds = blather.DataStore(dt=dt)
        ds.AddMessage(Normalize(post), type=t, dt=dt)
        ds.Save()

      else:
        print '  ... ignored because no comment found'

      data['guids'][feed].append(ent.guid)
    else:
      print '  ... already processed'

data.close()
