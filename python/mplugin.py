#!/usr/bin/python

# Support for dynamically loaded plugins in python

import imp

def LoadPlugin(path, name, log=None):
  """Load a plugin from the specified path."""

  name = name.replace(' ', '_').lower()
  if log:
    log('Plugin: Loading %s' % name)

  plugin_info = imp.find_module(name, [path])
  return imp.load_module(name, *plugin_info)
