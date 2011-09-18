#!/usr/bin/python

"""Business logic."""

import os
import time

import database
import track
import gflags


FLAGS = gflags.FLAGS

def GenerateRankSql(skips):
  """Generate the SQL to rank tracks."""

  return ('rand() + '
          '  (least(tracks.plays, 15) * 0.00005 * %d) - '
          '  (ifnull(usersummary.skips, 0) * 0.01 * %d) + '
          '  (to_days(now()) - '
          '   greatest(to_days(last_played), '
          '            to_days(last_skipped))) * '
          '            0.000001 * (5 - %d)'
          '  as idx'
          % (skips + 1, skips + 1, skips + 1))


class BusinessLogic(object):
  """Handle business logic like track selection."""

  def __init__(self, db, log):
    self.db = db
    self.log = log
    self.cache = {}

  def picktrack(self, client_id=-1, recent=False, skips=0, limit=1):
    """Pick a track for this client and make sure it exists."""

    returnable = []

    recent_sql = ''
    if recent:
      recent_sql = 'where (to_days(now()) - to_days(creation_time)) < 15'
      self.log('Request is for a recent track')
    sql = ('select *, %s from tracks left join usersummary on '
           'usersummary.user="%s" and usersummary.track_id = id '
           '%s order by idx desc limit %s;'
           %(GenerateRankSql(skips), 
             self.getclientsetting(client_id, 'user', 'shared'),
             recent_sql, limit))
    self.log('Bulk pick sql = %s' % sql)

    for row in self.db.GetRows(sql):
      self.log('Got %s' % row.keys())
      self.log('Considering %d, rank %f (plays %d, skips %s, last_played %s, '
               'last_skipped %s created %s, %d recent skips)'
               %(row['id'], row['idx'], row['plays'],
                 row['usersummary.skips'], row['last_played'],
                 row['last_skipped'], row['creation_time'],
                 skips))

      this_track = track.Track(self.db)
      this_track.FromId(row['id'])
      rendered = this_track.RenderValues()
      (rendered['mp3_url'],
       rendered['mp3_file']) = self.findMP3(rendered['id'],
                                            client_id=client_id)
      if rendered['mp3_url']:
        returnable.append(rendered)
      if len(returnable) == limit and limit == 1:
        return returnable[0]

    if returnable:
      return returnable

    self.log('Could not find an MP3')
    return {}

  def findMP3(self, id, client_id=-1):
    """Find a working MP3 file from a list of paths."""

    for row in self.db.GetRows('select path from paths where track_id=%d;'
                               % id):
      self.log('Considering id = %d: %s' %(id, row['path']))
      if row['path'].endswith('.mp3') and os.path.exists(row['path']):
        self.log('MP3 check: %s' % row['path'])
        mp3_source = self.getclientsetting(client_id, 'mp3_source', '')
        if mp3_source:
          mp3_url = ('%s/%s' %(mp3_source,
                               row['path'].replace(FLAGS.audio_path, '')))
        else:
          mp3_url = '/mp3/%s' % id

        self.log('MP3 ok: %s' % mp3_url)
        return (mp3_url, row['path'])

    self.log('No MP3 file found for id = %d' % id)
    return (None, None)

  def markplayed(self, id, client_id=-1):
    """Mark a track played in the database."""
    
    self.log('Marking %s played' % id)
    self.db.ExecuteSql('update tracks set plays = plays + 1, '
                       'last_played = NOW(), last_action = NOW() '
                       'where id=%s;'
                       % id)
    self.db.ExecuteSql('insert into events(timestamp, track_id, event) '
                       'values(now(), %s, "play");'
                       % id)
    self.db.ExecuteSql('commit;')

    user = self.getclientsetting(client_id, 'user', 'shared')
    self.db.ExecuteSql('insert ignore into usersummary (user, track_id, '
                       'plays, skips) values("%s", %s, 0, 0);'
                       %(user, id))
    self.db.ExecuteSql('commit;')
    self.db.ExecuteSql('update usersummary set plays=plays+1, updated=now() '
                       'where user="%s" and track_id=%s;'
                       %(user, id))
    self.db.ExecuteSql('commit;')

  def markskipped(self, id, skips, client_id=-1):
    """Mark a track as skipped in the database."""

    self.log('Marking %s skipped (%d skipped in a row)' % (id, skips))
    self.db.ExecuteSql('update tracks set skips = skips + 1, '
                       'last_skipped = NOW(), last_action = NOW() '
                       'where id=%s;'
                       % id)
    self.db.ExecuteSql('insert into events(timestamp, track_id, event) '
                       'values(now(), %s, "skip");'
                       % id)
    self.db.ExecuteSql('commit;')

    user = self.getclientsetting(client_id, 'user', 'shared')
    self.db.ExecuteSql('insert ignore into usersummary (user, track_id, '
                       'plays, skips) values("%s", %s, 0, 0);'
                       %(user, id))
    self.db.ExecuteSql('commit;')
    self.db.ExecuteSql('update usersummary set skips=skips+1, updated=now() '
                       'where user="%s" and track_id=%s;'
                       %(user, id))
    self.db.ExecuteSql('commit;')

  def getclientsetting(self, client_id, setting, default):
    """Lookup a setting for a client."""
 
    self.log('Lookup for value %s for client %s' %(setting, client_id)) 
    if (client_id, setting) in self.cache:
      (when, value) = self.cache[(client_id, setting)]
      if time.time() - when < 900:
        self.log('Returning cached value %s for setting %s, client %s'
                 %(value, setting, client_id))
        return value

    client_settings = self.db.GetOneRow('select * from clients where id=%s;'
                                        % client_id)
    if not client_settings:
      self.log('Client not found: %s' % client_id)
    if not client_settings.has_key(setting):
      self.log('Client %s has not set setting: %s' %(client_id, setting))

    if client_settings and client_settings.has_key(setting):
      self.cache[(client_id, setting)] = (time.time(), client_settings[setting])
      return client_settings[setting]

    self.log('Returning default value for setting: %s' % setting)
    self.cache[(client_id, setting)] = (time.time(), default)
    return default

  def setclientsetting(self, client_id, setting, value):
    """Set a user variable."""
    
    value = value.replace('%2F', '/').replace('%3A', ':')
    if value == '':
      value = None

    self.log('Updating %s to %s for %s' %(setting, value, client_id))
    self.db.ExecuteSql('insert ignore into clients(id, createtime) '
                       'values(%s, now());'
                       % client_id)
    self.db.ExecuteSql('update clients set %s="%s" '
                       'where id=%s;'
                       %(setting, value, client_id))
    self.db.ExecuteSql('commit;')

    self.cache[(client_id, setting)] = (time.time(), value)
