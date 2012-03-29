#!/bin/bash

dest=`cat ~/.openstack_rss_destination`

./reviews2rss.py
scp openstack/*.rss $dest > /dev/null