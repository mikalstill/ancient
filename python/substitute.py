#!/usr/bin/python

# Simple templating system

import re

_SUBST_RE = re.compile('(.*){{([^}]+)}}(.*)', re.MULTILINE | re.DOTALL)

def substitute(data, subst):
  """Perform template substitution."""

  m = _SUBST_RE.match(data)
  while m:
    data = '%s%s%s' %(m.group(1),
                      subst.get(m.group(2), '<i>%s missing</i>'
                                % m.group(2)),
                      m.group(3))
    m = _SUBST_RE.match(data)
    
  return data
