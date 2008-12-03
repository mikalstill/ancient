#!/usr/bin/python2.4
#
# Copyright (c) Michael Still 2006, released under the terms of the GNU GPL v2

"""mythtv.py -- a MythTV mbot plugin

This plugin implements the MythTV on screen display and telnet functionality
in a simple example mbot bot.
"""

import sys
import socket
import os
import re

mythtv = None

class MythTelnet:
  """Execute commands using the MythTV front end telnet interface"""

  def __init__(self):
    try:
      self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.s.connect(('localhost', 6546))
      self.s.recv(1024)
    except:
      print 'Connection failed for MythTV telnet interface'
      self.s = None

  def execute_command(self, cmd):
    """Execute text commands via the front end telnet interface, and return
    the string output from the command"""

    print 'Executing "%s" on MythTV front end' % cmd
    self.s.send(cmd + '\n')  
    result = ''
    data = ''

    while data[-2:] != '# ':
      result = result + data
      data = self.s.recv(1024)
    result = result + data[:-3]
    result = result.rstrip('\r\n')

    # Special case "query location" so that it shows the program name
    if cmd == 'query location' and result.startswith('Playback Recorded'):
      print 'Executing playback subquery'
      program = ' '.join(result.split(' ')[-2:])
      recording = self.execute_command('query recording ' + program).split(' ')
      result = '%s %s' %(' '.join(result.split(' ')[:-2]), 
                         ' '.join(recording[2:]))

    print 'Returning "%s"' % result
    return result

def Verbs():
  """Verbs -- return the verbs which this module supports

  Takes no arguments, and returns an array of strings.
  """

  return ['jump', 'key', 'play', 'query', 'say']

def Status():
  """Status -- suggest a message to display as the status string for the bot
  
  Takes no arguments, and returns a string. In this case we return the current
  location in the MythTV user interface.
  """

  return mythtv.execute_command('query location')

def Help(verb):
  """Help -- display help for a verb

  Takes the name of a verb, and returns a string which is the help message for
  that verb.
  """

  if verb == 'say':
    return ('say <string> - display this string using mythtvosd (only works '
           'if the frontend is currently playing a recording)')
  help_string = mythtv.execute_command('help ' + verb)
  return re.sub(' +', ' ', help_string)

def Command(verb, line):
  """Command -- execute a given verb with these arguments

  Takes the verb which the user entered, and the remainder of the line.
  Returns a string which is sent to the user.
  """

  if verb == 'say':
    if mythtv.execute_command('query location')[:17] == 'Playback Recorded':
      os.system('mythtvosd --template=alert alert_text="' + line + '"')
      return 'Done'
    else:
      return 'Unable to say, because frontend is not playing'

  # If it's not a say, then send it on to the MythTV front end telnet interface
  return mythtv.execute_command(verb + ' ' + line)
    
def Cleanup():
  """Cleanup -- you're about to be unloaded.

  We don't need to do anthing in this case.
  """
  return

# Do initialization here
print u'mythtv_bot is loading'
mythtv = MythTelnet()
