#!/bin/bash

# Find the destination of a link in only shell
# $1 is the name of the file to check (including path if required)

ls -l $1 | tr -s " " | sed 's/^.*[0-9] //' | sed 's/^.*-> //'
