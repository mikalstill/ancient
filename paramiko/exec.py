#!/usr/bin/python

# A simple command example for Paramiko.
# Args:
#   1: hostname
#   2: username
#   3: command to run

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

# Start a cmd channel
cmd_channel = t.open_session()
cmd_channel.exec_command(sys.argv[3])

data = cmd_channel.recv(1024)
while data:
  sys.stdout.write(data)
  data = cmd_channel.recv(1024)

# Cleanup
cmd_channel.close()
t.close()
sock.close()
