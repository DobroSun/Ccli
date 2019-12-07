#!/bin/bash -e

cd build
cmake -DBUILD_TESTING=ON ..
make all
./test/global_tests
