#!/bin/bash

function verify (){
    touch testout/$2
    echo -n "    - \"$1\" "
    echo "$1" | ./sample > testout/$2.new
    if [ `diff testout/$2 testout/$2.new | wc -l | tr -d " "` -gt 0 ]
      then
      echo ""
      echo "    - Results have changed!"
      echo "    - Old:"
      cat testout/$2
      echo ""
      echo "    - New:"
      cat testout/$2.new
    else
      echo "[Repeated results supressed]"
    fi
    mv testout/$2.new testout/$2
}

###################################################################

echo "Removing old db"
rm foo.tdb

echo "Creating db"
verify "CREATE TABLE foo (cola, colb, colc);" create001
verify "CREATE TABLE hyphen-hyphen (hyphen-col);" create002
verify "CREATE TABLE mIxEdCaSe (cola);" create003
verify "CREATE TABLE singlecol ('foo');" create004
verify "CREATE TABLE cepConfig (version);" create005

if [ "%$1%" = "%create%" ]
then
  exit
fi

echo ""
echo "Insert into table"
verify "INSERT INTO foo (cola, colb, colc) VALUES ('duck', 'chicken', 'frog');" insert001
verify "INSERT INTO foo (cola, colb) VALUES ('duck', 'hamster');" insert002
verify "INSERT INTO foo (cola, colc) VALUES ('banana', 'frog');" insert003
verify "INSERT INTO banana (cola) VALUES ('chicken');" insert004
verify "INSERT INTO foo (cola) VALUES ('this-has-hyphens');" insert005
verify "INSERT INTO hyphen-hyphen (hyphen-col) VALUES ('567');" insert006
verify "INSERT INTO mIxEdCaSe (cola) VALUES ('567');" insert007
verify "INSERT INTO cepConfig (mainwindowsizex) VALUES ('224');" insert008

if [ "%$1%" = "%insert%" ]
then
  exit
fi

echo ""
echo "Select tests"
verify "SELECT cola, colb, colc FROM foo;" sel001
verify "SELECT * FROM foo;" sel002
verify "SELECT cola, colc FROM foo;" sel003
verify "SELECT cola, colb, colc FROM foo WHERE cola = 'duck';" sel004
verify "SELECT * FROM nosuchtable;" sel005

echo ""
echo "Alter tests"
verify "ALTER foo ADD COLUMN cold;" alter001
verify "SELECT * FROM foo;" alter002
verify "INSERT INTO foo (cola, cold) VALUES('duck', 'gerkin');" alter003
verify "SELECT * FROM foo;" alter002
