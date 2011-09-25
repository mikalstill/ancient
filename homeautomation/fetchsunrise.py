#!/usr/bin/python

# Determine sunrise and sunset times. These don't actually change each year, but this
# is the easiest way to plug it into our framework.

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import re
import sys
import urllib

import cachedfetch

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

SUNRISE_RE = re.compile('.*<sunrise>(.*)</sunrise>')
SUNSET_RE = re.compile('.*<sunset>(.*)</sunset>')

def GetSunInfo(month, day):
  sunrise = None
  sunset = None

  remote = cachedfetch.Fetch('http://www.earthtools.org/sun/%s/%s/%d/%d/99/0'
                             %(sys.argv[1], sys.argv[2], day, month),
                             maxage=6*30*24*60*60)
  for l in remote.split('\n'):
    m = SUNRISE_RE.match(l)
    if m:
      sunrise = m.group(1)

    m = SUNSET_RE.match(l)
    if m:
      sunset = m.group(1)

  # Work out a duration
  (risehour, risemin, risesec) = sunrise.split(':')
  (sethour, setmin, setsec) = sunset.split(':')
  rise = datetime.datetime(2010, 1, 1, int(risehour), int(risemin), int(risesec))
  set = datetime.datetime(2010, 1, 1, int(sethour), int(setmin), int(setsec))
  duration = set - rise

  return (sunrise, sunset, duration.seconds)


if len(sys.argv) < 4:
  d = datetime.datetime(datetime.datetime.now().year, 1, 1)
else:
  d = datetime.datetime(int(sys.argv[3]), 1, 1)

one_day = datetime.timedelta(days=1)

for i in range(365):
  (up, down, secs) = GetSunInfo(d.month, d.day)
  print '%d/%d/%d, %s, %s, %s' %(d.year, d.month, d.day, up, down, secs)
  d += one_day
