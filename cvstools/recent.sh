#!/bin/bash

find . -type f -mtime -7 -print | grep -v CVS
