#!/bin/bash

function verify (){
    touch testout/$2
    echo "    - \"$1\""
    echo "$1" | ./sample > testout/$2.new
    if [ `diff testout/$2 testout/$2.new | wc -l | tr -d " "` -gt 0 ]
      then
      echo " -- $1:"
      echo "    - Results have changed!"
      echo "    - Old:"
      cat testout/$2
      echo ""
      echo "    - New:"
      cat testout/$2.new
    else
      echo "    - Repeated results supressed"
    fi
    mv testout/$2.new testout/$2
}

###################################################################

echo "Removing old db"
rm foo.tdb

echo "Creating db"
verify "CREATE TABLE foo (cola, colb, colc);" create001

if [ "%$1%" = "%create%" ]
then
  exit
fi

echo " -- Insert into table"
verify "INSERT INTO foo (cola, colb, colc) VALUES ('duck', 'chicken', 'frog');" insert001
verify "INSERT INTO foo (cola, colb) VALUES ('duck', 'hamster');" insert002
verify "INSERT INTO foo (cola, colc) VALUES ('banana', 'frog');" insert003

if [ "%$1%" = "%insert%" ]
then
  exit
fi

echo ""
echo "Select tests"
verify "SELECT cola, colb, colc FROM foo;" sel001

echo ""
verify "SELECT * FROM foo;" sel002

echo ""
verify "SELECT cola, colc FROM foo;" sel003

echo ""
verify "SELECT cola, colb, colc FROM foo WHERE cola = 'duck';" sel004

echo ""
verify "SELECT * FROM nosuchtable;" sel005
