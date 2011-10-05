#!/usr/bin/python

# A tree implementation which supports finding the "nearest" node

import random
import sys
import time

class Tree(object):
  def __init__(self, value, count=0, parent=None):
    self.value = self.SerializeValue(value)
    self.count = count
    self.parent = parent
    self.children = 0

    self.lt = None
    self.gt = None

  def __repr__(self):
    retval = ''
    if self.lt:
      retval += '%r ' % self.lt
    retval += '%s(%d)' %(self.value, self.count)
    if self.gt:
      retval += ' %r' % self.gt
    return retval

  def __len__(self):
    return self.children + 1

  def __iter__(self):
    if self.lt:
      for n in self.lt:
        yield n
    yield self
    if self.gt:
      for n in self.gt:
        yield n

  def Find(self, query):
    query = self.SerializeValue(query)
    if query == self.value:
      return self

    if self.lt is not None and query < self.value:
      return self.lt.Find(query)
    if self.gt is not None and query > self.value:
      return self.gt.Find(query)
    return self

  def GetValue(self):
    return self.UnserializeValue(self.value)

  def GetCount(self):
    return self.count

  def Increment(self):
    self.count += 1

  def _Rebalance(self):
    items = list(self.__iter__())
    if len(items) != 5:
      raise Exception('Incorrect number of items')

    self.value = items[2].value
    self.count = items[2].count

    self.lt = Tree(items[1].value, count=items[1].count, parent=self)
    self.lt.children = 1
    self.lt.lt = Tree(items[0].value, count=items[0].count, parent=self.lt)
    self.gt = Tree(items[3].value, count=items[3].count, parent=self)
    self.gt.children = 1
    self.gt.gt = Tree(items[4].value, count=items[4].count, parent=self.gt)

  def Insert(self, query):
    query = self.SerializeValue(query)

    if self.lt is None and query < self.value:
      self.lt = Tree(query, parent=self)
      return self.lt
    elif self.gt is None and query > self.value:
      self.gt = Tree(query, parent=self)
      return self.gt
    else:
      raise Exception('Insertion error. Are you at the intended parent?\n'
                      'Value:  %r\n'
                      'Left:   %r\n'
                      'Right:  %r\n'
                      'Insert: %r'
                      %(self.value, self.lt, self.gt, query))

  def _SubNodeLenSafe(self, node):
    if not node:
      return 0
    return len(node)

  def Depth(self):
    depth = 0
    if self.lt:
      depth = self.lt.Depth()
    if self.gt:
      depth = max(depth, self.gt.Depth())
    return depth + 1

  def SerializeValue(self, value):
    if type(value) == str and value.startswith('Serialized:'):
      return value

    out = []
    for item in value:
      if type(item) == int:
        out.append('%016d' % item)
      elif type(item) == str:
        out.append(item)
      else:
        print 'Unknown type: %s for %s' %(type(item), item)
        out.append(item)

    try:
      return 'Serialized:%s' % '-'.join(out)
    except Exception, e:
      print 'Serialization failed:'
      print e
      print repr(out)
      sys.exit(1)

  def UnserializeValue(self, value):
    out = []
    for item in value[len('Serialized:'):].split('-'):
      try:
        out.append(int(item))
      except Exception, e:
        print 'Unserialization failed:'
	print e
        print repr(value)
        sys.exit(1)
    return out
    

if __name__ == "__main__":
  nums = range(1000)
  random.shuffle(nums)

  start_time = time.time()
  t = Tree(nums[0])
  for num in nums[1:]:
    node = t.Find(num)
    if node.GetValue() == num:
      node.Increment()
    else:
      node = node.Insert(num)
      node.Increment()

  print '\nTook %.02f seconds' %(time.time() - start_time)

  start_time = time.time()
  print '\nLength: %d' % len(t)
  print 'Took %.02f seconds' %(time.time() - start_time)

  start_time = time.time()
  print '\nDepth: %d' % t.Depth()
  print 'Took %.02f seconds' %(time.time() - start_time)
