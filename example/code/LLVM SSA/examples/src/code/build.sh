#!/bin/bash

for i in `find . -name Makefile` ; do
  pushd ./ ;
  cd $i ; 
  make clean ;
  popd ;
done
