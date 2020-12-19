#!/bin/bash

sudo apt-get purge cmake

sudo apt-get install libssl-dev
sudo apt-get install xorg-dev libglu1-mesa-dev

version=3.16
build=3
mkdir ~/temp
cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz
cd cmake-$version.$build/

./bootstrap
make -j4
sudo make install