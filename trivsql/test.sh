#!/bin/bash

function verify (){
    touch testout/$2
    echo "    - \"$1\""
    echo "$1" | ./sample > testout/$2.new
    if [ `diff testout/$2 testout/$2.new | wc -l | tr -d " "` -gt 0 ]
      then
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
echo " -- Create table"
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
echo " -- This should return all three rows with all three columns"
verify "SELECT cola, colb, colc FROM foo;" sel001

echo ""
echo " -- This should return all three rows with all three columns"
verify "SELECT * FROM foo;" sel002

echo ""
echo " -- This should return three rows with only two columns"
verify "SELECT cola, colc FROM foo;" sel003

echo ""
echo " -- This should return two rows, both with cola = duck"
verify "SELECT cola, colb, colc FROM foo WHERE cola = 'duck';" sel004
