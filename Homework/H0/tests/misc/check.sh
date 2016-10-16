#!/bin/bash

allInputs=( $@ ) ;

let allInputsLen=${#allInputs[@]}-1 ;
for i in `seq 0 2 $allInputsLen` ; do
  fileName=${allInputs[$i]} ;
  outputDir=${allInputs[$i+1]} ;
  echo "Checking $fileName against oracles stored in $outputDir" ;

  if ! test -f $fileName ; then
    echo "  Test failed" ; 
    echo "  File \"$fileName\" is missing" ;
    exit 1;
  fi

  let success=0; 
  outputs=`ls $outputDir`;
  for i in $outputs ; do
    diffOut=`diff $fileName $outputDir/$i` ;
    if test "$diffOut" == "" ; then 
      let success=1 ;
      break ;
    fi
    echo "$diffOut" > diff/${i}_diff_output ;
  done
  if test "$success" == "0" ; then
    echo "  Test failed because output $fileName isn't correct" ; 
    echo "  Output differences can be found in \"diff\"" ;
    exit 1;
  fi
done
echo "Test passed!" ;
