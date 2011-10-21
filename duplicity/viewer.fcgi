#!/usr/bin/env python

import cgi
import datetime
import os
import time
import MySQLdb

db = MySQLdb.connect(user = 'duplicity', db = 'duplicity', passwd = 'duplicity')
cursor = db.cursor(MySQLdb.cursors.DictCursor)

dir_cache = {}
file_cache = {}


def GetDirectory(path):
  #if path in cache:
  #  (age, val) = cache[path]
  #  if time.time() - age < 360:
  #    return val

  cursor.execute('select * from filesystem where parent="%s" '
                 'order by filename asc;'
                 % path)
  ents = []
  for row in cursor:
    ents.append(row['filename'])
  dir_cache[path] = (time.time(), ents)
  return ents


def GetFileVersions(path):
  cursor.execute('select * from versions where path="%s" order by epoch asc;'
                 % path)
  ents = []
  for row in cursor:
    ents.append((row['epoch'], row['seen_on'].split()))
  file_cache[path] = ents
  return ents


def myapp(environ, start_response):
  global cursor
  global cache

  start_response('200 OK', [('Content-Type', 'text/html')])
  form = cgi.parse(environ['wsgi.input'], environ=environ)
  
  results = []
  path = '/'
  if form.has_key('path'):
    path = form['path'][0]

  ents = GetDirectory(path)
  if ents:
    results.append('<h1>%s</h1>' % path)
    for ent in ents:
      if GetDirectory(os.path.join(path, ent)):
        results.append('[dir] '
                       '<a href="duplicity-viewer.fcgi?path=%s">%s</a><br/>'
                       %(os.path.join(path, ent), ent))
      else:
        results.append('[file] '
                       '<a href="duplicity-viewer.fcgi?path=%s">%s</a><br/>'
                       %(os.path.join(path, ent), ent))

  else:
    ents = GetFileVersions(path)
    results.append('<h1>%s (%d versions)</h1>' %(path, len(ents)))
    for (age, seen_on) in ents:
      results.append('%s (contained in: %s)<br/>'
                     %(datetime.datetime.fromtimestamp(age),
                       ', '.join(seen_on)))

  return results


if __name__ == '__main__':
  from fcgi import WSGIServer
  WSGIServer(myapp).run()
