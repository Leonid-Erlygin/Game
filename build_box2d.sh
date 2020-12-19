#!/bin/bash
unzip box2d_packet.zip
cd box2d-1025f9a10949b963d6311995910bdd04f72dae6c
./build.sh
cp -r include ../
cp build/src/libbox2d.a ../
cd ..
rm -r box2d-1025f9a10949b963d6311995910bdd04f72dae6c

