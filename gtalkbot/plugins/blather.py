#!/usr/bin/python2.4
#
# Copyright (c) Michael Still 2007, released under the terms of the GNU GPL v2

"""blather.py -- a very simple replacement for Twitter that works with blosxom
"""

import cPickle
import datetime
import os
import time
import unicodedata

# This is the location to write the blog files to, you'll need to customize it
_prefix = '/data/stillhq.com/blather/'

# Helper methods
class DataStore:
  """ Handle everything about storing these messages """

  def __init__(self, dt = datetime.datetime.now()):
    self.filename = dt.strftime('%Y%m%d')
    self.data = {}
    self.dt = dt

    # Load the pickle for this day (if it exists)
    self.p_filename = '%s%s.pkl' %(_prefix, self.filename)
    if os.path.exists(self.p_filename):
      p_file = open(self.p_filename, 'r')
      self.data = cPickle.load(p_file)
      p_file.close()
    else:
      self.data['upto'] = 0

  def AddMessage(self, line, type='post', dt=datetime.datetime.now()):
    # Possible types are post, and status. Status is used for FB and twitter

    encoded_line = unicodedata.normalize('NFKD', unicode(line)).encode('ascii', 'ignore')
    i = self.data['upto'] + 1
    self.data[i] = ('<b>%s</b>: %s<br/>'
                    %(dt.strftime('%H:%M'),
                      encoded_line))
    self.data['type-%s' % i] = type 
    self.data['upto'] += 1
    self.dt = dt

  def RemoveMessage(self, number):
    self.data[number] = ''
    self.data['type-%s' % number] = 'removed'

  def ListMessages(self):
    retval = ''
    for i in range(1, self.data['upto'] + 1):
      retval = '%s[%d] %s\n' %(retval, i, self.data[i])
    return retval

  def FindMessagesOfType(self, t):
    for i in range(1, self.data['upto'] + 1):
      if self.data['type-%s' % i] == t:
         yield i

  def GetMessage(self, number):
    return self.data[number]

  def ChangeMessage(self, number, text):
    self.data[number] = text

  EXTN_MAP = {'post': 'blog',
              'status': 'sblog',
              'search': 'gblog'}
  def Save(self):
    p_file = open(self.p_filename, 'w')
    cPickle.dump(self.data, p_file)
    p_file.close()

    # Determine what messages to write
    extns = {}

    for i in range(1, self.data['upto'] + 1):
      try:
        encoded_data = unicodedata.normalize('NFKD',
                         self.data[i]).encode('ascii', 'ignore')
      except:
        encoded_data = self.data[i]

      t = self.data.get('type-%d' % i, 'post')
      extns.setdefault(t, [])
      extns[t].append(encoded_data)

    # And then churn out some HTML
    epoch = time.mktime(self.dt.timetuple())
    for extn in extns:
      filename = '%s%s.%s' %(_prefix, self.filename, self.EXTN_MAP.get(extn, extn))
      b_file = open(filename, 'w')

      if extn == 'post':
        b_file.write('Blathering for %s\n%s\n\n[btags: ]\n'
                     %(self.dt.strftime('%A, %d %B %Y'),
                     '\n'.join(extns[extn])))
      else:
        b_file.write('\n'.join(extns[extn]))

      b_file.close()
      os.utime(filename, (epoch, epoch))


def Verbs():
  """Verbs -- return the verbs which this module supports"""
  return ['b', 'l', 'e', 'd']

def Status():
  """Status -- suggest a message to display as the status string for the bot"""
  return 'Blathering away'

def Help(verb):
  """Help -- display help for a verb"""

  if verb == 'b':
    return 'Use this to have whatever you say blogged in the blather category'
  if verb == 'l':
    return 'List blathers for today'
  if verb == 'e':
    return 'Edit a blather for today (format is <number> <new text>)'
  if verb == 'd':
    return 'Delete a blather for today (format is <number>)'
  return ''

def Command(verb, line):
  """Command -- execute a given verb with these arguments"""

  ds = DataStore()
  if verb == 'b':
    ds.AddMessage(line)
  elif verb == 'l':
    return ds.ListMessages()

  elif verb == 'e':
    number = line.split(' ')[0]
    text = line.split(' ')[1:]
    ds.ChangeMessage(int(number), ' '.join(text))

  elif verb == 'd':
    number = line.split(' ')[0]
    ds.RemoveMessage(int(number))

  ds.Save()
  return "Done"
    
def Cleanup():
  """Cleanup -- you're about to be unloaded"""
  return

# Do initialization here
print u'blather_bot is loading'
