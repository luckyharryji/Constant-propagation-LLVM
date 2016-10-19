#!/bin/bash

if ! test -f compiler_output ; then
  echo "Test failed" ; 
  echo "File \"compiler_output\" is missing" ;
  exit 1;
fi
if ! test -f output/oracle_output ; then
  echo "Test failed" ; 
  echo "File \"output/oracle_output\" is missing" ;
  exit 1;
fi

awk '{
    if (  ($1 != "warning:")    &&
          ($2 != "warning")     ){
      printf("%s\n", $0);
    }
  }' compiler_output  > tmp_output ;
mv tmp_output compiler_output ;

outputs=`ls output`;
for i in $outputs ; do
  diffOut=`diff compiler_output output/$i` ;
  if test "$diffOut" == "" ; then 
    echo "Test passed!" ; 
    exit 0;
  fi
  echo "$diffOut" > diff/${i}_diff_output ;
done

echo "Test failed" ; 
echo "Output differences can be found in \"diff\"" ;
