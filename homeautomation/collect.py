#!/usr/bin/python

# Collect readings

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import lockfile
import os
import time
import MySQLdb

import gflags
import mplugin


FLAGS = gflags.FLAGS
gflags.DEFINE_string('collector_dir', 'collectors',
                     'Directory containing collector plugins')
gflags.DEFINE_string('dbuser', 'home', 'DB username')
gflags.DEFINE_string('dbname', 'homeautomation', 'DB name')
gflags.DEFINE_string('only', '', 'Only run these plugins (space separated)')
gflags.DEFINE_boolean('lock', True, 'Should we use locking?')


def main(argv):
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  lock = lockfile.FileLock('/tmp/collector2')
  if FLAGS.lock:
    if lock.is_locked():
      print 'There is already another collector running'
      sys.exit(1)

    lock.acquire()

  try:
    db = MySQLdb.connect(user = FLAGS.dbuser, db = FLAGS.dbname)
    cursor = db.cursor(MySQLdb.cursors.DictCursor)

    plugins = os.listdir(FLAGS.collector_dir)
    if FLAGS.only:
      plugins = []
      for plugin in FLAGS.only.split(' '):
        plugins.append('%s.py' % plugin)

    for ent in plugins:
      if ent.endswith('.py'):
        print
        print '----------------------------------------------------------'
        print '%s: Running %s' %(datetime.datetime.now(), ent)
        try:
          plugin = mplugin.LoadPlugin(FLAGS.collector_dir, ent[:-3], log=None)
          plugin.Collect(cursor)
        except Exception, e:
          print '%s: Exception: %s' %(datetime.datetime.now(), e)

  finally:
    if FLAGS.lock:
      lock.release()

if __name__ == "__main__":
  main(sys.argv)
