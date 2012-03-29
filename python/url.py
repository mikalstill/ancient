#!/usr/bin/python

import sys
import urllib
import urlparse

# http://stackoverflow.com/questions/120951/how-can-i-normalize-a-url-in-python.
def quote(s, charset='utf-8'):
  """Sometimes you get an URL by a user that just isn't a real
  URL because it contains unsafe characters like ' ' and so on.  This
  function can fix some of the problems in a similar way browsers
  handle data entered by the user:
  
  >>> url_fix(u'http://de.wikipedia.org/wiki/Elf (Begriffsklarung)')
  'http://de.wikipedia.org/wiki/Elf%20%28Begriffskl%C3%A4rung%29'
  
  :param charset: The target charset for the URL if the url was
  given as unicode string.
  """

  try:
    if isinstance(s, unicode):
      s = s.encode(charset, 'ignore')

    scheme, netloc, path, qs, anchor = urlparse.urlsplit(s)
    path = urllib.quote(path, '/%')
    qs = urllib.quote_plus(qs, ':&=')
    return urlparse.urlunsplit((scheme, netloc, path, qs, anchor))

  except Exception, e:
    print 'Failed to encode: %s' % e
    print 'URL was: %s' % s
    sys.exit(1)
