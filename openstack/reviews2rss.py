#!/usr/bin/python

# Take gerrit status feeds and turn them into an RSS feed

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import base64
import datetime
import json
import re
import time
import MySQLdb

from dateutil import tz

import gflags
import sql

import dbcachingexecute

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

  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  for change in ParseReviewList(db, component, status):
    # Attempt to parse the time in the review
    # 2012-05-13 21:29:57 UTC
    updated_at = datetime.datetime(*time.strptime(change['lastUpdated'],
                                                  '%Y-%m-%d %H:%M:%S %Z')[0:5])
    from_zone = tz.tzutc()
    to_zone = tz.tzlocal()
    utc_updated_at = updated_at.replace(tzinfo=from_zone)
    local_updated_at = utc_updated_at.astimezone(to_zone)

    print (' ... Time %s converted to %s UTC and %s local'
           %(change['lastUpdated'], utc_updated_at, local_updated_at))

    files = []
    for filename in GetFileList(db,
                                time.mktime(local_updated_at.timetuple()),
                                change['change']):
      files.append(filename)
    change['files'] = '\n&lt;li&gt;'.join(files)

    # Save to the DB
    cursor.execute('insert ignore into changes (changeid, timestamp, parsed) '
                   'values("%s", %s, "%s");'
                   %(change['change'],
                     sql.FormatSqlValue('timestamp', local_updated_at),
                     base64.encodestring(repr(change))))
    cursor.execute('commit;')

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
  for l in dbcachingexecute.Execute(db, time.time(),
                                    'gerrit_query',
                                    'ssh review.openstack.org gerrit query %s',
                                    'status:%s project:%s' %(status,
                                                             component),
                                    cleanup=True):
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


def GetFileList(db, last_change, changeid):
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  for l in dbcachingexecute.Execute(db, last_change,
                                    'gerrit_query_change',
                                    ('ssh review.openstack.org gerrit query '
                                     'change:%s --current-patch-set --files'),
                                    changeid):
    if l.startswith('      file: '):
      filename = l.split(': ')[-1]
      if filename != '/COMMIT_MSG':
        top_dirs = '_'.join(filename.split('/')[:2]).replace('.', '_').replace('-', '_')
        try:
          cursor.execute('create table files_%s (path varchar(500), '
                         'timestamp datetime, epoch int, '
                         'changeid varchar(100), '
                         'primary key(path, changeid, timestamp), '
                         'index(timestamp), index(epoch)) engine=innodb;'
                         % top_dirs)
        except:
          pass

        sql = ('insert ignore into files_%s '
               '(path, timestamp, epoch, changeid) '
               'values ("%s", now(), %d, "%s");'
               %(top_dirs, filename, int(time.time()), changeid))
        # print sql
        cursor.execute(sql)
        cursor.execute('commit;')

        yield filename


def Reviews(db, component):
  cursor = db.cursor(MySQLdb.cursors.DictCursor)
  for l in dbcachingexecute.Execute(db, time.time() - 300,
                                    'gerrit_query_approvals_json',
                                    ('ssh review.openstack.org gerrit query '
                                     'project:%s --all-approvals --patch-sets '
                                     '--format JSON'),
                                    component, cleanup=True):

    try:
      d = json.loads(l)
    except:
      continue

    for ps in d.get('patchSets', {}):
      for review in ps.get('approvals', []):
        # Deliberately leave the timezone alone here so its consistant with
        # reports others generate.
        updated_at = datetime.datetime.fromtimestamp(review['grantedOn'])

        print '%s review by %s at %d' %(d['id'],
                                        review['by'].get('username', 'unknown'),
                                        review['grantedOn'])
        
        cursor.execute('insert ignore into reviews '
                       '(changeid, username, timestamp) values '
                       '("%s", "%s", %s);'
                       %(d['id'], review['by'].get('username', 'unknown'),
                         sql.FormatSqlValue('timestamp', updated_at)))
        cursor.execute('commit;')


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

  Reviews(db, 'openstack/nova')
  CreateRss(db, 'openstack/nova', 'open')
  CreateRss(db, 'openstack/nova', 'merged')

