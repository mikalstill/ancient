#!/usr/bin/python

import fcntl
import os
import select
import string
import subprocess
import sys
import traceback


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


try:
  dialog = ['C0644 21 c\n',
            'file c file c file c\n\0',
            'C0644 21 d\n',
            'file d file d file d\n\0']

  proc = subprocess.Popen(['scp', '-v', '-d', '-t', '/tmp'],
                          stdin=subprocess.PIPE,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.PIPE)

  r = [proc.stdout, proc.stderr]
  w = []
  e = [proc.stdout, proc.stderr]

  fl = fcntl.fcntl(proc.stdout, fcntl.F_GETFL)
  fcntl.fcntl(proc.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)
  fl = fcntl.fcntl(proc.stderr, fcntl.F_GETFL)
  fcntl.fcntl(proc.stderr, fcntl.F_SETFL, fl | os.O_NONBLOCK)

  stdin_closed = False
  while proc.returncode is None:
    (readable, _, errorable) = select.select(r, w, e)

    for flo in readable:
      if flo == proc.stdout:
        d = os.read(proc.stdout.fileno(), 1024)
        if len(d) > 0:
          sys.stdout.write('O: %s\n' % printable(d))

          if len(dialog) > 0:
            sys.stdout.write('I: %s\n' % printable(dialog[0]))
            os.write(proc.stdin.fileno(), dialog[0])
            dialog = dialog[1:]

          if len(dialog) == 0 and not stdin_closed:
            sys.stdout.write('>>> stdin closed\n')
            proc.stdin.close()
            stdin_closed = True

        else:
          sys.stdout.write('>>> stdout closed\n')
          r.remove(proc.stdout)
          e.remove(proc.stdout)

      elif flo == proc.stderr:
        d = os.read(proc.stderr.fileno(), 1024)
        if len(d) > 0:
          sys.stdout.write('E: %s\n' % printable(d))
        else:
          sys.stdout.write('>>> stderr closed\n')
          r.remove(proc.stderr)
          e.remove(proc.stderr)

      else:
        sys.stdout.write('>>> Unknown readable: %s: %s\n'
                         %(repr(flo), flo.read()))

    for flo in errorable:
      sys.stdout.write('>>> Error on %s\n' % repr(flo))
      r.remove(flo)
      e.remove(flo)

    proc.poll()

  print '#: %s' % proc.returncode

except:
  exc = sys.exc_info()
  for tb in traceback.format_exception(exc[0], exc[1], exc[2]):
    print tb
    del tb
