#!/bin/bash

rm -rf src ;
cp -r ../Template src ;
cp *.cpp src/catpass/ ;

cd src ;
./run_me.sh ;
