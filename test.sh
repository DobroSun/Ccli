#!/bin/bash -e

cd build
cmake -DBUILD_TESTING=ON -DDEFINE_DEBUG=OFF ..
make all
./test/global_tests
