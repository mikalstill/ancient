#!/usr/bin/python

# Flatten a file into one big line, and the return portions which match an
# expression

# $1: pattern (something like hello(.*)there)
# $2+: files (- for stdin)

import re
import sys

pattern = ''
try:
  pattern = sys.argv[1]
except:
  print 'Specify a pattern please'

regexp = re.compile('(.*)%s(.*)' % pattern)

for file in sys.argv[2:]:
  if file == '-':
    f = sys.stdin
  else:
    f = open(file, 'r')

  s = ''
  for line in f.readlines():
    s += line.rstrip('\n').rstrip('\r')

    m = regexp.match(s)
    while m:
      print '%s: %s' %(file, m.group(1))
      s = m.group(3)
      m = regexp.match(s)
