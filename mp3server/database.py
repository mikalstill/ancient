#!/usr/bin/python

# Copyright (C) Michael Still (mikal@stillhq.com) 2006, 2007, 2008
# Released under the terms of the GNU GPL v2

import datetime
import MySQLdb
import os
import sys
import traceback
import unicodedata

import gflags
FLAGS = gflags.FLAGS
gflags.DEFINE_string('db_host', 'localhost',
                     'The name of the host the MySQL database is on')
gflags.DEFINE_string('db_user', 'root',
                     'The name of the user to connect to the database with')
gflags.DEFINE_string('db_password', '',
                     'The password for the database user')
gflags.DEFINE_string('db_name', 'mp3server',
                     'The name of the database which MythNetTV uses')

gflags.DEFINE_boolean('db_debugging', False,
                      'Output debugging messages for the database')


CURRENT_SCHEMA='1'

class Database:
  """Handle all the details of MySQL."""

  def __init__(self, dbname=None, dbuser=None, dbpassword=None,
               dbhost=None):
    self.OpenConnection(dbname=dbname, dbuser=dbuser, dbpassword=dbpassword,
                        dbhost=dbhost)
    self.CheckSchema()

  def OpenConnection(self, dbname=None, dbuser=None, dbpassword=None,
                     dbhost=None):
    """OpenConnection -- parse the MythTV config file and open a connection
    to the MySQL database"""

    global HAVE_WARNED_OF_DEFAULTS

    if dbname:
      # This override makes writing unit tests simpler
      db_name = dbname
    else:
      db_name = FLAGS.db_name

    if dbuser:
      user = dbuser
    else:
      user = FLAGS.db_user

    if dbpassword:
      password = dbpassword
    else:
      password = FLAGS.db_password

    if dbhost:
      host = dbhost
    else:
      host = FLAGS.db_host

    # Open the DB connection
    try:
      self.db_connection = MySQLdb.connect(host = host,
                                           user = user,
                                           passwd = password,
                                           db = db_name)
    except Exception, e:
      print 'Could not connect to the MySQL server: %s' % e
      sys.exit(1)

  def TableExists(self, table):
    """TableExists -- check if a table exists"""

    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    try:
      cursor.execute('describe %s;' % table)

    except MySQLdb.Error, (errno, errstr):
      if errno == 1146:
        return False
      else:
        print 'Error %d: %s' %(errno, errstr)
        sys.exit(1)

    cursor.close()
    return True

  def CheckSchema(self):
    """CheckSchema -- ensure we're running the latest schema version"""

    # Check if we even have tables
    for table in ['settings']
      if not self.TableExists(table):
        self.CreateTable(table)

    # Check the schema version
    self.version = self.GetSetting('schema')
    if int(self.version) < int(CURRENT_SCHEMA):
      print 'Updating tables'
      self.UpdateTables()

    elif int(self.version) > int(CURRENT_SCHEMA):
      print 'The database schema is newer than this version of the code, '
      print 'it seems like you might need to upgrade?'
      print
      print 'Current = %s' % CURRENT_SCHEMA
      print 'Database = %s' % self.version
      sys.exit(1)

  def GetSetting(self, name):
    """GetSetting -- get the current value of a setting"""

    row = self.GetOneRow('select value from mythnettv_settings where '
                         'name="%s";' % name)
    if row == None:
      return None
    return row['value']

  def GetSettingWithDefault(self, name, default):
    """GetSettingWithDefault -- get a setting with a default value"""

    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('select value from mythnettv_settings where '
                   'name="%s";' % name)
    if cursor.rowcount != 0:
      retval = cursor.fetchone()
      cursor.close()
      return retval['value']
    else:
      self.db_connection.query('insert into mythnettv_settings (name, value) '
                               'values("%s", "%s");' %(name, default))
      self.Log('Settings value %s defaulted to %s' %(name, default))
      return default

  def WriteSetting(self, name, value):
    """WriteSetting -- write a setting to the database"""
    self.WriteOneRow('mythnettv_settings', 'name',
                     {'name': name, 'value': value})

  def GetOneRow(self, sql):
    """GetOneRow -- get one row which matches a query"""

    try:
      cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
      cursor.execute(sql)
      retval = cursor.fetchone()
      cursor.close()

      if retval == None:
        if FLAGS.db_debugging:
          print 'Database: no result for %s' % sql
        return retval

      for key in retval.keys():
        if retval[key] == None:
          del retval[key]

      return retval

    except Exception, e:
      print 'Database error:'
      traceback.print_exc()
      sys.exit(1)

  def GetRows(self, sql):
    """GetRows -- return a bunch of rows as an array of dictionaries"""

    retval = []
    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(sql)

    for i in range(cursor.rowcount):
      row = cursor.fetchone()
      retval.append(row)

    return retval

  def FormatSqlValue(self, name, value):
    """FormatSqlValue -- some values get escaped for SQL use"""

    if type(value) == datetime.datetime:
      return 'STR_TO_DATE("%s", "%s")' \
             %(value.strftime('%a, %d %b %Y %H:%M:%S'),
               '''%a, %d %b %Y %H:%i:%s''')
    if name == 'date':
      return 'STR_TO_DATE("%s", "%s")' %(value, '''%a, %d %b %Y %H:%i:%s''')
    if type(value) == long or type(value) == int:
      return value
    if value == None:
      return 'NULL'

    try:
      return '"%s"' % Normalize(value).replace('"', '""')
    except Exception, e:
      raise FormatException('Could not format string value %s = %s (%s)'
                            %(name, value, e))

  def WriteOneRow(self, table, key_col, dict):
    """WriteOneRow -- use a dictionary to write a row to the specified table"""

    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('select %s from %s where %s = "%s"' \
                   %(key_col, table, key_col, dict[key_col]))

    if cursor.rowcount > 0:
      self.Log('Updating %s row with %s of %s' %(table, key_col,
                                                 dict[key_col]))
      vals = []
      for col in dict:
        val = '%s=%s' %(col, self.FormatSqlValue(col, dict[col]))
        vals.append(val)

      sql = 'update %s set %s where %s="%s";' %(table, ','.join(vals),
                                                key_col, dict[key_col])

    else:
      self.Log('Creating %s row with %s of %s' %(table, key_col,
                                                 dict[key_col]))
      vals = []
      for col in dict:
        val = self.FormatSqlValue(col, dict[col])
        vals.append(val)

      sql = 'insert into %s (%s) values(%s);' \
             %(table, ','.join(dict.keys()), ','.join(vals))

    cursor.close()
    self.db_connection.query(sql)

  def CreateTable(self, tablename):
    """CreateTable -- a table has been found to be missing, create it with
    the current schema"""

    print 'Info: Creating %s table' % tablename
    if tablename == 'settings':
      self.db_connection.query('create table settings (name text, '
                               'value text);')
      self.db_connection.query('insert into settings (name, value) '
                               'values("schema", 0);')

    else:
      print 'Error: Don\'t know how to create %s' % tablename
      sys.exit(1)

  def UpdateTables(self):
    """Handle schema upgrades"""

    if self.version == '0':
      self.Log('Upgrading schema from 0 to 1')
      self.version = '1'
