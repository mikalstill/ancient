#!/usr/bin/env python
"""Python Interface to the Current Cost CC128 Power Meter

This module provides two main classes:

  - CurrentCostReader - the interface object
  - CurrentCostReading - a class to represent a reading from the device.
    - CurrentCostUsageReading

Example usage:

  import pycurrentcost

  cc = pycurrentcost.CurrentCostReader(port='/dev/tty.usbserial')
  reading = cc.get_reading()
  print reading

"""

import os
import serial
import logging
import xml.dom.minidom

class CurrentCostReading(object):
  """A base class to represent a single reading from the device."""
  def __init__(self, **kwargs):
    # The device name of the erial port.
    self.port = kwargs.get('port', None)
    # The version of the device, as shown in <src> tag
    self.version = kwargs.get('version', None)
    # Radio ID returned by the sensor (<id> tag)
    self.radio_id = kwargs.get('radio_id', None)
    # Last poll time (<time> tag)
    self.poll_time = kwargs.get('poll_time', None)
    # The temperature at the display unit
    self.temperature = kwargs.get('temperature', None)
    # The raw XML string this object was built from
    self.xml_str = kwargs.get('xml_str', None)
    # The DOM object for that string
    self.xml_dom = None

  def __str__(self):
    doc_str = '''
Serial port : %s
Device Version : %s
Sensor Radio ID : %s
Poll Time : %s
Temperature : %s
XML Response : %s''' % (self.port, self.version, self.radio_id,
                        self.poll_time, self.temperature, self.xml_str)
    return doc_str

  def dom(self, xml_str=None):
    """Returns the DOM object for this object. Overwrites the XML for the
    object, if provided"""

    if not xml_str and not self.xml_str:
      raise ValueError, 'cannot generate DOM, no xml present or provided'
    else:
      if not self.xml_str:
        self.xml_str = xml_str

    # passed in value wins over existing value
    if xml_str and xml_str != self.xml_str:
      self.xml_str = xml_str

    try:
      self.xml_dom = xml.dom.minidom.parseString(self.xml_str)
    except xml.parsers.expat.ExpatError, e:
      print 'Error parsing XML Response : %s' % (e)
      print 'XML was : %s' % (self.xml_str)
      raise

    return self.xml_dom

  def populate(self):
    """Populates the object attributes from the xml_str attribute"""
    if not self.xml_str:
      raise CurrentCostException, 'populate() called on object with no XML'

    # remove occasional stupidness from the XML the device sends back.
    print 'Populating reading object from XML : %s' % self.xml_str
    self.fix_xml()
    print 'Fixed XML : %s' % self.xml_str

    self.xml_dom = self.dom()
    self.version = get_single_tag_contents(self.xml_dom, 'src')
    self.radio_id = get_single_tag_contents(self.xml_dom, 'id')
    self.poll_time = get_single_tag_contents(self.xml_dom, 'time')
    self.temperature = get_single_tag_contents(self.xml_dom, 'tmpr')


  def fix_xml(self):
    """The serial interfce sometimes returns something silly.
    The two examples I've seen so far are a forward-slash 
    before the line, or a missing opening <"""
    xml_str = self.xml_str

    if xml_str.startswith('msg'):
      xml_str = '<%s' % (xml_str)

    if xml_str.startswith('/<msg'):
      xml_str = xml_str[1:]

    # Mikal: sometimes we get more than one message, with the first one
    # being truncated...
    msgs = xml_str.split('<msg><src>')[1:]
    print 'Messages: %s' % '\n  '.join(msgs)
    if len(msgs) > 1:
      print 'Taking the second message'
      xml_str = '<msg><src>%s' % msgs[1]

    self.xml_str = xml_str

