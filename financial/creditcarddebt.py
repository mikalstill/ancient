#!/usr/bin/python

# Graph credit card debt per account in Australia based on RBA data.

# TODO(mikal): also graph per human over the age of 18...

import sys
sys.path.append('/data/src/stillhq_public/trunk/python/')

import datetime
import dateutil.relativedelta
import tempfile
import time
import os
import xlrd

import cachedfetch
import readexcel
import substitute


# Templates
f = open(sys.argv[0].replace('py', 'json'))
json_template = f.read()
f.close()

f = open(sys.argv[0].replace('py', 'json.element'))
json_element_template = f.read()
f.close()

# Load inflation data -- this needs to be renormalized
(fd, inflation_fname) = tempfile.mkstemp()
os.write(fd,
         cachedfetch.Fetch('http://www.rba.gov.au/statistics/tables/xls/'
                           'g02hist.xls'))
os.close(fd)

inflation = {}
current_inflation = 100.0
xls = readexcel.readexcel(inflation_fname)
for row in xls.iter_list('Data'):
  # Rows:
  #  - 0: Month-Year
  #  - 1: All groups
  #  - 2: Tradables
  #  - 3: Tradables excluding food
  #  - 4: Non-tradables
  #  - 5: Food
  #  - 6: Alcohol and tobacco
  #  - 7: Clothing and footwear
  #  - 8: Housing
  #  - 9: Household contents and services
  #  - 10: Health
  #  - 11: Transportation
  #  - 12: Communication
  #  - 13: Recreation
  #  - 14: Education
  #  - 15: Financial and insurance services

  try:
    current_inflation = float(row[1])
    (year, month, day) = row[0].split('/')
    dt = datetime.datetime(int(year), int(month), 1)
    for i in range(3):
      inflation['%s-%s' % (dt.year, dt.month)] = float(row[1])
      dt -= dateutil.relativedelta.relativedelta(months=+1)
  except:
    pass

# Now load credit card data
(fd, ccdebt_fname) = tempfile.mkstemp()
os.write(fd,
         cachedfetch.Fetch('http://www.rba.gov.au/statistics/tables/xls/'
                           'c01hist.xls'))
os.close(fd)

xls = readexcel.readexcel(ccdebt_fname)

peraccount_values = []
inflation_corrected_values = []

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
  #  - 5: Value of purchases (millions)
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
      accounts = float(row[1]) * 1000
      balance = (float(row[10]) * 1000000)
      balance_per_account = balance / accounts

      day_tuple = dt.timetuple()
      epoch = time.mktime(day_tuple)
      x_labels.append(epoch)

      # Not corrected for inflation
      peraccount_values.append('{"x": %d, "y": %f}' %(epoch,
                                                      balance_per_account))

      # Corrected for inflation
      previous_inflation = inflation['%s-%s' % (dt.year, dt.month)]
      correction = current_inflation / previous_inflation
      inflation_corrected_values.append('{"x": %d, "y": %f}'
                                        %(epoch,
                                          balance_per_account * correction))

      i += 1

      sys.stderr.write('%s: %d accounts, %d debt, %d per account, '
                       '%d corrected per account\n'
                       %(dt.strftime('%m/%Y'), accounts, balance,
                         balance_per_account, balance_per_account * correction))

      if balance_per_account > max_balance:
          max_balance = balance_per_account
    except Exception, e:
      sys.stderr.write('Exception %s: %s, %s\n' %(e, row[0], row[10]))

# Write output graph
per_elem = substitute.substitute(json_element_template,
                                 subst={'color': 'FF0000',
                                        'name': 'Balance per account',
                                        'values': ', '.join(peraccount_values),
                                        'x_step': 36})
inf_elem = substitute.substitute(json_element_template,
                                 subst={'color': '00FF00',
                                        'name': ('Balance per account '
                                                 '(corrected for inflation)'),
                                        'values':
                                          ', '.join(inflation_corrected_values),
                                        'x_step': 36})

out = substitute.substitute(json_template,
                            subst={'title': 'Australian credit card debt',
                                   'elements': '%s,\n%s' %(per_elem, inf_elem),
                                   'x_min': x_labels[0],
                                   'x_max': x_labels[-1],
                                   'x_step': (x_labels[-1] - x_labels[0]) / 20,
                                   'x_format': '#date:M Y#',
                                   'y_max': max_balance,
                                   'y_step': max_balance / 20})

print out
