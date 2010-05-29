#!/usr/bin/python

# Fetch a file using HTTP, maintaining a simple cache to reduce load on the
# target webserver

import hashlib
import os
import sys
import time
import urllib

def Fetch(url):
  if not os.path.exists('cache'):
    os.mkdir('cache')

  ent = hashlib.sha224('url').hexdigest()
  if not os.path.exists('cache/%s' % ent):
    return _Fetch(url, ent)

  mod = os.path.getmtime('cache/%s' % ent)
  if time.time() - mod > 24 * 3600:
    return _Fetch(url, ent)
  
  sys.stderr.write('Cache hit for %s\n' % url)
  f = open('cache/%s' % ent)
  try:
    return f.read()
  finally:
    f.close()

def _Fetch(url, ent):
  remote = urllib.urlopen(url)
  local = open('cache/%s' % ent, 'w')
  d = remote.read()
  local.write(d)
  remote.close()
  local.close()
  return d

if __name__ == '__main__':
  print Fetch('http://www.stillhq.com')
