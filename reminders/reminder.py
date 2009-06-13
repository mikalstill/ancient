#!/usr/bin/python

# Read a list of events (and their associated dates) from ~/.reminder and
# display a count down to the user. The format of the file is:
#
# YYYYMMDD Event

import datetime
import os
import re

path = '%s/.reminder' % os.environ['HOME']
hours_away = []
days_away = {}

if os.path.exists(path):
  todo_re = re.compile('^TODO (.*)$')
  reminder_re = re.compile('^([0-9]{4})([0-9]{2})([0-9]{2}) (.*)$')
  now = datetime.datetime.now()

  print
  print '*' * 30
  print ' REMINDERS'
  print '*' * 30
  print

  f = open(path)
  for l in f:
    l = l.rstrip()
    m = reminder_re.match(l)
    if m:
      year = int(m.group(1))
      month = int(m.group(2))
      day = int(m.group(3))
      event = m.group(4)
      
      target = datetime.datetime(year, month, day)
      if target > now:
        delta = target - now

        if delta.days == 0:
          hours_away.append(event)
        else:
          days_away.setdefault(target, [])
          days_away[target].append(event)

    else:
      m = todo_re.match(l)
      if m:
        print ' - %s' % m.group(1)

print
if hours_away:
  print 'Tomorrow: %s' % (', '.join(hours_away))
  print

distances = days_away.keys()
distances.sort()

one_day = datetime.timedelta(days=1)

for distance in distances:
  delta = distance - now

  work_days = 0
  weekends = 0
  t = now
  while t < distance:
    dow = t.strftime('%w')
    if dow != '0' and dow != '6':
      work_days +=1
    else:
      weekends += 1
    t += one_day

  print ('%3dd, %3dwd, %3dwe: %s'
         % (delta.days, work_days, weekends / 2,
            ', '.join(days_away[distance])))

print
