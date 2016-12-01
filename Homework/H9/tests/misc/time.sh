#!/bin/bash

inputFile=$1 ;
outputFile=$2 ;
LLVMPASSPATH=$3 ;
nums=$4 ;

totalTime="0" ;
let count=0 ;
for i in `seq 1 $nums` ; do
  opt -load ${LLVMPASSPATH} -basicaa -CAT -time-passes $inputFile -o $outputFile 2> tmpOut

  time=`grep CAT tmpOut | awk '{print $1}'` ;
  rm tmpOut ;

  totalTime=`echo "$totalTime + $time" | bc` ;
  let count=$count+1 ;
done
echo "" ;

averageTime=`echo "scale=3; $totalTime / $count" | bc` ;
clang $outputFile -lm -o binary ;
./binary &> tmpOut ;
echo `grep "CAT invocations" tmpOut` ;
echo "Time: $averageTime seconds" ;

rm tmpOut binary ;
