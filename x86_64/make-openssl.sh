#!/bin/sh
#export CC=arm-linux-gcc
#export CXX=arm-linux-g++
#export CC=arm-linux-gnueabihf-gcc
#export CXX=arm-linux-gnueabihf-g++
cd ../lib/openssl
mkdir -p debug
cd debug
rm -rf *
cmake .. -DCMAKE_INSTALL_PREFIX=/home/mo/share/netconf2/x86_64/install/openssl/_install

make
make install
