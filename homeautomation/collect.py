#!/usr/bin/python

# Collect readings

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import os
import time
import MySQLdb

import mplugin


COLLECTOR_DIR = 'collectors'


db = MySQLdb.connect(user = 'root', db = 'home')
cursor = db.cursor(MySQLdb.cursors.DictCursor)


while True:
  for ent in os.listdir(COLLECTOR_DIR):
    if ent.endswith('.py'):
      plugin = mplugin.LoadPlugin(COLLECTOR_DIR, ent[:-3], log=None)
      plugin.Collect(cursor)

  time.sleep(10)
