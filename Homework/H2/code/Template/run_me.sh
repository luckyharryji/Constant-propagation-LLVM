#!/bin/bash

# Set the CLANG compilers to use if unset
if test -z "$CC" ; then
  export CC=clang-3.8
  export CXX=clang++-3.8
fi

rm -rf build/ ; 
mkdir build ; 
cd build ; 
cmake -DCMAKE_INSTALL_PREFIX="~/CAT" ../ ; 
make ;
make install ;
cd ../ 
