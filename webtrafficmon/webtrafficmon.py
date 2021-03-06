#!/usr/bin/python

# A simple traffic monitor using pcapy and impacket. This needs to be run as
# root.

import asyncore
import cStringIO
import datetime
import gflags
import impacket
import pcapy
import shelve
import socket
import sys
import time
import thread
import threading
import traceback
import types


from impacket.ImpactDecoder import EthDecoder, LinuxSLLDecoder

FLAGS = gflags.FLAGS
gflags.DEFINE_string('i', 'eth1',
                     'The name of the interface to monitor')
gflags.DEFINE_string('ip', '0.0.0.0', 'Bind to this IP')
gflags.DEFINE_integer('port', 8080, 'Bind to this port')
gflags.DEFINE_boolean('verbose', False, 'Show debug output')


cstart = ('<img src="http://chart.apis.google.com/chart?cht=lc&chs=600x50&'
          'chco=ff0000,00ff00,0000ff&chg=20&chxt=y&')

running = True
maxcount = 100
maxbytes = 100
history = []
persistant = shelve.open('history.%d.shlf' % time.time(), writeback=True)
last_dump = time.time()


def DisplayFriendlySize(bytes, short=False):
  """DisplayFriendlySize -- turn a number of bytes into a nice string"""

  t = type(bytes)
  if t != types.LongType and t != types.IntType and t != decimal.Decimal:
    return 'NotANumber(%s=%s)' %(t, bytes)

  if bytes < 1024:
    if short:
      return '%dB' % bytes
    return '%d bytes' % bytes

  if bytes < 1024 * 1024:
    if short:
      return '%dK' % (bytes / 1024)
    return '%d kb (%d bytes)' %((bytes / 1024), bytes)

  if bytes < 1024 * 1024 * 1024:
    if short:
      return '%dM' % (bytes / (1024 * 1024))
    return '%d mb (%d bytes)' %((bytes / (1024 * 1024)), bytes)

  if short:
    return '%dG' % (bytes / (1024 * 1024 * 1024))
  return '%d gb (%d bytes)' %((bytes / (1024 * 1024 * 1024)), bytes)


class http_server(asyncore.dispatcher):
  """Listen for new client connections, which are then handed off to
     another class
  """

  def __init__(self, ip, port, data_lock):
    self.ip= ip
    self.port = port
    self.data_lock = data_lock
    
    asyncore.dispatcher.__init__(self)
    self.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    self.bind((ip, port))
    self.listen(5)

  def writable(self):
    return 0

  def handle_read(self):
    pass

  def readable(self):
    return self.accepting

  def handle_connect(self):
    pass

  def handle_accept(self):
    conn, addr = self.accept()
    print '%s %s New connection' %(datetime.datetime.now(), repr(addr))
    handler = http_handler(conn, addr, self.data_lock)


