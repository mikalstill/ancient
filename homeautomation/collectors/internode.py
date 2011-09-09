#!/usr/bin/python

# If you wish to create your own usage meter interface, do not copy the 
# interface from this program, please contact Internode via 
# http://www.internode.on.net/contact/support/ for the API document

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import time
import xml.etree.ElementTree
import yaml

import cachedfetch

BASEURL = 'https://customer-webtools-api.internode.on.net/api/v1.5'
AUTH = yaml.load(open('/home/mikal/.internode'))
DAYS_TO_FETCH = 7

# Note that internode data is only updated once per hour
def req(url=[], params=[], node=['api', 'services'], maxage=3600):
  """Make an internode api request."""

  url.insert(0, BASEURL)
  svcs = cachedfetch.Fetch('/'.join(url) + '?' + '&'.join(params),
                           maxage=maxage,
                           username=AUTH['username'], 
                           password=AUTH['password'],
                           useragent='stillhq.com home automation/$Rev$')
  x = xml.etree.ElementTree.XML(svcs)

  for n in node:
    x = x.find(n)
  return x

def Collect(cursor):
  # Find out what services are enabled for this user
  print 'Fetching information about this user'
  for service in req([], [], ['api', 'services']):
    srv_type = service.attrib['type']
    srv_id = service.text

    # Then find out what resources we can access for that service
    print 'Fetching resource list for service id %s' % srv_id
    for resource in req([srv_id], [], ['api', 'resources']):
      rsc_type = resource.attrib['type']
      rsc_id = resource.text

      # Grab everything we can for this resource
      if rsc_type == 'history':
        # Traffic totals for days in the past
        for day in req([srv_id, rsc_id],
                       ['verbose=1', 'count=%s' % DAYS_TO_FETCH],
                       ['api', 'usagelist']):
          dt = datetime.datetime(*time.strptime(day.attrib['day'],
                                                "%Y-%m-%d")[0:5])
          epoch = time.mktime(dt.timetuple())
          for stat in day:
            field = 'History %s %s %s' %(stat.attrib['name'], 
                                         stat.attrib.get('direction', 'all'),
                                         stat.attrib['unit'])
            value = stat.text
            print '  %s = %s @ %s' %(field, value, dt)

            cursor.execute('insert ignore into sensors'
                           '(epoch_seconds, sensor, value, hostname) '
                           'values(%d, "%s", "%s", "internode");'
                           %(epoch, field, value))
            cursor.execute('commit;')

      elif rsc_type == 'usage':
        dt = datetime.datetime.now()
        epoch = time.mktime(dt.timetuple())
        traffic = req([srv_id, rsc_id], [], ['api', 'traffic'])
        field = 'Traffic %s %s' %(stat.attrib['name'], stat.attrib['unit'])
        value = stat.text
        print '  %s = %s @ %s' %(field, value, dt)

        cursor.execute('insert ignore into sensors'
                       '(epoch_seconds, sensor, value, hostname) '
                       'values(%d, "%s", "%s", "internode");'
                       %(epoch, field, value))
        cursor.execute('commit;')

      elif rsc_type == 'resource' or rsc_type == 'service':
        # Nothing interesting for this one
        pass

      else:
        print 'Unknown resource %s!' % rsc_type
