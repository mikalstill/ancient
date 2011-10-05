#!/usr/bin/python

# Bisect a sorted list and return elements in the right order for tree insertion

class Bisect(object):
  def __init__(self, items):
    self.items = items

  def __iter__(self):
    if not self.items:
      return

    l = len(self.items)
    c = l / 2
    yield self.items[c]
    for item in Bisect(self.items[:c]):
      yield item
    for item in Bisect(self.items[c + 1:]):
      yield item


if __name__ == '__main__':
  for item in Bisect(range(10)):
    print item
