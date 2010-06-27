#!/usr/bin/python

import sys
import os
import re
import getopt
import datetime
import bz2

#For opening the URL
import socket
import urllib2
import urllib
import cookielib


BOMSOLARURL = 'http://www.bom.gov.au/web03/ncc/www/awap/solar/solarave/daily/grid/0.05/history/nat/'
BOMTEMPURL =  'http://www.bom.gov.au/web03/ncc/www/awap/temperature/maxave/daily/grid/0.05/history/nat/'
SUFFIX = '.grid'
SUFFIXC = '.Z'
LZWURL = 'http://www.avenard.org/unzip/?url='

__doc__ = """Meteo Data retrieval tool

Copyright (c) 2010 Jean-Yves Avenard and Hydrix Pty Ltd
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Jean-Yves Avenard and Hydrix Pty Ltd nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Solar data retrieved is (c) Copyright Commonwealth of Australia 2010, Bureau of Meteorology (ABN 92 637 533 532)
Check their copyright notice and license agreement:
http://www.bom.gov.au/other/copyright.shtml

Solar exposure data derived from satellite imagery processed by the Bureau of
Meteorology from the Geostationary Meteorological Satellite series operated by
Japan Meteorological Agency and from GOES-9 operated by the National
Oceanographic & Atmospheric Administration (NOAA) for the Japan Meteorological
Agency
http://www.bom.gov.au/climate/austmaps/metadata-daily-solar-exposure.shtml

Daily temperature data is (c) Copyright Commonwealth of Australia 2010, Bureau of Meteorology (ABN 92 637 533 532)
Check their copyright notice and license agreement:
http://www.bom.gov.au/climate/austmaps/metadata-daily-temperature.shtml

Usage: meteodata.py [options]

Options:
    -h / --help
        Print this message and exit.

    -d / --date [YYYYMMDD][:YYYMMDD]
        Retrieve data for date DD/MM/YYYY, default is data of yesterday
        or between two provided date
        (retrieval takes around 10s per day if not cached)

    -l / --lat [latitude]
        Retrieve data for given latitude only

    -L / --lon [longitude]
        Retrieve data for given longitude only

    -r / --raw
        Print raw data retrieved

    -c / --cache
        Cache the data for future retrieval
    
    -p / --path
        Where to write or read cache files (default is current directory)

    -s / --silent
        Don't print progress statements
        
    -t / --table
        Output data in array/table format
        
    -o / --output [filename]
        By default, the output is directed to stdout. Write standard output to
        filename instead.
        
    -e / --empty [value]
        Use value for unknown data ; default is -1

    -T / --temp
        Retrieve temperature data

    -S / --solar
        Retrieve solar exposure data (default)
"""

errorstr = ""

class Error(Exception):
	def __init__(self, message,code):
		self.message = message
		self.code = code
	def __str__(self):
		return repr(self.message)

class Web:
	def __init__(self, url, timeout=30, data=None):
		self.handle = None
	
		if data:
			txdata = urllib.urlencode(data)
		else:
			txdata = None
		txheaders =  {'User-agent' : 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)'}

		# timeout in seconds
		socket.setdefaulttimeout(timeout)

		try:
			req = urllib2.Request(url, txdata, txheaders)
			# create a request object

			self.handle = urllib2.urlopen(req)

		except IOError, e:
			errorstr = 'We failed to open "%s".' % url
			if hasattr(e, 'code'):
				errorstr += '\nWe failed with error code - %s.' % e.code
			elif hasattr(e, 'reason'):
				errorstr += "\nThe error object has the following 'reason' attribute :\n"
				errorstr += repr(e.reason)
				errorstr += "\nThis usually means the server doesn't exist, is down, or we don't have an internet connection."
			raise Error(errorstr,200)

	def read(self):
		return self.handle.read()

	def close(self):
		return self.handle.close()

	def readline(self):
		return self.handle.readline()

def usage():
	print __doc__

def test(condition, true, false):
    """
    Implements the C expression:

      condition ? true : false

    Required to correctly interpret plural forms.
    """
    if condition:
        return true
    else:
        return false

