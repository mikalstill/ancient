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
  '265': 3,
  '285': 4,
  '28F': 9,
  '290': 9,
  '291': 9,
  '400': 9,
  '401': 2,
  '420': 9,
  '430': 3,
  '433': 9,
  '4B0': 9,
  '4EA': 2,
  '4F0': 8,
  '4F3': 4,
  '501': 4,
  '50C': 4,
  '511': 4,
  }

special_ascii = {
  'E1': ']1[', # Numbers in boxes, blinking
  'E2': ']2[',
  'E3': ']3[',
  'E4': ']4[',
  'E5': ']5[',
  'E6': ']6[',
  'E7': '[1]', # Inverted numbers in boxes
  'E8': '[2]',
  'E9': '[3]',
  'EA': '[4]',
  'EB': '[5]',
  'EC': '[6]',
  'F3': 'C',   # Small caps
  'F4': 'H',
}

hud = ''
hud_raw = []

if sys.argv[1] == '-':
  f = sys.stdin
else:
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

  elif d[0] == '265':
    bits = []
    if int(d[1], 16) & 1 << 5:
      bits.append('left')
    if int(d[1], 16) & 1 << 6:
      bits.append('right')
    if not bits:
      bits = ['none']
    
    print 'Blinker: %s' % ' '.join(bits)

  elif d[0] == '285' or d[0] == '511':
    print 'Key position: %s' % ' '.join(d[1:])

  elif d[0] == '28F':
    print 'Undecoded opcode: %s' % ' '.join(d[1:])

  elif d[0] == '290' or d[0] == '291':
    if d[0] == '290':
      hud = ''
      hud_raw = []

    sys.stdout.write('HUD %d/2: ' %(int(d[0]) - 289))
    for i in d[2:]:
      decoded = special_ascii.get(i, '%s' % chr(int(i, 16)))
      hud += decoded
      hud_raw.append(i)

      sys.stdout.write(decoded)
      sys.stdout.flush()
    sys.stdout.write(' (%s)\n' % ' '.join(d[1:]))

    if d[0] == '291':
      print 'HUD Complete: %s (%s)' %(hud, ' '.join(hud_raw))

  elif d[0] == '400':
    print ('Trip computer: Avg speed %f km/h, Inst Cons %f L/100km, '
           'Cons %d L/100km, Range %f km'
           %(float(int('%s%s' %(d[1], d[2]), 16)),
             float(int('%s%s' %(d[3], d[4]), 16) / 10),
             float(int('%s%s' %(d[5], d[6]), 16) / 10),
             float(int('%s%s' %(d[7], d[8]), 16))))

  elif d[0] == '401':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '420':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '430':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '433':
    # The first two bytes are always 00 01 on my Mazda 3 without climate control
    print 'Climate: Amb %s (%s)' %(int(d[3], 16) / 4.0, ' '.join(d[1:]))

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

  elif d[0] == '501':
    print 'Undecoded opcode: %s' % l

  elif d[0] == '50C':
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