class CurrentCostUsageReading(CurrentCostReading):
  """A Usage reading, the most common kind of reading. 
    This basically consists of the info common to all readings, 
    plus readings from individual channels (usually just watts)"""
  def __init__(self, **kwargs):
    CurrentCostReading.__init__(self, **kwargs)
    # a dict of channel# -> watts
    self.channels = {}

    if self.xml_str is not None:
      self.populate()

  def __str__(self):
    doc_str = CurrentCostReading.__str__(self)
    doc_str = doc_str + 'Channel Readings: \n'
    if len(self.channels) == 0:
      doc_str = doc_str + 'None'
    for i in self.channels.keys():
      doc_str = doc_str + '\t%s = %s' % (i, self.channels[i])
    return doc_str

  def populate(self):
    """Populate the object, inclding data from <ch...> tags"""
    CurrentCostReading.populate(self)
    # Get contents of <ch...> tags
    for i in range(1, 9):
      data = get_nested_tag_contents(self.xml_dom, 'ch%d' % (i))
      if data:
        print 'Channel info : ch%s = %s' %(i, data)
        self.channels.setdefault(i, data)
    
def get_single_tag_contents(node, tag):
  """Given a DOM node object and a tag name, returns the text of what's in 
that tag.
This is only for <src>, <tmpr>, <sensor> tags, etc. Tags with tags 
inside should use get_nested_tag_contents"""

  tags = node.getElementsByTagName(tag)

  if not tags:
    return None

  if len(tags) > 1:
    raise ValueError, 'Number of <%s> tags >1' % (tag)

  element = tags[0]

  for node in element.childNodes:
    if node.nodeType == node.TEXT_NODE:
      return node.data

def get_nested_tag_contents(node, tag):
  """Given a DOM node and a tag name, returns a dict of tags 
and contents from inside it.
i.e. given <bing><dzzt1>1</dzt1><dzzt2>2</dzzt2></bing> 
we would return { 'dzzt1' : 1, 'dzzt2' : 2 }
this is mainly used for the <ch...> tags in usage reports, 
which just have a <watts> tag inside them (for now, I'm assuming)"""

  logging.debug('Looking for contents of %s tags in %s', tag, node.toxml())

  tags = node.getElementsByTagName(tag)

  if not tags:
    return None

  if len(tags) > 1:
    raise ValueError, 'Number of <%s> tags >1' % (tag)

  element = tags[0]

  children = {}
  for node in element.childNodes:
    if node.nodeType == node.TEXT_NODE:
      children.update({ tag : node.data })
    else:
      children.update(
        get_nested_tag_contents(node.parentNode, node.localName))
  return children

class CurrentCostHistoryReading(CurrentCostReading):
  """Placeholder, not implemented"""
  pass

class CurrentCostException(Exception):
  """Generic eception for this module"""
  pass

class CurrentCostReadException(CurrentCostException):
  """Exception for when we have an error reading from the serial port"""
  pass

class CurrentCostReader(object):
  """Interface to the Current Cost CC128 power meter.
  Example usage:

    cc = pycurrentcost.CurrentCostReader(port='/dev/tty.usbserial')
    reading = cc.get_reading()

  """
  def __init__(self, port=None):
    if not port:
      raise ValueError, 'serial port device not specified'
    self.port = port
    if not os.path.exists(port):
      raise ValueError, 'device %s does not exist' % (port, )  

  def get_xml(self):
    """Poll the seril port once, until we get a non-empty line of text from it.
    Return the result unmodified."""
    ser = serial.Serial(self.port,
                        baudrate=57600,
                        bytesize=serial.EIGHTBITS,
                        parity=serial.PARITY_NONE,
                        stopbits=serial.STOPBITS_ONE)
    # alow 3 attempts at getting a response
    read_attempts = 0
    result = None
    while read_attempts < 3:
      read_attempts = read_attempts + 1
      print'Reading from %s, attempt %d' %(self.port, read_attempts)
      result = ser.readline()
      # The unit likes to output random newlines
      if result == '\n':
        continue
      else:
        print 'Read : %s' % result
        break
      
    if read_attempts == 3:
      # Failed to read
      raise CurrentCostReadException(
            'could not read from %s after 3 attempts' % (self.port))

    return result

  def get_reading(self):
    """Obtain a new reading from the device"""
    xml_str = self.get_xml()
    return CurrentCostUsageReading(port=self.port, xml_str=xml_str)

if __name__ == '__main__':
  import sys

  if len(sys.argv) != 2:
    print "Usage: %s <serial port device>" % (sys.argv[0])
    sys.exit(-1)
  cc = CurrentCostReader(sys.argv[1])
  reading = cc.get_reading()
  print reading.xml_str
