#!/usr/bin/python

# Push directory.txt to google contacts for the user specified on the command
# line.

# $1: google user to connect as

import atom
import getpass
import re
import sys

import gdata.contacts
import gdata.contacts.client


ENTRY_RE = re.compile('([^:]*): (.*)')


user = sys.argv[1]

gd_client = gdata.contacts.client.ContactsClient(source='ContactsSyncer')
gd_client.ClientLogin(user,
                      getpass.getpass('Password for %s >> ' % user), 
                      gd_client.source)

# Google contacts does not support name searches! So instead we grab all the contacts.
existing = {}

print 'Fetching all existing contacts'
query = gdata.contacts.client.ContactsQuery()
query.max_results = 5000
feed = gd_client.GetContacts(q=query)
for i, entry in enumerate(feed.entry):
    lpid = None
    for ep in entry.extended_property:
        if ep.name == 'launchpadID':
            lpid = ep.value

    if lpid:
        print '  %s (launchpad id = %s)' %(entry.name.full_name.text, lpid)
        existing[lpid] = entry

# Open the cache of LDAP entries
f = open('directory.txt', 'r')
l = f.readline()

while l:
    # Parse the entry
    l = l.rstrip()
    m = ENTRY_RE.match(l)
    entry = m.group(1)
    data = eval(m.group(2))

    # We only process entries with a launchpadID
    if not 'launchpadID' in data:
        continue

    # Push the contact to google
    changed = False
    print 'Processing %s' % entry

    if not data['launchpadID'][0] in existing:
        full_name = gdata.data.FullName(text=entry)
        name = gdata.data.Name(full_name=full_name)
        new_contact = gdata.contacts.data.ContactEntry(name=name)

        lpid = gdata.data.ExtendedProperty(name='launchpadID', value=data['launchpadID'][0])
        new_contact.extended_property.append(lpid)

        op = gd_client.CreateContact
        print '  Creating contact'

    else:
        new_contact = existing[data['launchpadID'][0]]
        op = gd_client.Update
        print '  Updating contact'

    # Email
    if 'mail' in data and data['mail']:
        present = []
        for em in new_contact.email:
            present.append(em.address)

        if len(present) == 0:
            primary = 'true'
        else:
            primary = 'false'

        print '  Email addresses already present: %s' % repr(present)

        for email in data['mail']:
            if not email in present:
                print '    Adding email address: %s (primary = %s)' %(email, primary)
                em = gdata.data.Email(address=email, primary=primary, rel=gdata.data.WORK_REL)
                new_contact.email.append(em)
                primary = 'false'
                changed = True

    # Phone numbers
    phones = []
    if 'telephoneNumber' in data and data['telephoneNumber']:
        for phone in data['telephoneNumber']:
            phones.append((phone, gdata.contacts.PHONE_WORK))
    if 'mobile' in data and data['mobile']:
        for phone in data['mobile']:
            phones.append((phone, gdata.contacts.PHONE_MOBILE))
        
    if phones:
        present = []
        for ph in new_contact.phone_number:
            present.append(ph.text)

        if len(present) == 0:
            primary = 'true'
        else:
            primary = 'false'

        print '  Phone numbers already present: %s' % repr(present)

        for phone, rel in phones:
            if not phone in present:
                print '    Adding phone number: %s' % phone
                ph = gdata.data.PhoneNumber(text=phone,
                                            primary=primary,
                                            rel=rel)
                new_contact.phone_number.append(ph)
                primary = 'false'
                changed = True
    
    if changed:
        contact_entry = op(new_contact)
    else:
        print '  No changes to save'

    # Read
    l = f.readline()

print 'Done'
