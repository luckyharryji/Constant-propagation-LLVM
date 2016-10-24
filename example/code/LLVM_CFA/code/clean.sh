#!/bin/bash

dirs=`find . -name Makefile`;

for i in $dirs ; do
  dirName=`dirname $i`  ;
  if test "$dirName" != "." ; then
    pushd ./ ;
    cd $dirName ; 
    make clean ;
    popd ;
  fi
done
