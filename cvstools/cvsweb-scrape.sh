#!/bin/bash

lynx http://$1/cgi-bin/cvsweb --dump | grep "\[DIR\]" | sed 's/\[[0-9]*\]//'
