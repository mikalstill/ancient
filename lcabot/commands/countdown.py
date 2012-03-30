#!/usr/bin/python2.4

# countdown -- provide a count down to important LCA deadlines

import datetime

class CountDown(object):
    def __init__(self, log):
        self.log = log

    def Name(self):
        """Who am I?"""
        return 'countdown'

    def Verbs(self):
        """Return the verbs which this module supports

        Takes no arguments, and returns an array of strings.
        """

        return ['countdown']

    def Help(self, verb):
        """Display help for a verb

        Takes the name of a verb, and returns a string which is the help
        message for that verb.
        """

        if verb == 'countdown':
            return 'Count down the number of days to important events.'
        return ''

    def Command(self, verb, line):
        """Execute a given verb with these arguments
        
        Takes the verb which the user entered, and the remainder of the line.
        Returns a string which is sent to the user.
        """

        if verb == 'countdown':
            for event, year, month, day in [('CFP opens', 2012, 06, 01),
                                            ('early bird registration',
                                             2012, 10, 01),
                                            ('conference', 2013, 01, 28)]:
                dt = datetime.datetime(year, month, day)
                delta = dt - datetime.datetime.now()
                yield 'Days until %s: %d' %(event, delta.days)

    def HeartBeat(self):
        """Gets called at regular intervals"""
        pass

    def Cleanup(self):
        """We're about to be torn down."""
        pass


def Init(log):
    """Initialize all command classes."""
    yield CountDown(log)
