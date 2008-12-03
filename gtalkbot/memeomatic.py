#!/usr/bin/python

import feedparser
import os
import re
import shelve
import sys
import unicodedata
import urllib


_SENTENCE_LENGTH = 5


def Normalize(value):
  normalized = unicodedata.normalize('NFKD', unicode(value))
  normalized = normalized.encode('ascii', 'ignore')
  return normalized


def ListDifference(l1, l2):
  delta = []
  
  for l in l2:
    if l not in l1:
      delta.append(l)

  return len(delta)


plugins_dir = '%s/plugins' % os.getcwd()
print 'Appending %s to module path' % plugins_dir
sys.path.append(plugins_dir)
import blather

data = shelve.open('memes.slf', writeback=True)
data.setdefault('sentences', {})
data.setdefault('titles', {})
data.setdefault('content', {})
data.setdefault('content_orig', {})
data.setdefault('memes', [])

ds = blather.DataStore()

changed = False

# Scan feeds, looking for new posts. This just populates the database.
for feed in data['feeds']:
  print
  print 'Fetching %s' % feed
  d = feedparser.parse(feed)

  # Newest entries are first
  entries = d.entries
  entries.reverse()
  
  for ent in entries:
    print '  Considering %s' % ent.title
    data['titles'][ent.link] = ent.title
    
    content = Normalize(ent.description)
    data['content_orig'][ent.link] = content
    content = ' '.join(content.split('\n'))
    content = re.sub('<[^>]*>', '', content)
    content = re.sub('[^\w]+', ' ', content)
    content = content.lower()
    data['content'][ent.link] = content

    words = content.split()
    for i in range(len(words) - _SENTENCE_LENGTH):
      key = ' '.join(words[i:i + _SENTENCE_LENGTH])
      data['sentences'].setdefault(key, [])

      if not ent.link in data['sentences'][key]:
        data['sentences'][key].append(ent.link)

# Now we have a database of sentences and the posts which share them. What we
# really want is a collection of shared sentences that form a meme, and the
# posts which contain those sentences.
for sentence in data['sentences']:
  found = False

  if len(data['sentences'][sentence]) > 4:
    domains = {}
    
    # Its possible that they're all from one domain...
    for url in data['sentences'][key]:
      domain = url.strip('http://').split('/')[0]
      domains[domain] = True

    # Its not a meme unless the sentence is shared by at least four posts.
    # Try to find an existing meme which contains these posts.
    for (sentences, urls, published) in data['memes']:
      if not found and ListDifference(urls, data['sentences'][sentence]) < 2:
        data['memes'].remove((sentences, urls, published))

        if sentence not in sentences:
          sentences.append(sentence)

        new_titles = []
        for u in data['sentences'][sentence]:
          if not u in urls:
            urls.append(u)
            new_titles.append('<a href="%s">%s</a>'
                              %(u, data['titles'][u]))

        data['memes'].append((sentences, urls, published))
        found = True

        if published and new_titles:
          print 'Added posts to an existing meme'
          ds.AddMessage('Memeomatic extended an existing meme: %s'
                        % ', '.join(new_titles))
          changed = True

    if not found and len(domains) > 1:
      print ('Created a new meme for "%s" with %s'
             %(sentence, data['sentences'][sentence]))
      data['memes'].append(([sentence], data['sentences'][sentence], False))

# Publish new memes
for meme in data['memes']:
  (sentences, urls, published) = meme
  if not published:
    titles = []
    for url in urls:
      titles.append('<a href="%s">%s</a>' %(url, data['titles'][url]))
    
    ds.AddMessage('Memeomatic found a new meme: %s' % ', '.join(titles))
    data['memes'].remove((sentences, urls, published))
    data['memes'].append((sentences, urls, True))
    print 'Published a new meme'
    changed = True

if changed:
  ds.Save()
data.close()
