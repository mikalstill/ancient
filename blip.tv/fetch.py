#!/usr/bin/python

# $1 is your search term. Something like "linuxconfau".

import os
import progressbar
import re
import sys
import urllib


# <a href="/file/4683393/" title="Watch Keynote: Vinton G. Cerf">
VIDEO_RE = re.compile('.*a href.*/file/([0-9]*)/.*title=.*')

# <option value="/file/4683393?filename=Linuxconfau-KeynoteVintonGCerf673.flv">Source &mdash; Flash Video (.flv)</option>
DOWNLOAD_RE = re.compile('.*option value.*/file/.*filename=(.*)">.*')


def FetchUrl(url):
  url = url.rstrip()
  filename = url.split('/')[-1]

  if os.path.exists(filename):
    print filename 
    return

  remote = urllib.urlopen(url)
  total_length = int(remote.info()['Content-Length'])
  current_length = 0
  local = open('incoming', 'w')

  widgets = ['%s: ' % filename,
             progressbar.Percentage(),
             ' ',
             progressbar.Bar(marker=progressbar.RotatingMarker()),
             ' ',
             progressbar.ETA(),
             ' ',
             progressbar.FileTransferSpeed()
            ]
  pbar = progressbar.ProgressBar(widgets=widgets, maxval=total_length).start()

  d = remote.read(1024)
  current_length += len(d)
  while d:
    pbar.update(current_length)

    local.write(d)
    d = remote.read(1024)
    current_length += len(d)

  pbar.finish()
  local.close()
  remote.close()

  os.rename('incoming', filename)


# main
try:
  os.remove('incoming')
except:
  pass

for i in range(1, 100):
  found = 0
  print 'Fetching http://blip.tv/?search=%s;s=search;page=%d' %(sys.argv[1], i)
  f = urllib.urlopen('http://blip.tv/?search=%s;s=search;page=%d'
                     % (sys.argv[1], i))
  for idxline in f.readlines():
    m = VIDEO_RE.match(idxline)
    if m:
      print 'Found video page: %s' % m.group(1)
      found += 1

      g = urllib.urlopen('http://blip.tv/file/%s/' % m.group(1))
      for vidline in g.readlines():
        n = DOWNLOAD_RE.match(vidline)
        if n:
          print 'Found download: %s' % n.group(1)
          FetchUrl('http://blip.tv/file/get/%s' % n.group(1).split('/')[-1])

  if found == 0:
    sys.exit(0)

