#!/bin/bash

# Select a specified item from a list. Copyright (c) Michael Still 2002
# Released under the terms of the GNU GPL

# $1 is the number to get, $* except for $1 is the list of options, delimited
# by a space each

# We can the shift operation to get to the right number
shift $1
echo $1
