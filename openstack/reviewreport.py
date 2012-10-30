#!/usr/bin/python

# Summarize reviews for the last 14 days

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import MySQLdb

import gflags
import sql

FLAGS = gflags.FLAGS
gflags.DEFINE_string('dbuser', 'openstack', 'DB username')
gflags.DEFINE_string('dbname', 'openstack_gerrit', 'DB name')
gflags.DEFINE_string('dbpassword', '', 'DB password')


USERS = ['mikalstill', 'vishvananda', 'johannes.erdfelt', 'markmc', 'smoser',
         'zulcss']


if __name__ == '__main__':
  # Parse flags
  try:
    argv = FLAGS(sys.argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  db = MySQLdb.connect(user = FLAGS.dbuser,
                       db = FLAGS.dbname,
                       passwd = FLAGS.dbpassword)
  cursor = db.cursor(MySQLdb.cursors.DictCursor)

  reviewers = []
  cursor.execute('select distinct(username) from reviews;')
  for row in cursor:
    if row['username'] not in ['jenkins', 'smokestack']:
      reviewers.append(row['username'])
  reviewers.sort()

  reviews = {}
  sum = {}
  for user in reviewers:
    reviews[user] = {}
    sum[user] = 0
    cursor.execute('select distinct(date(timestamp)) as date, '
                   'count(*) as count from reviews where username="%s" '
                   'group by date order by timestamp desc limit 14;'
                   % user)

    for row in cursor:
      d = row['date']
      if d:
        d_str = '%04d-%02d-%02d' %(d.year, d.month, d.day)
        reviews[user][d_str] = row['count']

  now = datetime.datetime.now()
  d = datetime.datetime(now.year, now.month, now.day)
  one_day = datetime.timedelta(days=1)

  sys.stdout.write('\t')  
  for user in USERS:
    sys.stdout.write('\t%s' % user[:5])
  sys.stdout.write('\tTOTAL\n')

  for i in xrange(14):
    d_str = '%04d-%02d-%02d' %(d.year, d.month, d.day)
    d_sums = {}
    day_total = 0
    sys.stdout.write(d_str)
    for user in reviewers:
      d_sums[user] = 0
      count = reviews[user].get(d_str, '')
      if count:
        sum[user] += count
        d_sums[user] += count
        day_total += count

    for user in USERS:
      if d_sums[user] > 0:
        sys.stdout.write('\t%s' % d_sums[user])
      else:
        sys.stdout.write('\t')

    sys.stdout.write('\t%s\n' % day_total)
    d -= one_day

  sys.stdout.write('\t')  
  for user in USERS:
    sys.stdout.write('\t-----')
  sys.stdout.write('\n')

  sys.stdout.write('\t')  
  for user in USERS:
    sys.stdout.write('\t%d' % sum[user])
  sys.stdout.write('\n')

  print
  sys.stdout.write('All reviewers seen (%d seen): ' % len(sum))
  keys = sum.keys()
  keys.sort()
  for reviewer in keys:
    if sum[reviewer] > 0:
      sys.stdout.write('%s (%d) ' %(reviewer, sum[reviewer]))
  print
