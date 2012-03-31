#!/usr/bin/python

# Copyright (c) Twisted Matrix Laboratories.
# Copyright (c) Michael Still 2013

# See LICENSE for details.


"""
A simple LCA2013 irc bot.

Run this script with two arguments, the channel name the bot should
connect to, and file to log to, e.g.:

  $ python lcabot.py <channel> <channel password>

will log channel #test to the file 'test.log'.
"""


# twisted imports
from twisted.words.protocols import irc
from twisted.internet import reactor, protocol
from twisted.python import log

import datetime
import imp
import os
import re
import sys
import time

VERBOSE = False


class MessageLogger:
    """
    An independent logger class (because separation of application
    and protocol logic is a good thing).
    """
    def __init__(self, file):
        self.file = file

    def log(self, message):
        """Write a message to the file."""
        timestamp = time.strftime("[%H:%M:%S]", time.localtime(time.time()))
        self.file.write('%s %s\n' % (timestamp, message))
        self.file.flush()

    def close(self):
        self.file.close()


class Lcabot(irc.IRCClient):
    """A logging IRC bot."""
    
    nickname = "lcabot"

    def __init__(self):
        self.plugins = []
        self.verbs = {}
        self.last_heartbeat = time.time()

    def _writeLog(self, msg):
        try:
            self.logger.log(msg)
        except:
            pass
        log.msg(msg)

    def _msg(self, channel, msg):
        self.msg(channel, msg)
        self._writeLog('%s >> <%s> %s' %(channel, self.nickname, msg))

    def _topic(self, channel, topic):
        self.topic(channel, topic)
        self._writeLog('%s >> Set topic to "%s"' %(channel, topic))
    
    def _describe(self, channel, action):
        self.describe(channel, action)
        self._writeLog('%s >> /me %s' %(channel, action))

    def _loadPlugins(self):
        self._unloadPlugins()

        self._writeLog('[Loading plugins]')
        self.plugins = []
        self.verbs = {}

        plugin_directory = 'commands'
        re_plugin = re.compile('[^.].*\.py$')
        for plugin_file in os.listdir(plugin_directory):
            self._writeLog('Possible plugin: %s' % plugin_file)
            if re_plugin.match(plugin_file):
                name = plugin_file[:-3]
                self._writeLog('>> %s' % name)

                try:
                    plugin_info = imp.find_module(name, [plugin_directory])
                    plugin = imp.load_module(name, *plugin_info)

                    for module in plugin.Init(self._writeLog):
                        self.plugins.append(module)
                        yield module
                        for verb in module.Verbs():
                            self.verbs[verb] = module
                            self._writeLog('   implements verb %s' % verb)
                except Exception, e:
                    self._writeLog('Exception from %s: %s' %(module, e))

    def _unloadPlugins(self):
        self._writeLog('[Unloading plugins]')
        for module in self.plugins:
            try:
                module.Cleanup()
            except Exception, e:
                self._writeLog('Exception from %s: %s' %(module, e))

    def _handleResponse(self, responses):
        for resp in responses:
            if resp:
                channel, kind, body = resp
                if not channel:
                    channel = self.factory.channel
                if kind == 'msg':
                    self._msg(channel, body)
                elif kind == 'topic':
                    self._topic(channel, body)

    def _doHeartbeat(self):
        """Heartbeat our modules."""

        if time.time() - self.last_heartbeat > 60:
            self.last_heartbeat = time.time()
            for module in self.plugins:
                try:
                    self._writeLog('[Heartbeat sent to %s]' % module.Name())
                    self._handleResponse(list(module.HeartBeat()))
                except Exception, e:
                    self._writeLog('Exception from %s: %s' %(module, e))

    def connectionMade(self):
        irc.IRCClient.connectionMade(self)
        self.logger = MessageLogger(open(self.factory.filename, "a"))
        self._writeLog("[connected at %s]" % 
                       time.asctime(time.localtime(time.time())))

    def connectionLost(self, reason):
        irc.IRCClient.connectionLost(self, reason)
        self._writeLog("[disconnected at %s]" % 
                       time.asctime(time.localtime(time.time())))
        self.logger.close()

    ###########
    # callbacks for events
    def signedOn(self):
        """Called when bot has succesfully signed on to server."""
        self._writeLog("[I have signed on]")
        self.join(self.factory.channel, self.factory.channel_password)

    def joined(self, channel):
        """This will get called when the bot joins the channel."""
        self._writeLog("[I have joined %s]" % channel)
        self._loadPlugins()

    def topicUpdated(self, user, channel, topic):
        """Called when the topic changes or we join a channel."""
        self._writeLog('%s >> topic set by %s is "%s"'
                       %(channel, user, topic))

    def privmsg(self, user, channel, msg):
        """This will get called when the bot receives a message."""

        user = user.split('!', 1)[0]
        self._writeLog('%s >> <%s> %s' % (channel, user, msg))
        
        # Otherwise check to see if it is a message directed at me
        msg = msg.rstrip()
        if msg.startswith(self.nickname + ':'):
            command = ':'.join(msg.split(':')[1:]).lstrip()
            elems = command.split(' ')

            if elems[0] == 'reload':
                for module in self._loadPlugins():
                    self._msg(channel, '%s: Loaded %s' %(user, module.Name()))

            elif elems[0] in self.verbs:
                module = self.verbs[elems[0]]
                try:
                    self._writeLog('[Command sent to %s]' % module.Name())
                    self._handleResponse(list(module.Command(channel,
                                                             elems[0],
                                                             command)))
                except Exception, e:
                    self._writeLog('Exception from %s: %s' %(module, e))

            else:
                if channel == self.nickname:
                    self._msg(user, ('I understand the following commands: %s'
                                     % ', '.join(self.verbs.keys())))
                else:
                    self._describe(channel, 'is confused')
                    self._msg(channel, ('%s: I am the linux.conf.au bot. PM me '
                                        'for help.' % user))

    def action(self, user, channel, msg):
        """This will get called when the bot sees someone do an action."""

        user = user.split('!', 1)[0]
        self._writeLog('%s >> * %s %s' % (channel, user, msg))

    def userJoined(self, user, channel):
        """This is called when a user joins a channel."""

        for module in self.plugins:
            try:
                self._writeLog('[Join event for %s on %s sent to %s]'
                               %(user, channel, module.Name()))
                self._handleResponse(list(module.NoticeUser(channel, user)))
            except Exception, e:
                self._writeLog('Exception from %s: %s' %(module, e))

    # irc callbacks
    def irc_NICK(self, prefix, params):
        """Called when an IRC user changes their nickname."""
        old_nick = prefix.split('!')[0]
        new_nick = params[0]
        self._writeLog("%s is now known as %s" % (old_nick, new_nick))


    # For fun, override the method that determines how a nickname is changed on
    # collisions. The default method appends an underscore.
    def alterCollidedNick(self, nickname):
        """
        Generate an altered version of a nickname that caused a collision in an
        effort to create an unused related name for subsequent registration.
        """
        return nickname + '^'

    # Scary inner goo
    def dataReceived(self, data):
        """Log all incoming data."""

        if VERBOSE:
            for l in data.replace('\r', '').split('\n'):
                self._writeLog('IN: %s' % l)
        irc.IRCClient.dataReceived(self, data)

        if data.startswith('PING :') and data.endswith('\n'):
            self._doHeartbeat()

    def sendLine(self, line):
        """Log all outgoing data."""

        if VERBOSE:
            for l in line.replace('\r', '').split('\n'):
                self._writeLog('OUT: %s' % l)

        irc.IRCClient.sendLine(self, line)


class LcaBotFactory(protocol.ClientFactory):
    """A factory for LcaBots.

    A new protocol instance will be created each time we connect to the server.
    """

    def __init__(self, channel, channel_password, filename):
        self.channel = channel
        self.channel_password = channel_password
        self.filename = filename

    def buildProtocol(self, addr):
        p = Lcabot()
        p.factory = self
        return p

    def clientConnectionLost(self, connector, reason):
        """If we get disconnected, reconnect to server."""
        connector.connect()

    def clientConnectionFailed(self, connector, reason):
        print "connection failed:", reason
        reactor.stop()


if __name__ == '__main__':
    # initialize logging
    log.startLogging(sys.stdout)
    
    # create factory protocol and application
    logfile = datetime.datetime.now().strftime('%Y%m%d-%H%M%S.log')
    f = LcaBotFactory(sys.argv[1], sys.argv[2], logfile)

    # connect factory to this host and port
    reactor.connectTCP("irc.freenode.net", 6667, f)

    # run bot
    reactor.run()
