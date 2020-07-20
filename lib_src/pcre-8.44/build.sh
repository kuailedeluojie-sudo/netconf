export CC=arm-linux-gcc 
export CXX=arm-linux-g++
./configure --host=arm --prefix=$PWD/_install  --enable-unicode-properties --enable-utf8
