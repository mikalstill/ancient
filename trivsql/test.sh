#!/bin/bash

function verify (){
    loclearn=0
    if [ "%$3%" = "%2%" ]
    then
      loclearn=1
    elif [ "%$3%" = "%1%" ]
    then
      loclearn=1
    fi

    locdisplay=0
    if [ "%$34%" = "%2%" ]
    then
      locdisplay=1
    elif [ "%$4%" = "%1%" ]
    then
      locdisplay=1
    fi

    touch testout/$2
    echo -n "$2: \"$1\" "
    echo "$1" > testout/$2.new
    echo "" >> testout/$2.new
    echo "$1" | ./sample foo.tdb >> testout/$2.new
    if [ `diff testout/$2 testout/$2.new | wc -l | tr -d " "` -gt 0 ]
    then
      if [ "%$loclearn%" = "%1%" ]
      then
        echo "[NEW RESULT LEARNT]"
	cp testout/$2.new testout/$2
      else
        echo -n "[FAIL"
	touch testout/$2.prev
        if [ `diff testout/$2.prev testout/$2.new | wc -l | tr -d " "` -gt 0 ]
        then
          echo " NEW]"
	  echo ""
	  cat testout/$2.new
	  echo ""
        else
          echo " OLD]"
        fi
      fi
    else
      echo "[PASS]"
    fi

    if [ "%$locdisplay%" = "%1%" ]
    then
      echo ""
      cat testout/$2.new
      echo ""
    fi

    # We remember last time as well
    mv testout/$2.new testout/$2.prev
}

###################################################################

echo "Parsing command line variables"
result=0

learn=0     # Don't learn the output as correct
display=0   # Don't display output

while [ $result -lt 1 ]
do
  getopts "ld" arg 
  result=$?

  if [ $result -lt 1 ]
  then
    case $arg in
	\? )
	echo "Usage: $0 [-l]"
	echo ""
	echo "l: Learn that the current output is \"correct\""
	echo "d: Display the current output"
        echo ""    
	exit 0 ;;
	
	l )
	if [ "%$optarg%" = "%%" ]
        then
          echo "Learning all new values"
          learn=1
        else
          echo "Learning new values for $optarg"
          learn=$optarg
        fi ;;

	d )
	if [ "%$optarg%" = "%%" ]
        then
          echo "Displaying values"
          display=1
        else
          echo "Displaying value for $optarg"
          display=$optarg
        fi ;;
    esac
  fi
done

echo "Removing old db"
rm foo.tdb

echo "Creating db"
verify "CREATE TABLE foo (cola, colb, colc);" create001 $learn $display
verify "CREATE TABLE hyphen-hyphen (hyphen-col);" create002 $learn $display
verify "CREATE TABLE mIxEdCaSe (cola);" create003 $learn $display
verify "CREATE TABLE singlecol ('foo');" create004 $learn $display
verify "CREATE TABLE cepConfig (version);" create005 $learn $display

if [ "%$1%" = "%create%" ]
then
  exit
fi

echo ""
echo "Insert into table"
verify "INSERT INTO foo (cola, colb, colc) VALUES ('duck', 'chicken', 'frog');" insert001 $learn $display
verify "INSERT INTO foo (cola, colb) VALUES ('duck', 'hamster');" insert002 $learn $display
verify "INSERT INTO foo (cola, colc) VALUES ('banana', 'frog');" insert003 $learn $display
verify "INSERT INTO banana (cola) VALUES ('chicken');" insert004 $learn $display
verify "INSERT INTO foo (cola) VALUES ('this-has-hyphens');" insert005 $learn $display
verify "INSERT INTO hyphen-hyphen (hyphen-col) VALUES ('567');" insert006 $learn $display
verify "INSERT INTO mIxEdCaSe (cola) VALUES ('567');" insert007 $learn $display
verify "INSERT INTO cepConfig (mainwindowsizex) VALUES ('224');" insert008 $learn $display

if [ "%$1%" = "%insert%" ]
then
  exit
fi

echo ""
echo "Select tests"
verify "SELECT cola, colb, colc FROM foo;" sel001 $learn $display
verify "SELECT * FROM foo;" sel002 $learn $display
verify "SELECT cola, colc FROM foo;" sel003 $learn $display
verify "SELECT cola, colb, colc FROM foo WHERE cola = 'duck';" sel004 $learn $display
verify "SELECT * FROM nosuchtable;" sel005 $learn $display
verify "SELECT cola FROM foo;" sel006 $learn $display

echo ""
echo "Alter tests"
verify "ALTER foo ADD COLUMN cold;" alter001 $learn $display
verify "SELECT * FROM foo;" alter002 $learn $display
verify "INSERT INTO foo (cola, cold) VALUES('duck', 'gerkin');" alter003 $learn $display
verify "SELECT * FROM foo;" alter004 $learn $display

echo ""
echo "Update tests"
verify "CREATE TABLE upd (one, two);" update001 $learn $display
verify "INSERT INTO upd (one, two) VALUES('a', 'b');" update002 $learn $display
verify "SELECT * FROM upd;" update003 $learn $display
verify "UPDATE upd SET one = 'c';" update004 $learn $display
verify "SELECT * FROM upd;" update005 $learn $display
verify "INSERT INTO upd (one, two) VALUES('e', 'f');" update006 $learn $display
verify "UPDATE upd SET two = 'g' WHERE two = 'f';" update007 $learn $display
verify "SELECT * FROM upd;" update008 $learn $display

echo ""
echo "Selection tree tests"
verify "SELECT * FROM foo WHERE cola = 'duck' AND colb = 'chicken';" seltree001 $learn $display
verify "SELECT * FROM foo WHERE cola = 'duck' OR colb = 'chicken';" seltree002 $learn $display
