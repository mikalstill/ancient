#!/usr/bin/python

# With a big nod to http://www.madox.net/blog/projects/mazda-can-bus/

import gzip
import sys

valid_packet_lengths = {
  '190': 2,
  '201': 8,
  '205': 2,
  '228': 2,
  '231': 2,
  '240': 3,
  '430': 3,
  '4B0': 9,
  }

f = gzip.open(sys.argv[1])
for l in f.readlines():
  l = l.rstrip()
  d = l.split(' ')

  # Check packet length
  if d[0] in valid_packet_lengths and valid_packet_lengths[d[0]] != len(d):
    #print 'Corrupt: %s' % l
    continue

  if d[0] == '190':
    if d[1] == '00':
      print 'Handbrake off'
    else:
      print 'Handbrake on'

  elif d[0] == '201':
    print ('Engine: RPM %d Undecoded %d Speed %f km/h Accelerator %d'
           %(int('%s%s' %(d[1], d[2]), 16),
             int('%s%s' %(d[3], d[4]), 16),
             int('%s%s' %(d[5], d[6]), 16) / 100.0,
             int(d[7], 16)))

  elif d[0] == '205':
    print 'Brake active: %s' % d[1]

  elif d[0] == '228' or d[0] == '231':
    # Does not appear to be valid on my manual Mazda 3 (always zero)
    print 'Gear: %s' % d[1]

  elif d[0] == '240':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '430':
    print 'Undecoded opcode: %s' % l

  elif l.startswith('4B0'):
    # I think the units might be off here, there is way too many zero readings
    print ('Wheels: LF %f RF %f LR %f RR %f (km/h)'
           %(float(int('%s%s' %(d[1], d[2]), 16) - 10000) / 100,
             float(int('%s%s' %(d[3], d[4]), 16) - 10000) / 100,
             float(int('%s%s' %(d[5], d[6]), 16) - 10000) / 100,
             float(int('%s%s' %(d[7], d[8]), 16) - 10000) / 100))

  elif l.startswith('BUFFER FULL') or l.find('AT') != -1 or l.startswith('>'):
    pass
  elif len(l) < 1:
    pass
  else:
    print 'Unknown (%d): %s' %(len(d), l)

f.close()
