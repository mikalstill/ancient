#!/usr/bin/python

# This is an equivalent of the puppet facter for network stuff

import re
import subprocess
import time
import yaml


IPREGEX = '\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}'


class Facts(object):
    def __init__(self):
        self.run_cache = {}

    def _run(self, cmd):
        out = []

        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        l = p.stdout.readline()
        while l:
            l = l.rstrip()
            out.append(l)
            l = p.stdout.readline()

        return out

    def _cached_run(self, cmd, timeout=300):
        if cmd in self.run_cache:
            out, when = self.run_cache[cmd]
            if time.time() - when < timeout:
                return out

        out = self._run(cmd)
        self.run_cache[cmd] = (out, time.time())
        return out

    def parse_netstat(self):
        """Parse output of netstat for default gateway and interface."""
        match_re = re.compile('^0\.0\.0\.0\s+(' + IPREGEX + ')\s+' +
                              '0\.0\.0\.0\s.*\s([a-z].*)$')
        for line in self._cached_run('/bin/netstat -rn'):
            m = match_re.match(line)
            if m:
                # group 1 is gateway ip, group 2 is gateway interface
                yield (m.group(1), m.group(2))

    def default_gateway(self):
        """Return the IP of the first default gateway."""
        defaults = list(self.parse_netstat())
        return defaults[0][0]

    def default_interface(self):
        """Return the interface of the first default gateway."""
        defaults = list(self.parse_netstat())
        return defaults[0][1]

    def default_ip_address(self):
        """Return the IP address configured for the default interface."""
        match_re = re.compile('^' + IPREGEX + '\s+dev.*src\s+(' + IPREGEX +')$')
        for line in self._cached_run('/sbin/ip route get %s'
                                     % self.default_gateway()):
            m = match_re.match(line)
            if m:
                # group 1 is the ip
                return m.group(1)

    def default_netmask(self):
        """Return the netmask configured on the default interface."""
        match_re = re.compile('.*inet addr:' + IPREGEX + '\s+' +
                              'Bcast:' + IPREGEX + '\s+' +
                              'Mask:(' + IPREGEX + ')$')
        for line in self._cached_run('/sbin/ifconfig %s'
                                     % self.default_interface()):
            m = match_re.match(line)
            if m:
                # group 1 is the netmask
                return m.group(1)

    def network_name(self):
        """Based on available facts, determine what network we are on."""
        # TODO
        pass


if __name__ == '__main__':
    f = Facts()
    print 'Default Gateway: %s' % f.default_gateway()
    print 'Default Interface: %s' % f.default_interface()
    print 'Default IP Address: %s' % f.default_ip_address()
    print 'Default Netmask: %s' % f.default_netmask()
