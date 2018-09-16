#!/bin/sh

set -e 

echo "Installing Dependencies..."

sudo yum install -y \
    git \
    gcc cmake \
    json-c json-c-devel \
    jansson jansson-devel \
    libmicrohttpd-devel \
    libcurl-devel \
    systemd-devel

sudo ldconfig

cd ~

sudo echo "/usr/local/lib" >> /etc/ld.so.conf
sudo ldconfig -v

if [ -d ulfius ]; then
    rm -rf ulfius
fi

git clone git@github.com:babelouest/ulfius.git

cd ulfius

git submodule update --init

cd lib/orcania
make && sudo make install
cd ../yder
make && sudo make install
cd ../..
make
sudo make install

exit 0
