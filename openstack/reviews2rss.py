#!/usr/bin/python

# Take gerrit status feeds and turn them into an RSS feed

import subprocess

def CreateRss(component, status):
  p = subprocess.Popen('ssh review.openstack.org gerrit query '
                       'status:%s project:%s' %(status, component),
                       shell=True,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)

  f = open('%s_%s.rss' %(component, status), 'w')

  f.write("""<?xml version="1.0" ?>
  <rss version="2.0">
  <channel>
     <title>%s code reviews (%s)</title>
     <link>https://review.openstack.org/</link>
     <description>openstack/nova code reviews</description>
     <language>en</language>
     <generator>stillhq.com</generator>
     <ttl>180</ttl>
     <docs>http://blogs.law.harvard.edu/tech/rss</docs>
  """ %(component, status))

  keys = []
  values = {}
  for l in p.stdout.readlines():
    l = l.strip().rstrip()

    if len(l) == 0 and values and 'subject' in values:
      description = []
      for key in keys:
        v = values[key]

        if key == 'url':
          description.append('&lt;b&gt;%s:&lt;/b&gt; '
                             '&lt;a href="%s"&gt;%s&lt;/a&gt;'
                             %(key, v, v))
        else:
          description.append('&lt;b&gt;%s:&lt;/b&gt; %s' %(key, v))
      values['description'] = '&lt;br/&gt;'.join(description)

      values['component'] = component
      f.write("""  <item>
      <title>%(component)s: %(subject)s (%(status)s)</title>
      <pubDate>%(lastUpdated)s</pubDate>
      <description>
      %(description)s
      </description>
      <link>%(url)s</link>
      <guid isPermaLink="true">%(url)s</guid>
    </item>
    """ % values)

      values = {}
      keys = []

    elif l.startswith('change '):
      values['change'] = ' '.join(l.split(' ')[1:])

    else:
      elems = l.split(': ')
      values[elems[0]] = ': '.join(elems[1:])
      if elems[0] not in keys:
        keys.append(elems[0])

  f.write("""</channel>
  </rss>
  """)
  f.close()


if __name__ == '__main__':
  CreateRss('openstack/nova', 'open')
  CreateRss('openstack/nova', 'merged')
