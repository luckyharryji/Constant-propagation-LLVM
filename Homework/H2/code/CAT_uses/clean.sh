#!/bin/bash

for i in `ls` ; do
  if ! test -d $i ; then
    continue ;
  fi
  cd $i ; 
  make clean ;
  cd ../ ;
done
