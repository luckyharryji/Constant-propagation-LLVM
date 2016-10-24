#!/bin/bash

for i in `find . -name CatPass.cpp` ; do
  echo $i;
  dirName=`dirname $i` ;

  pushd ./
  cd $dirName ;
  make clean ; 
  popd ;
done
