#!/usr/bin/python

import copy
import cStringIO
import csv
import gzip
import re
import sys


COLUMN_RE = re.compile('^  `(.*)` (.*) *(.*),$')
INSERT_RE = re.compile('INSERT INTO `(.*)` VALUES \((.*)\);')


def ReadDump(filename):
  """Read a MySQL dump file, and yeild the resulting rows. Assumes only one table per dump."""

  if filename.endswith('.gz'):
    file = gzip.open(filename)
  else:
    file = open(filename)

  # First scan the CREATE TABLE looking for the fields we want
  engine_seen = False
  fields = []
  line = file.readline()

  while not engine_seen:
    if line.find('ENGINE') != -1:
      engine_seen = True

    m = COLUMN_RE.match(line)
    if m:
      fields.append(m.group(1))

    line = file.readline()

  print 'Fields: %s' % repr(fields)

  # Now process the rows
  line = file.readline()
  while line:
    m = INSERT_RE.match(line)
    if m:
      csvdata = cStringIO.StringIO()
      for row_data in m.group(2).split('),('):
        csvdata.write('%s\n' % row_data)
      csvdata.seek(0)

      for csvrow in csv.reader(csvdata, quotechar="'"):
        try:
          row = {}
          f = copy.deepcopy(fields)
          for elem in csvrow:
            row[f[0]] = elem
            f = f[1:]
          yield row

        except Exception, e:
          print '*** Decode failure: %s: %s' %(csvrow, e)

    line = file.readline()
