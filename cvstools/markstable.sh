#!/bin/bash

cd $1
echo "This project is currently in a stable state, and unlikely to change in the near future" > STABLE
cvs add STABLE
cd ..