class http_handler(asyncore.dispatcher):
  """Handle a single connection"""

  def __init__(self, conn, addr, data_lock):
    asyncore.dispatcher.__init__(self, sock=conn)
    self.addr = addr
    self.data_lock = data_lock
    self.buffer = ''

  def handle_read(self):
    global cstart
    global history
    global persistant
    
    rq = self.recv(1024)
    file = ''
    for line in rq.split('\n'):
      # print '%s %s --> %s' %(datetime.datetime.now(), repr(self.addr), line)
      line = line.rstrip('\r')

      if line.startswith('GET'):
        (_, file, _) = line.split(' ')

    self.data_lock.acquire()
    self.buffer += ('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n'
                    '<html><head>'
                    '<META HTTP-EQUIV="Refresh" CONTENT="30">'
                    '<title>Traffic monitor</title></head>'
                    '<body>')
    if not history or len(history) < 2:
      self.buffer += 'No data yet'
    else:
      self.buffer += ('Only currently active flows with more than one sample '
                      'shown (there are currently %d samples). Only the the '
                      'largest flows by bytes are shown.<br/><br/><table>'
                      % len(history))

      output = {}
      for flow in history[-1]:
        c = 0

        acount = []
        abytes = []
        bcount = []
        bbytes = []

        traffic = 0
        total_traffic = 0
        for h in history:
          d = h.get(flow, None)
          if d:
            c += 1
            (ac, ab, bc, bb) = d
            acount.append('%s.0' % ac)
            abytes.append('%s.0' % ab)
            bcount.append('%s.0' % bc)
            bbytes.append('%s.0' % bb)
            traffic = ab + bb
            total_traffic += traffic
          else:
            acount.append('0.0')
            abytes.append('0.0')
            bcount.append('0.0')
            bbytes.append('0.0')
            traffic = 0

        if c > 1 and traffic > 0:
          e = output.get(traffic, [])
          e.append('<td>%s</td><td>'
                   '%schxl=0:|0|%d&chds=0,%d,0,%d&chd=t:%s|%s"><br/>'
                   '%schxl=0:|0|%s&chds=0,%d,0,%d&chd=t:%s|%s"><br/>'
                   '</td><td>%s</td></tr>'
                   %(flow,
                     cstart, maxcount, maxcount, maxcount,
                     ','.join(acount), ','.join(bcount),
                     cstart, DisplayFriendlySize(maxbytes, short=True),
                     maxbytes, maxbytes,
                     ','.join(abytes), ','.join(bbytes),
                     DisplayFriendlySize(total_traffic)))
          output[traffic] = e

      keys = output.keys()
      keys.sort(reverse=True)
      bgcolor = ''
      for key in keys[:10]:
        for e in output[key]:
          self.buffer += '<tr%s>%s\n' %(bgcolor, e)
          
          if bgcolor:
            bgcolor = ''
          else:
            bgcolor = ' bgcolor="#DDDDDD"'

    self.buffer += '</table></body></html>'
    print '%s %s Served HTTP request' %(datetime.datetime.now(),
                                        repr(self.addr))
    self.data_lock.release()

  def writable(self):
    return len(self.buffer) > 0

  def handle_write(self):
    sent = self.send(self.buffer)
    self.buffer = self.buffer[sent:]
    if len(self.buffer) == 0:
      self.close()

  def handle_close(self):
    pass


