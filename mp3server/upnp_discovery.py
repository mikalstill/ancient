#!/usr/bin/python

# A simple discovery server for uPnP. You must run this on an IP which has
# nothing else listening to UDP port 1900, but you can have this redirect to
# a different IP.


import datetime
import SocketServer
import sys
import uuid

import gflags


FLAGS = gflags.FLAGS
gflags.DEFINE_string('ip', '0.0.0.0', 'Bind to this IP')
gflags.DEFINE_integer('port', 1900, 'Bind to this port')

gflags.DEFINE_string('mp3_ip', '192.168.1.99', 'Send clients to this IP')
gflags.DEFINE_integer('mp3_port', 12345, 'Send clients to this port')


class DiscoveryHandler(SocketServer.DatagramRequestHandler):
  """Handle incoming UDP requests."""

  def handle(self):
    """Respond to requests."""

    is_discover = False
    for line in self.rfile.read().split('\n'):
      print '%s %s %s' %(datetime.datetime.now(),
                         repr(self.client_address),
                         line.rstrip())
      if line.find('ssdp:discover') != -1:
        is_discover = True

    if is_discover:
      self.wfile.write("""HTTP/1.1 200 OK\r
LOCATION: http://%(mp3_ip)s:%(mp3_port)d/getDeviceDesc\r
CACHE-CONTROL: max-age=3600\r
EXT:\r
Server: Linux 2.6.27-11-generic, UPnP/1.0, mp3server 0.1\r
ST: urn:schemas-upnp-org:service:ContentDirectory:1\r
USN: uuid:%(uuid)s::urn:schemas-upnp-org:service:ContentDirectory:1\r
Content-Length: 0\r\n\r""" % { 'mp3_ip': FLAGS.mp3_ip,
                               'mp3_port': FLAGS.mp3_port,
                               'uuid': uuid.uuid4()
                               })
      print '%s %s Redirected to %s:%s' %(datetime.datetime.now(),
                                          repr(self.client_address),
                                          FLAGS.mp3_ip, FLAGS.mp3_port)
    

def main(argv):
  # Parse flags
  try:
    argv = FLAGS(argv)
  except gflags.FlagsError, e:
    print FLAGS

  print '%s Started listening on port %s' %(datetime.datetime.now(),
                                            FLAGS.port)
  SocketServer.UDPServer((FLAGS.ip, FLAGS.port), DiscoveryHandler).serve_forever()


if __name__ == "__main__":
  main(sys.argv)
