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


CURRENT_SCHEMA='19'


class FormatException(Exception):
  """ FormatException -- Used for reporting failures for format DB values """


def Normalize(value):
  normalized = unicodedata.normalize('NFKD', unicode(value))
  normalized = normalized.encode('ascii', 'ignore')
  return normalized


class Database:
  """Handle all the details of MySQL."""

  def __init__(self, dbname=None, dbuser=None, dbpassword=None,
               dbhost=None):
    self.OpenConnection(dbname=dbname, dbuser=dbuser, dbpassword=dbpassword,
                        dbhost=dbhost)
    self.CheckSchema()

  def OpenConnection(self, dbname=None, dbuser=None, dbpassword=None,
                     dbhost=None):
    """Determine what parameters to use, and connect to MySQL."""

    if dbname:
      # This override makes writing unit tests simpler
      self.db_name = dbname
    else:
      self.db_name = FLAGS.db_name

    if dbuser:
      self.user = dbuser
    else:
      self.user = FLAGS.db_user

    if dbpassword:
      self.password = dbpassword
    else:
      self.password = FLAGS.db_password

    if dbhost:
      self.host = dbhost
    else:
      self.host = FLAGS.db_host

    self._DoConnect()

  def _DoConnect(self):
    """Actually make a connection to the database."""

    try:
      self.db_connection = MySQLdb.connect(host = self.host,
                                           user = self.user,
                                           passwd = self.password,
                                           db = self.db_name)
    except Exception, e:
      print 'Could not connect to the MySQL server: %s' % e
      sys.exit(1)

  def GetCursor(self):
    """Return a database cursor."""

    try:
      cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    except:
      self._DoConnect()
      cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)

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
    for table in ['settings']:
      if not self.TableExists(table):
        self.CreateTable(table)

    # Check the schema version
    self.version = self.GetSetting('schema')
    if (self.version is None) or (int(self.version) < int(CURRENT_SCHEMA)):
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

    row = self.GetOneRow('select value from settings where '
                         'name="%s";' % name)
    if row == None:
      return None
    return row['value']

  def GetSettingWithDefault(self, name, default):
    """GetSettingWithDefault -- get a setting with a default value"""

    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute('select value from settings where '
                   'name="%s";' % name)
    if cursor.rowcount != 0:
      retval = cursor.fetchone()
      cursor.close()
      return retval['value']
    else:
      self.db_connection.query('insert into settings (name, value) '
                               'values("%s", "%s");' %(name, default))
      return default

  def WriteSetting(self, name, value):
    """WriteSetting -- write a setting to the database"""
    self.WriteOneRow('settings', 'name',
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
      print
      print 'Problem SQL: %s' % sql
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
      return '%d' % value

    if value == '':
      return '""'

    if value is None:
      return 'NULL'

    try:
      return '"%s"' % Normalize(value).replace('"', '""')
    except Exception, e:
      raise FormatException('Could not format string value %s = %s (%s)'
                            %(name, value, e))

  def WriteOneRow(self, table, key_col, dict):
    """WriteOneRow -- use a dictionary to write a row to the specified table"""

    row_count = 0
    try:
      cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
      cursor.execute('select %s from %s where %s = "%s"'
                     %(key_col, table, key_col, dict[key_col]))
      row_count = cursor.rowcount
      cursor.close()
    except:
      pass

    if dict.has_key(key_col) and row_count > 0:
      vals = []
      for col in dict:
        val = '%s=%s' %(col, self.FormatSqlValue(col, dict[col]))
        vals.append(val)

      sql = ('update %s set %s where %s="%s";' %(table, ','.join(vals),
                                                 key_col, dict[key_col]))

    else:
      vals = []
      for col in dict:
        val = self.FormatSqlValue(col, dict[col])
        vals.append(val)

      sql = ('insert into %s (%s) values(%s);'
             %(table, ','.join(dict.keys()), ','.join(vals)))


    self.db_connection.query(sql)
    self.db_connection.query('commit;')

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

    if self.version == '0' or self.version is None:
      self.version = '1'

    if self.version == '1':
      self.db_connection.query('create table tracks (artist varchar(255), '
                               'album varchar(255), number int, '
                               'song varchar(255), paths text, plays int, '
                               'skips int, tags varchar(255), '
                               'primary key(artist, album, number, song), '
                               'key(tags));')

      self.version = '2'

    if self.version == '2':
      self.db_connection.query('alter table tracks '
                               'modify column song varchar(300);')
      self.version = '3'

    if self.version == '3':
      self.db_connection.query('alter table tracks '
                               'add column last_played datetime;')
      self.version = '4';

    if self.version == '4':
      self.db_connection.query('alter table tracks '
                               'add column last_skipped datetime, '
                               'add column last_action datetime;')
      self.version = '5';

    if self.version == '5':
      self.db_connection.query('alter table tracks '
                               'add column id int not null auto_increment, '
                               'add key(id);')
      self.version = '6';

    if self.version == '6':
      self.db_connection.query('create table clients ('
                               'id int not null auto_increment, '
                               'mp3_source varchar(255), '
                               'primary key(id));')
      self.version = '7'

    if self.version == '7':
      self.db_connection.query('alter table clients '
                               'add column createtime datetime;')
      self.version = '8';

    if self.version == '8':
      self.db_connection.query('alter table clients '
                               'add column requests int;')
      self.version = '9';

    if self.version == '9':
      self.db_connection.query('alter table tracks '
                               'add column creation_time datetime;')
      self.version = '10';

    if self.version == '10':
      self.db_connection.query('alter table tracks drop primary key;')
      self.db_connection.query('alter table tracks add primary key(id);')
      self.db_connection.query('alter table settings '
                               'modify column name varchar(100);')
      self.db_connection.query('alter table settings add primary key(name);')
      self.version = '11';

    if self.version == '11':
      self.db_connection.query('create table events ('
                               'timestamp datetime, user varchar(32), '
                               'track_id int, event varchar(30));')
      self.version = '12';

    if self.version == '12':
      self.db_connection.query('create table tags ('
                               'tag varchar(32), track_id int, '
                               'primary key(tag, track_id));')

      # Move the data from the old location to the new one
      for row in self.GetRows('select id, tags from tracks '
                              'where tags <> "[]";'):
        for tag in eval(row['tags']):
          self.ExecuteSql('insert into tags(tag, track_id) '
                          'values("%s", %d);'
                          %(tag, row['id']))
          self.ExecuteSql('commit;')

      self.ExecuteSql('alter table tracks drop column tags;')

      self.version = '13'

    if self.version == '13':
      self.db_connection.query('create table paths ('
                               'path varchar(1000), track_id int, '
                               'primary key(path));')

      # Move the data from the old location to the new one
      failed = 0
      for row in self.GetRows('select id, paths from tracks '
                              'where paths <> "[]";'):
        for path in eval(row['paths']):
          try:
            self.ExecuteSql('insert into paths(path, track_id) '
                            'values("%s", %d);'
                            %(path, row['id']))
            self.ExecuteSql('commit;')
          except:
            print '%s Failed to migrate path %s' %(datetime.datetime.now(),
                                                   path)
            failed += 1

      if failed > 5:
        print 'Too many failed migrations'
        sys.exit(1)

      self.ExecuteSql('alter table tracks drop column paths;')

      self.version = '14'

    if self.version == '14':
      self.ExecuteSql('alter table paths add column duration float, '
                      'add column active boolean;')
      self.ExecuteSql('commit;')
      self.version = '15'

    if self.version == '15':
      self.ExecuteSql('alter table paths drop column active, '
                      'add column error varchar(255);')
      self.ExecuteSql('commit;')
      self.version = '16'

    if self.version == '16':
      self.ExecuteSql('create table art(artist varchar(255), '
                      'album varchar(255), art longtext, '
                      'primary key(artist, album));')
      self.ExecuteSql('commit;') 

      self.version = '17'

    if self.version == '17':
      self.ExecuteSql('alter table art add column error varchar(255);')
      self.ExecuteSql('commit;')
      self.version = '18'

    if self.version == '18':
      self.ExecuteSql('alter table tracks add column active int, '
                      'add column comment varchar(255);')
      self.ExecuteSql('commit;')
      self.version = '19'

    self.db_connection.query('commit;')
    self.WriteSetting('schema', self.version)

  def ExecuteSql(self, sql):
    """ ExecuteSql -- execute some SQL and return the number of rows affected
    """

    cursor = self.db_connection.cursor(MySQLdb.cursors.DictCursor)
    cursor.execute(sql)

    changed = cursor.rowcount
    cursor.close()

    return changed
