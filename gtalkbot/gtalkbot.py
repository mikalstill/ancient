#!/usr/bin/python2.4 -u
#
# Copyright (c) Michael Still 2006 and 2007
# Released under the terms of the GNU GPL v2

"""gtalkbot.py -- a modular Jabber bot

This is a simple python Google Talk and Jabber bot. It loads plugins from
the specified plugin directory, and then lets them register "verbs". These
verbs are simple single word commands which must be at the start of the line
entered by the other side of the chat conversation. If the first word of one
of these lines matches a registered verb, then the relevant plugin is
handed the verb, as well as the remainder of the line (in separate arguments).

The bot authenticates clients before it lets any verbs be executed.

This code is very heavily based on the echobot example from the PyXMPP 
distribution.
"""

import codecs
import datetime
import imp
import logging
import locale
import os
import re
import sys
import unicodedata

from pyxmpp.all import JID, Iq, Presence, Message, StreamError
from pyxmpp.jabber.client import JabberClient
from pyxmpp.client import Client
from pyxmpp import streamtls


plugins = {}
verbs = {}


def Normalize(value):
  normalized = unicodedata.normalize('NFKD', unicode(value))
  normalized = normalized.encode('ascii', 'ignore')
  return normalized


class PasswordVerifier:
  def authenticate(self, jid, password):
    """Authenticate the jabber id with the given password"""
    return False;


class PasswordCache(PasswordVerifier):
  """Reads a file of jabber IDs and passwords at startup, and then provides
  lookups for those for authentication"""

  def __init__(self, password_filename):
    """Read password_filename into a dictionary."""
    self.passwd = {}

    # We load passwords from a hard coded path for this example, but in a
    # real application you would do this better...
    print u'Loading passwords'
    try:
      passwdfile = open(password_filename, 'r')
    except:
      print u'Please create the password file at %s' % password_filename
      sys.exit(1)

    for line in passwdfile.readlines():
      [jid, password] = line.rstrip('\r\n').split('\t')
      self.passwd[jid] = password

  def authenticate(self, jid, password):
    """Determine if a given user has provided a password which matches the
    cache"""

    if not self.passwd.has_key(jid):
      return False;
    print u'Authenticating %s with "%s" against "%s"' %(jid, password,
                                                        self.passwd[jid])
    return self.passwd[jid] == password


class NoAuthentication(PasswordVerifier):
  def authenticate(self, jid, password):
    """Always say yes"""
    return True


class BotClient(JabberClient):
  """This class implements all the Jabber parts of the program, a lot
  of the Jabber functionality comes from inheriting from JabberClient,
  which is part of PyXMPP."""

  authenticated = {}

  def __init__(self, jid, password, password_verifier, outbox_directory):
    # If a bare JID is provided add a resource
    if not jid.resource:
      jid=JID(jid.node, jid.domain, "gtalkbot")

    # Setup client with provided connection information and identity data, 
    # this block also handles the SSL encryption using with Google Talk
    tls = streamtls.TLSSettings(require=True, verify_peer=False)
    auth = ['sasl:PLAIN']
    print u'Logging into talk network as %s' % jid
    JabberClient.__init__(self, jid, password,
                          disco_name="gtalkbot", disco_type="bot",
                          tls_settings=tls, auth_methods=auth)

    # Register features to be announced via Service Discovery
    self.disco_info.add_feature("jabber:iq:version")

    # Initialize the client
    print u'Initialize client'
    self.jid = jid
    self.passwd = password_verifier
    self.outbox_directory = outbox_directory

  def session_started(self):
    """This is called when the IM session is successfully started
    (after all the neccessery negotiations, authentication and
    authorization)."""
    JabberClient.session_started(self)
    print 'Session started'

    # Set up handlers for supported <iq/> queries
    self.stream.set_iq_get_handler('query', 'jabber:iq:version',
                                   self.get_version)

    # Set up handlers for <presence/> stanzas
    self.stream.set_presence_handler('subscribe', self.presence_control)
    self.stream.set_presence_handler('subscribed', self.presence_control)
    self.stream.set_presence_handler('unsubscribe', self.presence_control)
    self.stream.set_presence_handler('unsubscribed', self.presence_control)

    # Set up handler for <message stanza>
    self.stream.set_message_handler('normal', self.message)

  def get_version(self,iq):
    """Handler for jabber:iq:version queries.

    jabber:iq:version queries are not supported directly by PyXMPP, so the
    XML node is accessed directly through the libxml2 API.  This should be
    used very carefully!"""

    iq = iq.make_result_response()
    q = iq.new_query('jabber:iq:version')
    q.newTextChild(q.ns(), 'name', 'gtalkbot')
    q.newTextChild(q.ns(), 'version', '1.0')
    self.stream.send(iq)
    return True

  def message(self, stanza):
    """Handle incoming messages"""

    print 'Handling message'
    subject = stanza.get_subject()
    body = stanza.get_body()
    sender = stanza.get_from().as_utf8().split('/')[0]
    t = stanza.get_type()
    
    print ' ... sender = %s' % sender
    print ' ... subject = %s' % subject
    print ' ... body = %s' % body
    print ' ... type = %s' % t

    # If this is a text message, then we respond
    if stanza.get_type() != 'headline' and body:
      verb = body.split(' ')[0]
      verb = verb.lower()

      status_message = ''

      # Force users to be authenticated before they can do anything
      if not self.authenticated.has_key(sender):
        if verb == 'auth':
          if self.passwd.authenticate(sender, body[5:]):
            result = 'Welcome'
            status_message = 'Authenticated'
            self.authenticated[sender] = True
          else:
            status_message = 'Who are you?'
            result = 'Sorry, try again'
        else:
          status_message = 'Who are you?'
          result = 'You need to authenticate.\nUse auth <password>'

      # If we are authenticated, then other commands work
      elif verb == 'help':
        if len(body) == 4:
          sorted_verbs = verbs.keys()
          sorted_verbs.sort()
          result = 'Registered verbs are: %s\nTry help <verb> ' \
                   'to learn more about a verb.' % \
                   ', '.join(sorted_verbs)
          status_message = 'Helping'
        else:
          help_target = body.split(' ')[1]
          help_target = help_target.lower()

          if not verbs.has_key(help_target):
            result = 'No such command -- try help to find out which ' \
                     'commands do exist'
            status_message = 'Nothing to see here'
          else:
            plugin_name = verbs[help_target]
            try:
              result = plugins[plugin_name].Help(help_target)
              status_message = plugins[plugin_name].Status()
            except:
              result = 'No help available for this verb'
              status_message = 'Bummer'

      # And now we go looking for a plugin to execute the verb
      elif verbs.has_key(verb):
        plugin_name = verbs[verb]
        body = body[len(verb) + 1:]
        print 'Passing %s to %s (with %s)' %(verb, plugin_name, body)
        result = plugins[plugin_name].Command(verb, body)
        status_message = plugins[plugin_name].Status()

      else:
        result = 'Command not known'
        status_message = 'Huh?'

      p = Presence(status = status_message)
      self.stream.send(p)

      m = Message(to_jid = stanza.get_from(),
                  from_jid = stanza.get_to(),
                  stanza_type = stanza.get_type(),
                  subject = subject,
                  body = Normalize(result))
      self.stream.send(m)
      print ' ... recipient = %s' % stanza.get_from()
      print ' ... body = %s' % Normalize(result)

    return True

  def presence_control(self,stanza):
    """Handle subscription control <presence/> stanzas -- acknowledge
    them."""

    print 'Handling presence request'
    self.stream.send(stanza.make_accept_response())
    return True

  def idle(self):
    """Called when there is a chance to do idle work."""

    print '%s Idle' % datetime.datetime.now()
    if self.outbox_directory:
      # Outbox format is one message per file. First line is the recipient,
      # the remaining lines are the message

      for ent in os.listdir(self.outbox_directory):
        print ' ... Found %s' % ent
	f = open('%s/%s' %(self.outbox_directory, ent), 'r')
	d = f.readlines()
        f.close()

        m = Message(to_jid = JID(d[0].rstrip('\n')),
                    from_jid = self.jid,
                    stanza_type = 'chat',
                    subject = None,
                    body = Normalize(''.join(d[1:]).rstrip('\n')))
        self.stream.send(m)
        print ' ... recipient = %s' % d[0].rstrip('\n')
        print ' ... body = %s' % Normalize(''.join(d[1:]).rstrip('\n'))
        os.unlink('%s/%s' %(self.outbox_directory, ent))

    Client.idle(self)


