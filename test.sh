#!/bin/bash -e

cd build
cmake -DBUILD_TESTING=ON .. > /dev/null
make all > /dev/null
./test/integration_tests
