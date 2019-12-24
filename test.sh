#!/bin/bash -e

cd build
sudo cmake -DBUILD_TESTING=ON -DDEFINE_DEBUG=OFF ..
sudo make all
./tests/global_tests