def fetch(date, lat=False, lon=False, raw=False, cache=False, path='.', days = 1, silent=False, csv=True, datatype=0, empty=-1):
    datastr = [ "solar", "temperature" ]

    s = ""
    for i in range(days):
        datetxt = '%04d%02d%02d' % (date.year, date.month, date.day)
        if not silent:
            print "retrieving " + datetxt + "/" + datastr[datatype],
        #Check if data is cached
        if datatype > 0:
            filename = "%s/%s%s.%d%s.bz2" % (path, datetxt, datetxt, datatype, SUFFIX)
        else:
            filename = "%s/%s%s%s.bz2" % (path, datetxt, datetxt, SUFFIX)
            
        found = False
        try:
            handle = open(filename,"rb")
            if not silent:
                print " (cached)",
            found = True
        except:
            url = '%s%s%s%s%s%s' % (LZWURL, test(datatype,BOMTEMPURL,BOMSOLARURL), datetxt, datetxt, SUFFIX, SUFFIXC )
            handle = Web(url)
        if not silent:
            print
        content = handle.read()
        stream = bz2.decompress(content)
        handle.close()

        if len(stream) == 0:
            raise Error( "Invalid URL (probably incorrect date)", 1)

        #cache data if requested
        if cache and not found:
            handle = open(filename,"wb")
            handle.write(content)
            handle.close()

        if raw:
            print stream
            continue

        else:
            m = re.compile("ncols ([0-9]+)").search(stream)
            if not m:
                raise Error("Can't read number of columns",2)
            ncols = int(m.group(1))
            m = re.compile("nrows ([0-9]+)").search(stream)
            if not m:
                raise Error("Can't read number of rows", 3)
                sys.exit(1)
            nrows = int(m.group(1))
            m = re.compile("xllcenter (-?([0-9]*\.)?[0-9]+)").search(stream)
            if not m:
                raise Error( "Can't read longitude", 4)
            xcenter = float(m.group(1))
            m = re.compile("yllcenter (-?([0-9]*\.)?[0-9]+)").search(stream)
            if not m:
                raise Error("Can't read latitude", 5)
            ycenter = float(m.group(1))
            m = re.compile("cellsize (-?([0-9]*\.)?[0-9]+)").search(stream)
            if not m:
                raise Error("Can't read cell size\n",6)
            cellsize = float(m.group(1))
            m = re.compile("nodata_value (-?([0-9]*\.)?[0-9]+)").search(stream)
            if not m:
                raise Error("Can't read nodata value",7)
            nodata = float(m.group(1))
            
            nlon = xcenter - cellsize / 2
            nlat = ycenter - cellsize / 2
            #nlat + nrows * cellsize , nlon     nlat + nrows * cellsize , nlon + ncols * cellsize	
            #
            #
            #
            #nlat,nlon                          nlat  , nlon + ncols * cellsize
            # Check that latitude or longitude provided are within the area
            if lat is not False:
                if lat < nlat or lat > (nlat + nrows * cellsize):
                    raise Error("Invalid latitude (outside the covered zone)",8)
            if lon is not False:
                if lon < nlon or lon > (nlon + ncols * cellsize):
                    raise Error("Invalid longitude (outside the covered zone)",9)
                
            table = []
            lines = stream.split('\n')
            index = 0
            for line in lines:
                numbers = line.split(' ')[1:-1]
                if len(numbers) == ncols:
                    table.append([])
                    for number in numbers:
                        number = round(float(number),2)
                        if number == round(nodata,2):
                            table[-1].append(empty)
                        else:
                            table[-1].append(number)
                    index += 1

            if index != nrows:
                raise Error("Number of rows retrieved incorrect",10)
            indexy = int((nlat + nrows * cellsize - lat) / cellsize)
            indexx = int((lon - nlon) / cellsize)
            indexlat = nlat + nrows * cellsize
            indexlon = nlon

            if lon is False and lat is not False:
                #Just latitude provided
                if csv:
                    s += datetxt + "\n,"
                    for x in range(ncols):
                        s += str(indexlon) + ","
                        indexlon += cellsize
                    s += "\n" + str((nlat + (nrows - indexy) * cellsize)) + ","
                    for x in range(ncols):
                        s += str(table[indexy][x]) + ","
                    s += "\n"
                else:
                    s += str(table[indexy]) + "\n"
            elif lon is not False and lat is False:
                #Just longitude provided
                if csv:
                    s += datetxt + "\n," + str(nlon + indexx * cellsize) + "\n"
                    for y in range(nrows):
                        s += str(indexlat) + ","
                        s += str(table[y][indexx]) + "\n"
                        indexlat -= cellsize
                    s += "\n"
                    
                else:
                    table2 = []
                    for row in table:
                        table2.append([])
                        table2[-1].append(row[indexx])
                    s += str(table2) + "\n"
            elif lat and lon:
                if csv:
                    s += datetxt + "," + str(table[indexy][indexx]) + "\n"
                else:
                    s += str(table[indexy][indexx]) + "\n"
            else:
                if csv:
                    s2 = datetxt + "\n,"
                    for y in range(ncols):
                        s2 += str(indexlon) + ","
                        indexlon += cellsize
                    s += s2[:-1] + "\n"
                    for y in range(nrows):
                        s2 = str(indexlat) + ","
                        indexlat -= cellsize
                        for x in range(ncols):
                            s2 += str(table[y][x]) + ","
                        s += s2[:-1] + "\n"
                else:
                    s += str(table) + "\n"
        date += datetime.timedelta(days=1)
    return s

