#!/bin/sh

echo "Installing Dependencies..."

sudo yum install -y \
    git \
    gcc cmake
    json-c json-c-devel \
    jansson jansson-devel \
    libmicrohttpd-devel \
    libcurl-devel \

sudo ldconfig

cd ~

git clone git@github.com:babelouest/ulfius.git

cd ulfius

mkdir build
cd build
cmake .. 
make && sudo make install

exit 0
