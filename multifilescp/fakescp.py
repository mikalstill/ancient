#!/usr/bin/python

import fcntl
import os 
import select
import string
import subprocess
import sys


_LOG_FILE = '/tmp/scp.log'


def printable(s):
  out = ''

  for c in s:
    if c == '\n':
      out += '\\n'
    elif c in string.printable:
      out += c
    else:
      out += '\\%d' % ord(c)

  return out

log = open(_LOG_FILE, 'a')
log.write('>>> New command invokation: %s\n' % ' '.join(sys.argv))
log.flush()

argv = sys.argv
argv[0] = 'scp'
proc = subprocess.Popen(argv, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE)

r = [sys.stdin, proc.stdout, proc.stderr]
w = []
e = [sys.stdin, proc.stdout, proc.stderr]

fl = fcntl.fcntl(sys.stdin, fcntl.F_GETFL)
fcntl.fcntl(sys.stdin, fcntl.F_SETFL, fl | os.O_NONBLOCK)
fl = fcntl.fcntl(proc.stdout, fcntl.F_GETFL)
fcntl.fcntl(proc.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)
fl = fcntl.fcntl(proc.stderr, fcntl.F_GETFL)
fcntl.fcntl(proc.stderr, fcntl.F_SETFL, fl | os.O_NONBLOCK)

while proc.returncode is None:
  (readable, _, errorable) = select.select(r, w, e)

  for flo in readable:
    if flo == sys.stdin:
      d = os.read(sys.stdin.fileno(), 1024)
      if len(d) > 0:
        log.write('I: %s\n' % printable(d))
        os.write(proc.stdin.fileno(), d)
      else:
        log.write('>>> stdin closed\n')
        r.remove(sys.stdin)
        e.remove(sys.stdin)
        proc.stdin.close()

    elif flo == proc.stdout:
      d = os.read(proc.stdout.fileno(), 1024)
      if len(d) > 0:
        log.write('O: %s\n' % printable(d))
        os.write(sys.stdout.fileno(), d)
      else:
        log.write('>>> stdout closed\n')
        r.remove(proc.stdout)
        e.remove(proc.stdout)

    elif flo == proc.stderr:
      d = os.read(proc.stderr.fileno(), 1024)
      if len(d) > 0:
        log.write('E: %s\n' % printable(d))
        os.write(sys.stderr.fileno(), d)
      else:
        log.write('>>> stderr closed\n')
        r.remove(proc.stderr)
        e.remove(proc.stderr)

    else:
      log.write('>>> Unknown readable: %s: %s\n' %(repr(flo), flo.read()))

  for flo in errorable:
    log.write('>>> Error on %s\n' % repr(flo))
    r.remove(flo)
    e.remove(flo)

  proc.poll()