if __name__ == '__main__':

    date = datetime.datetime.now() - datetime.timedelta(days=1)
    lat = False
    lon = False
    raw = False
    cache = False
    path = '.'
    days = 1
    silent = False
    csv = True
    output = sys.stdout
    empty = -1
    datatype = 0 #0=solar, 1=temp

    try:
        opts, args = getopt.getopt(sys.argv[1:], \
            "hd:l:L:rcp:sto:e:TS", \
            ["help", "date=", "lat=", "lon=", "raw", "cache", "path=", "silent", "table", "output=", "empty=", "temp", "solar"])
    except getopt.GetoptError:
        # print help information and exit:
        print "Unrecognised option: "
        usage()
        sys.exit(2)

    for o, a in opts:
        if o in ["-h", "--help"]:
            usage()
            sys.exit(0)
        elif o in ["-d", "--date"]:   
            dates = a.split(':')
            date = datetime.date(int(dates[0][0:4]), int(dates[0][4:6]), int(dates[0][6:8]))
            if len(dates) == 2:
                days = (datetime.date(int(dates[1][0:4]), int(dates[1][4:6]), int(dates[1][6:8]))
                    - date + datetime.timedelta(days=1)).days
                if days < 0:
                    print "Error with dates:", dates[1], "<", dates[0]
                    sys.exit(3)

        elif o in ["-l", "--lat"]:
            lat = float(a)
        elif o in ["-L", "--lon"]:
            lon = float(a)
        elif o in ["-r", "--raw"]:
            raw = True
        elif o in ["-c", "--cache"]:
            cache = True
        elif o in ["-p", "--path"]:
            path = a
        elif o in ["-s", "--silent"]:
            silent = True
        elif o in ["-t", "--table"]:
            csv = False
        elif o in ["-o", "--output"]:
            output = open(a,'w')
        elif o in ["-e", "--empty"]:
            empty = a
        elif o in ["-T", "--temp"]:
            datatype = 1
        elif o in ["-S", "--solar"]:
            datatype = 0

    try:
        s = fetch(date, lat=lat, lon=lon, raw=raw, cache=cache, path=path, days=days, silent=silent, csv=csv, datatype=datatype, empty=empty)
    except Error, e:
        print e.message
        sys.exit(1)
    if not silent:
        output.write("\n")
    output.write(s)
    if output is not sys.stdout:
        output.close()
    if not silent:
        print "Done"
            
