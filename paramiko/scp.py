#!/usr/bin/python

# A simple scp example for Paramiko.
# Args:
#   1: hostname
#   2: username
#   3: local filename
#   4: remote filename

import getpass
import os
import paramiko
import socket
import sys

# Socket connection to remote host
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((sys.argv[1], 22))

# Build a SSH transport
t = paramiko.Transport(sock)
t.start_client()
t.auth_password(sys.argv[2], getpass.getpass('Password: '))

# Start a scp channel
scp_channel = t.open_session()
          
f = file(sys.argv[3], 'rb')
scp_channel.exec_command('scp -v -t %s\n'
                         % '/'.join(sys.argv[4].split('/')[:-1]))
scp_channel.send('C%s %d %s\n'
                 %(oct(os.stat(sys.argv[3]).st_mode)[-4:],
                   os.stat(sys.argv[3])[6],
                   sys.argv[4].split('/')[-1]))
scp_channel.sendall(f.read())

# Cleanup
f.close()
scp_channel.close()
t.close()
sock.close()
