#!/bin/bash

# This is evil, but it was better than manually writing all these tests...
# $1: start test number
# $2: end test number
# $3: output filename

count=1

rm cep_testPresentation_functions
rm cep_testPresentation_suite

for dataset in `ls ../../datasets | grep dat1 | sed 's/.dat1$//'`
do
  echo "Processing $dataset"

  for average in yes no
  do
    for errors in yes no
    do
      for x in yes no
      do
        for y in yes no
        do
          for z in yes no
          do
            for view in "-c" "-m"
            do
	      if [ $count -gt $1 ]
              then
                echo -n "*"
  	        cmdline="$dataset -a $average -e $errors -x $x -y $y -z $z $view"
	        func=`echo $cmdline | sed -e 's/ /_/g' -e 's/-//g'`

	        cat <<EOF >> cep_testPresentation_suite
  suiteOfTests->addTest(new CppUnit::TestCaller<Test>("$func", &Test::cpt_$func));
EOF

	        cat <<EOF >> cep_testPresentation_functions
void cpt_$func (){runUI("$cmdline");}
EOF
	      fi

	    count=$(( $count + 1 ))
	    if [ $count -gt $2 ]
	    then
	      cat cep_testPresentation.cpp.in1 > $3
	      cat cep_testPresentation_suite >> $3
	      cat cep_testPresentation.cpp.in2 >> $3
	      cat cep_testPresentation_functions >> $3
	      cat cep_testPresentation.cpp.in3 >> $3
	      echo ""
              exit
	    fi
            done
          done
        done
      done
    done
  done
done

cat cep_testPresentation.cpp.in1 > $3
cat cep_testPresentation_suite >> $3
cat cep_testPresentation.cpp.in2 >> $3
cat cep_testPresentation_functions >> $3
cat cep_testPresentation.cpp.in3 >> $3
echo ""