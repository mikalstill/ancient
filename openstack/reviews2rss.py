#!/usr/bin/python

# Take gerrit status feeds and turn them into an RSS feed

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import base64
import datetime
import re
import subprocess
import time
import MySQLdb

import gflags


FLAGS = gflags.FLAGS
gflags.DEFINE_string('dbuser', 'openstack', 'DB username')
gflags.DEFINE_string('dbname', 'openstack_gerrit', 'DB name')
gflags.DEFINE_string('dbpassword', '', 'DB password')


def CreateRss(db, component, status):
  filename = '%s_%s.rss' %(component, status)
  f = open(filename, 'w')

  f.write("""<?xml version="1.0" ?>
  <rss version="2.0">
  <channel>
     <title>%s code reviews (%s)</title>
     <link>https://review.openstack.org/</link>
     <description>openstack/nova code reviews</description>
     <language>en</language>
     <generator>stillhq.com</generator>
     <ttl>180</ttl>
     <docs>http://blogs.law.harvard.edu/tech/rss</docs>
  """ %(component, status))

  for change in ParseReviewList(db, component, status):
    files = []
    for filename in GetFileList(db, change['change']):
      files.append(filename)
    change['files'] = '\n&lt;li&gt;'.join(files)

    f.write("""  <item>
      <title>%(component)s: %(subject)s (%(status)s)</title>
      <pubDate>%(lastUpdated)s</pubDate>
      <description>
      %(description)s

      &lt;br/&gt;&lt;br/&gt;

      &lt;b&gt;Files:&lt;/b&gt;&lt;br/&gt;
      &lt;li&gt;%(files)s

      </description>
      <link>%(url)s</link>
      <guid isPermaLink="true">%(url)s</guid>
    </item>
    """ % change)

  f.write("""</channel>
  </rss>
  """)
  f.close()


def ParseReviewList(db, component, status):
  keys = []
  values = {}
  for l in CachingExecute(db, ('ssh review.openstack.org gerrit query '
                               'status:%s project:%s' %(status, component))):
    l = l.strip().rstrip()

    if len(l) == 0 and values and 'subject' in values:
      description = []
      for key in keys:
        v = values[key]

        if key == 'url':
          description.append('&lt;b&gt;%s:&lt;/b&gt; '
                             '&lt;a href="%s"&gt;%s&lt;/a&gt;'
                             %(key, v, v))
        else:
          description.append('&lt;b&gt;%s:&lt;/b&gt; %s' %(key, v))

      values['description'] = '&lt;br/&gt;'.join(description)

      values['component'] = component
      yield values

      values = {}
      keys = []

    elif l.startswith('change '):
      values['change'] = ' '.join(l.split(' ')[1:])

    else:
      elems = l.split(': ')
      values[elems[0]] = ': '.join(elems[1:])
      if elems[0] not in keys:
        keys.append(elems[0])


def GetFileList(db, changeid):
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  for l in CachingExecute(db, ('ssh review.openstack.org gerrit query '
                               'change:%s --current-patch-set --files'
                               % changeid)):
    if l.startswith('      file: '):
      filename = l.split(': ')[-1]
      if filename != '/COMMIT_MSG':
        yield filename


def CachingExecute(db, cmd):
  print 'Executing: %s' % cmd

  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  cursor.execute('select timestamp, epoch, result from commands '
                 'where cmd="%s" order by timestamp desc limit 1;'
                 % cmd)
  if cursor.rowcount > 0:
    row = cursor.fetchone()
    if time.time() - row['epoch'] < 300:
      print ' ... Using cached result'
      return base64.decodestring(row['result']).split('\n')

  p = subprocess.Popen(cmd,
                       shell=True,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)
  out = p.stdout.read()
  print ' ... Got %d bytes' % len(out)

  cursor.execute('insert into commands (cmd, timestamp, epoch, result) values '
                 '("%s", now(), %d, "%s");'
                 %(cmd, int(time.time()), base64.encodestring(out)))
  cursor.execute('commit;')
  return out.split('\n')


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  print 'DB connection: %s/%s to %s' %(FLAGS.dbuser, FLAGS.dbpassword,
                                       FLAGS.dbname)
  db = MySQLdb.connect(user = FLAGS.dbuser,
                       db = FLAGS.dbname,
                       passwd = FLAGS.dbpassword)

  CreateRss(db, 'openstack/nova', 'open')
  CreateRss(db, 'openstack/nova', 'merged')
