#!/bin/bash

rm -rf src ;
git clone ../../../../src/Template src ;
cp *.cpp src/catpass/ ;

cd src ;
./run_me.sh ;
