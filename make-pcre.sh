#!/bin/sh
export CC=arm-linux-gcc
export CXX=arm-linux-g++
#export CC=arm-linux-gnueabihf-gcc
#export CXX=arm-linux-gnueabihf-g++
cd lib_src/pcre
mkdir -p buid
cd buid
rm -rf *
cmake .. -DCMAKE_INSTALL_PREFIX=/home/mo/share/netconf2/lib_src/pcre/_install  --enable-unicode-properties
make
make install
