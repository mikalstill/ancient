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
echo "CREATE TABLE foo (cola, colb, colc);" | ./sample

echo " -- Insert into table"
echo "INSERT INTO foo (cola, colb, colc) VALUES ('duck', 'chicken', 'frog');" | ./sample
echo "INSERT INTO foo (cola, colb) VALUES ('duck', 'hamster');" | ./sample
echo "INSERT INTO foo (cola, colc) VALUES ('banana', 'frog');" | ./sample

echo ""
echo "Select tests"
echo " -- This should return all three rows with all three columns"
verify "SELECT cola, colb, colc FROM foo;" sel001

echo ""
echo " -- This should return all three rows with all three columns"
verify "SELECT * FROM foo;" sel002
