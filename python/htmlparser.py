#!/usr/bin/python

import re
import urllib2
import HTMLParser

import utility


GOOGLE_URL_RE = re.compile('.*url=(.*)')


class Parser(HTMLParser.HTMLParser):
  def __init__(self):
    # HTMLParser is not a "new style class"!
    HTMLParser.HTMLParser.__init__(self)
    self.parsed_text = []
    self.urls = []

  def handle_starttag(self, tag, attrs):
    if tag == 'a':
      for (name, value) in attrs:
        if name == 'href':
          m = GOOGLE_URL_RE.match(value)
          if m:
            if not m.group(1) in self.urls:
              self.urls.append(m.group(1))
          else:
            if not value in self.urls:
              self.urls.append(value)

  def handle_endtag(self, tag):
    pass

  def handle_data(self, data):
    self.parsed_text.append(utility.Normalize(data))

  def get_text(self):
    return ' '.join(self.parsed_text)

  def get_links(self):
    return self.urls