class Sniffer(threading.Thread):
  def __init__(self, data_lock):
    self.data_lock = data_lock
    threading.Thread.__init__(self)
  
  def run(self):
    global maxcount
    global maxbytes
    global history
    global persistant
    global running
    global last_dump
    
    flows = {}
    count = 0

    # Arguments here are:
    #   device
    #   snaplen (maximum number of bytes to capture _per_packet_)
    #   promiscious mode (1 for true)
    #   timeout (in milliseconds)
    cap = pcapy.open_live(FLAGS.i, 1500, 1, 1000)

    # We get layer 1 packets. Therefore we need to use the right decoder.
    datalink = cap.datalink()
    if pcapy.DLT_EN10MB == datalink:
      decoder = EthDecoder()
    elif pcapy.DLT_LINUX_SLL == datalink:
      decoder = LinuxSLLDecoder()
    else:
      print '%s Datalink type not supported: ' %(datetime.datetime.now(),
                                                 datalink)
      sys.exit(1)

    # Read packets -- header contains information about the data from pcap,
    # payload is the actual packet as a string
    (header, payload) = cap.next()
    while header:
      count += 1
      if FLAGS.verbose:
        print ('%s captured %d bytes, truncated to %d bytes'
               %(datetime.datetime.now(), header.getlen(), header.getcaplen()))

      try:
        # The link level packet contains a payload 
        l = decoder.decode(payload)
        p = l.child()
        key = None

        if p.ethertype == 0x800:
          # This is an IP packet
          ip = p.child()
          ips = [p.get_ip_src(), p.get_ip_dst()]
          ips.sort()

          if ip.protocol == 1:
            # ICMP
            if FLAGS.verbose:
              print ('  ICMP: %s -> %s type %s'
                     %(p.get_ip_src(), p.get_ip_dst(),
                       ip.get_type_name(ip.get_icmp_type())))

            key = 'ICMP %s' % repr(ips)

          elif ip.protocol == 6:
            # TCP
            if FLAGS.verbose:
              print '  TCP: %s:%d -> %s:%d' %(p.get_ip_src(),
                                              ip.get_th_sport(),
                                              p.get_ip_dst(),
                                              ip.get_th_dport())

            ports = [ip.get_th_sport(), ip.get_th_dport()]
            ports.sort()
            key = 'TCP %s %s' %(repr(ips), repr(ports))

          elif ip.protocol == 17:
            # UDP
            if FLAGS.verbose:
              print '  UDP: %s:%d -> %s:%d' %(p.get_ip_src(),
                                              ip.get_uh_sport(),
                                              p.get_ip_dst(),
                                              ip.get_uh_dport())

            ports = [ip.get_uh_sport(), ip.get_uh_dport()]
            ports.sort()
            key = 'TCP %s %s' %(repr(ips), repr(ports))

          elif ip.protocol:
            print '%s Unknown IP protocol %s' %(datetime.datetime.now(),
                                                ip.protocol)

          if key:
            flows.setdefault(key, (0, 0, 0, 0))
            (a_count, a_bytes, b_count, b_bytes) = flows[key]
            if ips == [p.get_ip_src(), p.get_ip_dst()]:
              a_count += 1
              a_bytes += header.getlen()
            else:
              b_count += 1
              b_bytes += header.getlen()
            flows[key] = (a_count, a_bytes, b_count, b_bytes)

        else:
          print '%s Unknown ethertype %x' %(datetime.datetime.now(),
                                            p.ethertype)

      except impacket.ImpactPacket.ImpactPacketException, e:
        print '%s Sniffer skipped packet: %s' %(datetime.datetime.now(), e)

      self.data_lock.acquire()
      if time.time() - last_dump > 30:
        # Recalibrate maximums
        maxcount = 100
        maxbytes = 100
        for flow in flows:
          (acount, abytes, bcount, bbytes) = flows[flow]
          if acount > maxcount:
            maxcount = acount
          if bcount > maxcount:
            maxcount = bcount
          if abytes > maxbytes:
            maxbytes = abytes
          if bbytes > maxbytes:
            maxbytes = bbytes

        # Append to history
        history.append(flows)
        history = history[-30:]

        # Write to persistant history as well
        persistant.setdefault('history', {})
        persistant['history'][time.time()] = flows
        persistant.sync()
        
        if len(persistant['history']) > 29:
          print '%s Committing suicide' % datetime.datetime.now()
          persistant.close()
          running = False
          raise SystemExit()
        
        print ('%s Sniffer captured %d packets in the last 30 seconds'
               %(datetime.datetime.now(), count))
        count = 0
        flows = {}
        last_dump = time.time()

      self.data_lock.release()
      del header
      del payload
      (header, payload) = cap.next()


def main(argv):
  global running
  
  # Parse flags
  try:
    argv = FLAGS(argv)
  except gflags.FlagsError, e:
    print FLAGS

  data_lock = thread.allocate_lock()
  server = http_server(FLAGS.ip, FLAGS.port, data_lock)

  # Start the sniffer thread
  sniffer = Sniffer(data_lock)
  sniffer.start()

  # Start the web server, which takes over this thread
  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)

  while running:
    print '%s ... %d' %(datetime.datetime.now(), time.time() - last_dump)

    data_lock.acquire()
    if time.time() - last_dump > 180:
      print '%s Dump stale' % datetime.datetime.now()
      running = False
    data_lock.release()

    asyncore.loop(timeout=10.0, count=1)


if __name__ == "__main__":
  main(sys.argv)
