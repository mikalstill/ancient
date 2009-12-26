#!/usr/bin/python

import MySQLdb

db = MySQLdb.connect(user = 'root', db = 'home')
cursor = db.cursor(MySQLdb.cursors.DictCursor)

event_times = []
cursor.execute('select * from events;')
for row in cursor:
  event_times.append(row['epoch_seconds'])

event_times.append(9999999999)
print 'Events: %s' % event_times

for i in range(len(event_times) - 1):
  print 'Considering event at %s' % event_times[i]
  cursor.execute('select * from sensors where sensor="t10fa473500000037" '
                 'and epoch_seconds > %s and epoch_seconds < %s;'
                 %(event_times[i], event_times[i + 1]))

  previous = None
  for row in cursor:
    if previous and float(row['value']) < previous:
      diff = previous - float(row['value'])
      print diff

    previous = float(row['value'])
    
  print
