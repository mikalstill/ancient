#!/usr/bin/python

# Collect readings

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import os
import time
import MySQLdb

import mplugin


COLLECTOR_DIR = 'collectors'


db = MySQLdb.connect(user = 'root', db = 'home')
cursor = db.cursor(MySQLdb.cursors.DictCursor)


for ent in os.listdir(COLLECTOR_DIR):
  if ent.endswith('.py'):
    print
    print '----------------------------------------------------------'
    print '%s: Running %s' %(datetime.datetime.now(), ent)
    try:
      plugin = mplugin.LoadPlugin(COLLECTOR_DIR, ent[:-3], log=None)
      plugin.Collect(cursor)
    except Exception, e:
      print '%s: Exception: %s' %(datetime.datetime.now(), e)
