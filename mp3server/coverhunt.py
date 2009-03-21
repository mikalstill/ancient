#!/usr/bin/python


"""Fetch album art from coverhunt.com."""

# Note that the coverhunt.com site legal page has no acceptable use statement,
# so I just assume this traffic is ok with them.

import base64
import copy
import cStringIO
import datetime
import MySQLdb
import re
import urllib

import database
import sys


_RESULTS_RE = re.compile('.*?<a href="(\/go\/[^"]+)".*Browse full sized.*')
_IMAGE_RE = re.compile('.*<img src="(http:\/\/ecx.images-amazon.com\/images[^"]+)".*')


class Art:
  """Artwork for an album."""

  def __init__(self, artist, album):
    self.artist = artist.replace('_', ' ')
    self.album = album.replace('_', ' ')

  def Search(self):
    """Try to find album art."""

    url = ('http://www.coverhunt.com/search/%s+%s'
           %(self.artist.replace(' ', '+'), self.album.replace(' ', '+')))
    print '%s Fetching %s' %(datetime.datetime.now(), url)

    try:
      remote = urllib.urlopen(url)
      for l in remote.readlines():
        m = _RESULTS_RE.match(l)
        if m:
          img_page_url = 'http://www.coverhunt.com%s' % m.group(1)
          print '%s Fetching %s' %(datetime.datetime.now(), img_page_url)

          img_page = urllib.urlopen(img_page_url)
          for l in img_page.readlines():
            m = _IMAGE_RE.match(l)
            if m:
              img_url = m.group(1)
              print '%s Fetching %s' %(datetime.datetime.now(), img_url)

              img = urllib.urlopen(img_url)
              img_data = img.read()
              img.close()

              return base64.encodestring(img_data)

          img_page.close()
      remote.close()

    except Exception, e:
      print e
      return None

    return None


if __name__ == '__main__':
  a = Art('morcheeba', 'big calm')
  print a.Search()
