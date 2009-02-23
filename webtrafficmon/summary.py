#!/usr/bin/python


import datetime
import os
import shelve
import sys
import time


SLICE_SIZE = 1800


for ent in os.listdir('.'):
  if ent.endswith('shlf'):
    (_, t, _) = ent.split('.')
    t = int(t)

    if time.time() - t > 3600:
      print 'Summarizing %s' % ent
      db = shelve.open(ent)
      print 'DB loaded'
      db.setdefault('history', {})

      keys = db['history'].keys()
      keys.sort()

      while len(keys) > 0:
        print '%s %d samples' %(datetime.datetime.now(), len(keys))
        rollup = {}

        for sample in keys[:SLICE_SIZE]:
          sample_readings = db['history'][sample]

          for flow in sample_readings:
            (new_ac, new_ab, new_bc, new_bb) = sample_readings[flow]
            rollup.setdefault(flow, (0, 0, 0, 0))
            (ac, ab, bc, bb) = rollup[flow]
            ac += new_ac
            ab += new_ab
            bc += new_bc
            bb += new_bb
            rollup[flow] = (ac, ab, bc, bb)

          sys.stdout.write('.')
          sys.stdout.flush()
        print

        summary = open('history.%d.summary' % t, 'w')
        for flow in rollup:
          summary.write('%s: %s\n' %(flow, repr(rollup[flow])))
        summary.write('EOF')
        summary.close()
        
        keys = keys[SLICE_SIZE:]
        t += SLICE_SIZE

      db.close()

      os.unlink(ent)
      print 'Done'