# The XMPP protocol is Unicode-based. To properly display data received
# it _must_ be converted to local encoding or UnicodeException may be 
# raised
locale.setlocale(locale.LC_CTYPE,"")
encoding = locale.getlocale()[1]
if not encoding:
  encoding = 'us-ascii'
sys.stdout = codecs.getwriter(encoding)(sys.stdout, errors='replace')
sys.stderr = codecs.getwriter(encoding)(sys.stderr, errors='replace')


# PyXMPP uses `logging` module for its debug output applications should 
# set it up as needed
logger = logging.getLogger()
logger.addHandler(logging.StreamHandler())
logger.setLevel(logging.INFO)

if len(sys.argv) < 2:
  print u'Usage:'
  print '\t%s configfile' % (sys.argv[0],)
  print 'example:'
  print '\t%s ~/.gtalkbotrc' % (sys.argv[0],)
  sys.exit(1)

# Read the config file
account = ''
password = ''
plugin_directory = ''
password_filename = '/usr/local/share/gtalkbot/passwd'
require_auth = True
outbox_directory = ''

try:
  config_file = open(sys.argv[1], 'r')
  for line in config_file.readlines():
    [option, value] = line.rstrip('\r\n').split('\t')
    if option == 'connectwith':
      account = value
    if option == 'password':
      password = value
    if option == 'pluginsdir':
      plugin_directory = value
    if option == 'authfile':
      password_filename = value
    if option == 'requireauth':
      require_auth = (value == 'False')
    if option == 'outboxdir':
      outbox_directory = value

except:
  print 'Error: %s' % sys.exc_info()[0]
  print ''
  print 'Please create an options file at %s' % sys.argv[1]
  print 'It should contain lines of the form:'
  print ''
  print 'connectwith	<account>'
  print 'pluginsdir	<location of plugin directory>'
  print 'password	<password>'
  print 'authfile	<path to password file'
  print '		 ... the default is /usr/local/share/gtalkbot/passwd'
  sys.exit(1)

# Load plugins
print u'Loading plugins...'
re_plugin = re.compile('[^.].*\.py$')
for plugin_file in os.listdir(plugin_directory):
  if re_plugin.match(plugin_file):
    name = plugin_file[:-3]
    print 'Loading %s' % name
    plugin_info = imp.find_module(name, [plugin_directory])
    plugin = imp.load_module(name, *plugin_info)
    plugins[name] = plugin

    # Load the verbs supported by this plugin
    for verb in plugin.Verbs():
      verbs[verb] = name

print u'Creating client...'
password_verifier = PasswordCache(password_filename)
if not require_auth:
  password_verifier = NoAuthentication()
c = BotClient(JID(account), password, password_verifier, outbox_directory)

print u'Connecting...'
c.connect()

print u'Connected'
try:
  # The client class provides a basic "main loop" for the application.
  c.loop(1)

except KeyboardInterrupt:
  print u'Disconnecting'
  c.disconnect()
