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
  '290': 9,
  '291': 9,
  '400': 9,
  '401': 2,
  '420': 9,
  '430': 3,
  '4B0': 9,
  '4EA': 2,
  '4F0': 8,
  '4F3': 4,
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

  elif d[0] == '290' or d[0] == '291':
    sys.stdout.write('HUD %d/2: ' %(int(d[0]) - 289))
    for i in d[1:]:
      sys.stdout.write('%s' % chr(int(i, 16)))
      sys.stdout.flush()
    sys.stdout.write(' (%s)\n' % ' '.join(d[1:]))

  elif d[0] == '400':
    print ('Trip computer: Avg speed %f km/h, Inst Cons %f L/100km, Cons %d L/100km, Range %f km'
           %(float(int('%s%s' %(d[1], d[2]), 16)),
             float(int('%s%s' %(d[3], d[4]), 16) / 10),
             float(int('%s%s' %(d[5], d[6]), 16) / 10),
             float(int('%s%s' %(d[5], d[6]), 16))))

  elif d[0] == '401':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '420':
    print 'Heartbeat counter?'

  elif d[0] == '430':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '4B0':
    # I think the units might be off here, there is way too many zero readings
    print ('Wheels: LF %f RF %f LR %f RR %f (km/h)'
           %(float(int('%s%s' %(d[1], d[2]), 16) - 10000) / 100,
             float(int('%s%s' %(d[3], d[4]), 16) - 10000) / 100,
             float(int('%s%s' %(d[5], d[6]), 16) - 10000) / 100,
             float(int('%s%s' %(d[7], d[8]), 16) - 10000) / 100))

  elif d[0] == '4EA':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '4F0':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '4F3':
    print 'Undecoded opcode: %s' % l

  elif (l.startswith('BUFFER FULL') or 
        l.find('AT') != -1 or 
        l.startswith('>') or
        l.find('OK') != -1 or
        l.startswith('ELM')):
    pass
  elif len(l) < 1:
    pass
  else:
    print 'Unknown (%d): %s' %(len(d), l)

f.close()
