#!/usr/bin/python

# With a big nod to http://www.madox.net/blog/projects/mazda-can-bus/
#
# 100% decoded: 265

import curses
import datetime
import gflags
import gzip
import sys
import time

FLAGS = gflags.FLAGS
gflags.DEFINE_boolean('console', False,
                      'Display decode as a console')
gflags.DEFINE_float('delay', 0.0,
                    'Delay before reading next line. Useful for analysis.')

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

values = {}
def display(opcode, s):
  if FLAGS.console:
    global values
    values[opcode] = s

    keys = values.keys()
    keys.sort()
    i = 1
    stdscr.erase()
    stdscr.addstr(0, 0, 'Last update at %s' % datetime.datetime.now())

    for key in keys:
      stdscr.addstr(i, 0, '%s: %s' %(key, values[key]))
      i += 1
    stdscr.refresh()

  else:
    print s

hud = ''
hud_raw = []

def decode(argv):
  if argv[1] == '-':
    f = sys.stdin
  else:
    f = gzip.open(argv[1])

  l = f.readline()
  while l:
    l = l.rstrip()
    decodeline(l)
    time.sleep(FLAGS.delay)
    l = f.readline()

  f.close()

def decodeline(l):
  global hud
  global hud_raw

  d = l.split(' ')

  # Check packet length
  if d[0] in valid_packet_lengths and valid_packet_lengths[d[0]] != len(d):
    #display(d[0], 'Corrupt: %s' % l)
   return 

  if d[0] == '190':
    if d[1] == '00':
      display(d[0], 'Handbrake off')
    else:
      display(d[0], 'Handbrake on')

  elif d[0] == '201':
    display(d[0], 
            'Engine: RPM %d Undecoded %d Speed %f km/h Accelerator %d'
            %(int('%s%s' %(d[1], d[2]), 16),
              int('%s%s' %(d[3], d[4]), 16),
              int('%s%s' %(d[5], d[6]), 16) / 100.0,
              int(d[7], 16)))

  elif d[0] == '205':
    display(d[0], 'Brake active: %s' % d[1])

  elif d[0] == '228' or d[0] == '231':
    # Does not appear to be valid on my manual Mazda 3 (always zero)
    display(d[0], 'Gear: %s' % d[1])

  elif d[0] == '240':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '265':
    bits = []
    if int(d[1], 16) & 1 << 5:
      bits.append('left')
    if int(d[1], 16) & 1 << 6:
      bits.append('right')
    if not bits:
      bits = ['none']

    display(d[0], 'Blinker: %s' % ' '.join(bits))

  elif d[0] == '285' or d[0] == '511':
    bits = []
    if int(d[1], 16) & 1 << 5:
      bits.append('accessories')
    if int(d[1], 16) & 1 << 6:
      bits.append('on')
    if not bits:
      bits = ['none: %s' % ' '.join(d[1:])]
    display(d[0], 'Key position: %s' % ' '.join(bits))

  elif d[0] == '28F':
    # The three opcodes 28F, 290 and 291 must appear in that order in a block
    # for the HUD to operate

    # TODO(mikal): decode byte five
    bits = []
    if int(d[1], 16) & 1 << 7:
      bits.append('cdin')
    if int(d[1], 16) & 1 << 5:
      bits.append('stereo')

    if d[4] == '01':
      bits.append('hello')
    elif d[4] == '18':
      bits.append('cdtrack')
    elif d[4] == '22':
      bits.append('fmfrequency')

    display(d[0], 'HUD Settings: %s (%s)' %(' '.join(bits), ' '.join(d[1:])))

  elif d[0] == '290' or d[0] == '291':
    if d[0] == '290':
      hud = ''
      hud_raw = []

    out = ('HUD %d/2: ' %(int(d[0]) - 289))
    for i in d[2:]:
      decoded = special_ascii.get(i, '%s' % chr(int(i, 16)))
      hud += decoded
      hud_raw.append(i)

      out += decoded
    out += ' (%s)\n' % ' '.join(d[1:])

    display(d[0], out)
    if d[0] == '291':
      # TODO(mikal): makes the assumption opcode 292 is unused
      display('292', 'HUD Complete: %s (%s)' %(hud, ' '.join(hud_raw)))

  elif d[0] == '400':
    display(d[0],
            'Trip computer: Avg speed %f km/h, Inst Cons %f L/100km, '
            'Cons %d L/100km, Range %f km'
            %(float(int('%s%s' %(d[1], d[2]), 16)),
              float(int('%s%s' %(d[3], d[4]), 16) / 10),
              float(int('%s%s' %(d[5], d[6]), 16) / 10),
              float(int('%s%s' %(d[7], d[8]), 16))))

  elif d[0] == '401':
    # Current best guess: 00 = mode change; 04 is display current;
    # 24 is cycle to next
    display(d[0], 'Trip computer display: %s' % ' '.join(d[1:]))

  elif d[0] == '420':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '430':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '433':
    # The first two bytes are always 00 01 on my Mazda 3 without climate
    # control. Observed values for d[3] and d[7] are at
    # http://spreadsheets.google.com/pub?key=tETfNV_IZXXhoBcw_Fl1lig&single=true&gid=0&output=html

    display(d[0],
            'Climate: Amb %s A/C %s ??? %s (%s)'
            %(int(d[3], 16) / 4.0, d[4] == '08',
              int(d[7], 16) / 4.0, ' '.join(d[1:])))

  elif d[0] == '4B0':
    # I think the units might be off here, there is way too many zero readings
    display(d[0],
            'Wheels: LF %f RF %f LR %f RR %f (km/h)'
            %(float(int('%s%s' %(d[1], d[2]), 16) - 10000) / 100,
              float(int('%s%s' %(d[3], d[4]), 16) - 10000) / 100,
              float(int('%s%s' %(d[5], d[6]), 16) - 10000) / 100,
              float(int('%s%s' %(d[7], d[8]), 16) - 10000) / 100))

  elif d[0] == '4EA':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '4F0':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '4F3':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '501':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif d[0] == '50C':
    display(d[0], 'Undecoded opcode: %s' % l)

  elif (l.startswith('BUFFER FULL') or 
        l.find('AT') != -1 or 
        l.startswith('>') or
        l.find('OK') != -1 or
        l.startswith('ELM') or
        l.startswith('?')):
    pass
  elif len(l) < 1:
    pass
  else:
    display(d[0], 'Unknown (%d): %s' %(len(d), l))


def main(argv):
  # Parse flags
  try:
    argv = FLAGS(argv)

  except gflags.FlagsError, e:
    print 'Flags error: %s' % e
    print
    print FLAGS

  if FLAGS.console:
    global stdscr
    stdscr = curses.initscr()

  try:
    decode(argv)
  finally:
    if FLAGS.console:
      curses.endwin()

if __name__ == "__main__":
  main(sys.argv)
