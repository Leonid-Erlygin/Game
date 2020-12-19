#!/bin/bash

cmake -Bcmake-build-debug -H. -DCMAKE_BUILD_TYPE=debug
cd cmake-build-debug
make
