#!/usr/bin/python

import sys

from IPy import IP

ip = IP(sys.argv[1])
ips = []
for x in ip:
  ips.append(x)

print 'Netblock length: %d' % len(ips)
print 'First: %s' % str(ips[0])
print 'Last:  %s' % str(ips[-1])
