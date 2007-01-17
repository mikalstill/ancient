#!/usr/bin/python2.4

import feedparser
import re
import urllib
import shelve
import sys
import os
import time

from stat import *

re_attributeparser = re.compile('([^=]*)="([^"]*)" *(.*)')
def ParseAttributes(inputline):
  line = inputline
  result = {}

  m = re_attributeparser.match(line)
  while m:
    result[m.group(1)] = m.group(2)
    line = m.group(3)
    m = re_attributeparser.match(line)

  return result


def PrintProgress(bytecount):
  if bytecount < 1024 * 1024:
    print '%dk' %(bytecount / 1024)
    return

  if bytecount < 1024 * 1024 * 1024:
    print '%dm' %(bytecount / 1024 / 1024)
    return


def Download(url, title, subtitle, description):
  print 'Downloading %s' % url
  re_filename = re.compile('.*/([^/\?]*).*')
  url = url.replace('&amp;', '&')
  m = re_filename.match(url)
  filename = m.group(1)

  print 'Destination will be %s' % filename
  details = shelve.open(filename + '.pkl', writeback = True)
  if details.has_key('done'):
    print 'This download is already complete'
  else:
    details['url'] = url
    details['title'] = title
    details['subtitle'] = subtitle
    details['description'] = description
    details.sync()

    remote = urllib.urlopen(url)
    local = open(filename, 'w')
  
    done = False
    total = 0
    count = 0

    while not done:
      data = remote.read(1024)
      length = len(data)
      if length < 1024:
        done = True
      local.write(data)
      total += len(data)

      if count % 50 == 0:
        PrintProgress(total)

      count += 1

    print 'Finished'
    remote.close()
    local.close()
    details['size'] = total
    details['done'] = True
    details.sync()

    if total < 1024 * 1024:
      print 'Download unbelievably small'
      sys.exit(1)

  # And now import the video
  if not details.get('imported', False):
    print 'Importing the video'
    mtime = time.strftime('%Y-%m-%d %H:%M:%S', 
                          time.gmtime(os.stat(filename)[ST_MTIME]))
    import_cmd = ('/home/mikal/mythtv/mythtv-0.20/contrib/myth.rebuilddatabase.pl'
              ' --file "%s" --norename --answer "y" --answer "1001"'
              ' --answer "%s" --answer "%s" --answer "%s" --answer "%s"'
              ' --answer "" --answer "y"' %(filename, title, subtitle,
              description, mtime))
    details['imported'] = True
    details['import_cmd'] = import_cmd
    details['mtime'] = mtime
    try:
      os.system(import_cmd)
    except:
      details['import_failed'] = True
    details.sync()
    sys.exit(1)


def Sync(xmlfile, title):
  # Grab the XML
  xmllines = xmlfile.readlines()

  # Modify the XML to work around namespace handling bugs in FeedParser
  print 'Apply media:content work around'
  lines = []
  re_mediacontent = re.compile('(.*)<media:content([^>]*)/ *>(.*)')

  for line in xmllines:
    m = re_mediacontent.match(line)
    count = 1
    while m:
      line = '%s<media:wannabe%d>%s</media:wannabe%d>%s' %(m.group(1), count,
                                                         m.group(2),
                                                         count, m.group(3))
      m = re_mediacontent.match(line)
      count = count + 1

    lines.append(line)

  # Parse the modified XML
  xml = ''.join(lines)
  parser = feedparser.parse(xml)
  if parser.feed.has_key('title'):
    print parser.feed.title
  elif parser.feed.has_key('description'):
    print parser.feed.description
  else:
    print '\tUrl: %s'%(url)

  print ''

  # Find the media:content entries
  for entry in parser.entries:
    videos = {}
    description = entry.description
    subtitle = entry.title

    if entry.has_key('media_description'):
      description = entry['media_description']

    # Enclosures
    if entry.has_key('enclosures'):
      for enclosure in entry.enclosures:
        videos[enclosure.type] = enclosure

    # Media:RSS
    for key in entry.keys():
      if key.startswith('media_wannabe'):
        attrs = ParseAttributes(entry[key])
        if attrs.has_key('type'):
          videos[attrs['type']] = attrs
        if attrs.has_key('title'):
          subtitle = attrs['title']

    if videos.has_key('video/x-msvideo'):
      print 'Picked an AVI file'
      Download(videos['video/x-msvideo']['url'],
               title,
               subtitle,
               description)
    elif videos.has_key('video/mp4'):
      print 'Picked a MP4 file'
      Download(videos['video/mp4']['url'],
               title,
               subtitle,
               description)
    else:
      print 'Unsure which to prefer from: %s' % repr(videos.keys())


if __name__ == "__main__":
  if sys.argv[1] == '--url':
    xmlfile = urllib.urlopen(sys.argv[2])
  else:
    xmlfile = open(sys.argv[2])

  Sync(xmlfile, sys.argv[3])
