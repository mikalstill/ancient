#!/usr/bin/python

# Determine sunrise and sunset times. These don't actually change each year, but this
# is the easiest way to plug it into our framework.

import sys
import urllib
import xml.dom.minidom

# URL: http://www.earthtools.org/sun/lat/long/25/06/99/0
# Returns:
# <sun xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://www.earthtools.org/sun.xsd"> 
#   <version>1.0</version> 
#   <location> 
#     <latitude>lat</latitude> 
#     <longitude>long</longitude> 
#   </location> 
#   <date> 
#     <day>25</day> 
#     <month>06</month> 
#     <timezone>10</timezone> 
#     <dst>0</dst> 
#   </date> 
#   <morning> 
#     <sunrise>07:14:34</sunrise> 
#     <twilight> 
#       <civil>06:46:06</civil> 
#       <nautical>06:14:08</nautical> 
#       <astronomical>05:43:00</astronomical> 
#     </twilight> 
#   </morning> 
#   <evening> 
#     <sunset>16:58:08</sunset> 
#     <twilight> 
#       <civil>17:26:36</civil> 
#       <nautical>17:58:34</nautical> 
#       <astronomical>18:29:41</astronomical> 
#     </twilight> 
#   </evening> 
# </sun> 

def GetSunInfo(month, day):
  remote = urllib.urlopen('http://www.earthtools.org/sun/%s/%s/%d/%d/99/0'
                          %(sys.argv[1], sys.argv[2], day, month))
  x = xml.dom.minidom.parse(remote)
  remote.close()

  print dir(x)
  for n in x.childNodes[0].childNodes:
    print
    print n.attributes

GetSunInfo(6, 25)
