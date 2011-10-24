#!/usr/bin/python

# Run an executable and return the results, with caching

import hashlib
import os
import subprocess
import sys
import time


def Run(cmd, maxage=3600):
  if not os.path.exists('cache'):
    os.mkdir('cache')

  ent = hashlib.sha224(cmd).hexdigest()
  if not os.path.exists('cache/%s' % ent):
    for l in _Run(cmd, ent):
      yield l

  mod = os.path.getmtime('cache/%s' % ent)
  if time.time() - mod > maxage:
    for l in _Run(cmd, ent):
      yield l
  
  sys.stderr.write('Cache hit for %s\n' % cmd)
  f = open('cache/%s' % ent)
  try:
    l = f.readline()
    while l:
      yield l
      l = f.readline()

  finally:
    f.close()


def _Run(cmd, ent):
  local = open('cache/%s' % ent, 'w')

  sys.stderr.write('Executing %s\n' % cmd)
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
  l = p.stdout.readline()
  while l:
    local.write(l)
    yield l
    l = p.stdout.readline()

  local.close()


if __name__ == '__main__':
  for l in Run('sleep 10'):
    print l
