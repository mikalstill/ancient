#!/bin/bash

/sbin/ifconfig eth0 | grep HWaddr | sed -e 's/^.*HWaddr //' -e 's/ *$//'
