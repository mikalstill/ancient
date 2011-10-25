#!/usr/bin/python

# Fetch a file using HTTP, maintaining a simple cache to reduce load on the
# target webserver

import hashlib
import os
import sys
import time
import urllib2

def Fetch(url, maxage=3600, username=None, password=None, useragent=None):
  if not os.path.exists('/data/temp/cache'):
    os.mkdir('/data/temp/cache')

  ent = hashlib.sha224(url).hexdigest()
  if not os.path.exists('/data/temp/cache/%s' % ent):
    return _Fetch(url, ent, username, password, useragent)

  mod = os.path.getmtime('/data/temp/cache/%s' % ent)
  if time.time() - mod > maxage:
    return _Fetch(url, ent, username, password, useragent)
  
  sys.stderr.write('Cache hit for %s\n' % url)
  f = open('/data/temp/cache/%s' % ent)
  try:
    return f.read()
  finally:
    f.close()

def _Fetch(url, ent, username, password, useragent):
  if username:
    passman = urllib2.HTTPPasswordMgrWithDefaultRealm()
    passman.add_password(None, url, username, password)
    auth = urllib2.HTTPBasicAuthHandler(passman)
    opener = urllib2.build_opener(auth)
    urllib2.install_opener(opener)

  req = urllib2.Request(url)

  if useragent:
    req.add_header('User-Agent', useragent)

  remote = urllib2.urlopen(req)
  local = open('/data/temp/cache/%s' % ent, 'w')
  d = remote.read()
  local.write(d)
  remote.close()
  local.close()
  return d

if __name__ == '__main__':
  print Fetch('http://www.stillhq.com')
