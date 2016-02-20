#!/bin/bash -vx

set -e

mkdir /build/

cd /build

cmake /source

make

#./test/tests

cp src/main /main
