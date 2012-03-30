#!/usr/bin/python

# Copyright (c) Twisted Matrix Laboratories.
# Copyright (c) Michael Still 2013

# See LICENSE for details.


"""
A simple LCA2013 irc bot.

Run this script with two arguments, the channel name the bot should
connect to, and file to log to, e.g.:

  $ python lcabot.py <channel> <logfile>

will log channel #test to the file 'test.log'.
"""


# twisted imports
from twisted.words.protocols import irc
from twisted.internet import reactor, protocol
from twisted.python import log

import datetime
import sys
import time


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


class LogBot(irc.IRCClient):
    """A logging IRC bot."""
    
    nickname = "lcabot"

    def _writeLog(self, msg):
        self.logger.log(msg)
        log.msg(msg)

    def _msg(self, channel, msg):
        self.msg(channel, msg)
        self._writeLog('%s >> %s' %(channel, msg))
    
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


    # callbacks for events

    def signedOn(self):
        """Called when bot has succesfully signed on to server."""
        self.join(self.factory.channel)

    def joined(self, channel):
        """This will get called when the bot joins the channel."""
        self._writeLog("[I have joined %s]" % channel)

    def privmsg(self, user, channel, msg):
        """This will get called when the bot receives a message."""

        user = user.split('!', 1)[0]
        self._writeLog("<%s> %s" % (user, msg))
        
        # Check to see if they're sending me a private message
        if channel == self.nickname:
            self._msg(user,
                     """I know how to do these things:
  countdown: how many days is it until LCA?
""")
            return

        # Otherwise check to see if it is a message directed at me
        msg = msg.rstrip()
        if msg.startswith(self.nickname + ':'):
            elems = msg.split(': ')
            if elems[1] == 'countdown':
                for event, year, month, day in [('CFP opens', 2012, 06, 01),
                                                ('early bird registration',
                                                 2012, 10, 01),
                                                ('conference', 2013, 01, 28)]:
                    dt = datetime.datetime(year, month, day)
                    delta = dt - datetime.datetime.now()
                    self._msg(channel, 'Days until %s: %d' %(event, delta.days))

            else:
                msg = '%s: I am the linux.conf.au bot. PM me for help.' % user
                self._msg(channel, msg)
                self._writeLog("<%s> %s" % (self.nickname, msg))

    def action(self, user, channel, msg):
        """This will get called when the bot sees someone do an action."""
        user = user.split('!', 1)[0]
        self._writeLog("* %s %s" % (user, msg))

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



class LogBotFactory(protocol.ClientFactory):
    """A factory for LogBots.

    A new protocol instance will be created each time we connect to the server.
    """

    def __init__(self, channel, filename):
        self.channel = channel
        self.filename = filename

    def buildProtocol(self, addr):
        p = LogBot()
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
    f = LogBotFactory(sys.argv[1], sys.argv[2])

    # connect factory to this host and port
    reactor.connectTCP("irc.freenode.net", 6667, f)

    # run bot
    reactor.run()
