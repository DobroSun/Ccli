#!/bin/bash -e

cd build;
cmake -DBUILD_TESTING=ON ..
sudo make all -j4
./test/run_tests
