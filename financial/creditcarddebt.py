#!/usr/bin/python

# Graph credit card debt per account in Australia based on RBA data.

# TODO(mikal): also graph per human over the age of 18...

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import tempfile
import time
import os
import xlrd

import cachedfetch
import readexcel
import substitute

(fd, fname) = tempfile.mkstemp()
os.write(fd,
         cachedfetch.Fetch('http://www.rba.gov.au/statistics/tables/xls/'
                           'c01hist.xls'))
os.close(fd)

f = open(sys.argv[0].replace('py', 'json'))
json_template = f.read()
f.close()

f = open(sys.argv[0].replace('py', 'json.element'))
json_element_template = f.read()
f.close()

xls = readexcel.readexcel(fname)
values = []
json_values = []
x_labels = []
max_balance = 0
i = 0

for row in xls.iter_list('Data'):
  # Rows:
  #  - 0: Month
  #  - 1: Number of accounts ('000)
  #  - 2: Number of cash advances ('000)
  #  - 3: Value of cash advances (millions)
  #  - 4: Number of purchases ('000)
  #  - 5: Value of purchaces (millions)
  #  - 6: Total number of transactions ('000)
  #  - 7: Total value of transactions (millions)
  #  - 8: Repayments (millions)
  #  - 9: Balances accuring interest (millions)
  #  - 10: Total balance (millions)
  #  - 11: Credit limit (millions)

  if row[0].startswith('19') or row[0].startswith('20'):
    day = row[0].split('/')
    dt = datetime.datetime(int(day[0]), int(day[1]), int(day[2]))

    try:
      accounts = float(row[1]) * 100
      balance = (float(row[10]) * 1000000) / accounts
      values.append((dt, balance))
      json_values.append('{"x": %d, "y": %f}' %(i,
                                                balance))
      x_labels.append(dt.strftime('%m/%Y'))
      i += 1

      if balance > max_balance:
          max_balance = balance
    except:
      continue

elem = substitute.substitute(json_element_template,
                             subst={'color': 'FF0000',
                                    'name': 'Balance per account',
                                    'values': ', '.join(json_values),
                                   'x_step': 36})

out = substitute.substitute(json_template,
                            subst={'title': 'Australian credit card debt',
                                   'elements': elem,
                                   'x_min': 0,
                                   'x_max': len(values) - 1,
                                   'x_step': 36,
                                   'x_labels': '", "'.join(x_labels),
                                   'y_max': max_balance,
                                   'y_step': max_balance / 20})

print out
