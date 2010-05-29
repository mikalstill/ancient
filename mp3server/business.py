#!/usr/bin/python

"""Business logic."""

import os

import database
import track
import gflags


FLAGS = gflags.FLAGS

def GenerateRankSql(skips):
  """Generate the SQL to rank tracks."""

  return ('rand() + '
          '  (least(tracks.plays, 15) * 0.00005 * %d) - '
          '  (usersummary.skips * 0.01 * %d) + '
          '  (to_days(now()) - '
          '   greatest(to_days(last_played), '
          '            to_days(last_skipped))) * '
          '            0.000001 * (5 - %d)'
          '  as idx'
          % (skips + 1, skips + 1, skips + 1))


def GetClientSetting(db, client_id, setting):
  """Lookup a setting for a client."""

  # TODO(mikal): cache this?
  client_settings = db.GetOneRow('select * from clients where id=%s;'
                                 % client_id)
  if client_settings and client_settings.has_key(setting):
    return client_settings[setting]
  return None


class BusinessLogic(object):
  """Handle business logic like track selection."""

  def __init__(self, db, log):
    self.db = db
    self.log = log

  def picktrack(self, client_id=-1, recent=False, skips=0, limit=1):
    """Pick a track for this client and make sure it exists."""

    returnable = []

    recent_sql = ''
    if recent:
      recent_sql = 'where (to_days(now()) - to_days(creation_time)) < 15'
      self.log('Request is for a recent track')
    sql = ('select %s from tracks join usersummary on '
           'usersummary.user="%s" and usersummary.track_id = id '
           '%s order by idx desc limit 100;'
           %(GenerateRankSql(skips), 
             GetClientSetting(self.db, client_id, 'user'),
             recent_sql))

    for row in self.db.GetRows(sql):
      self.log('Considering %d, rank %f (plays %d, skips %s, last_played %s, '
               'last_skipped %s created %s, %d recent skips)'
               %(row['id'], row['idx'], row['plays'], row['skips'],
                 row['last_played'], row['last_skipped'],
                 row['creation_time'],
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
        mp3_source = GetClientSetting(self.db, client_id, 'mp3_source')
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

    user = GetClientSetting(self.db, client_id, 'user')
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

    user = GetClientSetting(self.db, client_id, 'user')
    self.db.ExecuteSql('insert ignore into usersummary (user, track_id, '
                       'plays, skips) values("%s", %s, 0, 0);'
                       %(user, id))
    self.db.ExecuteSql('commit;')
    self.db.ExecuteSql('update usersummary set skips=skips+1, updated=now() '
                       'where user="%s" and track_id=%s;'
                       %(user, id))
    self.db.ExecuteSql('commit;')
