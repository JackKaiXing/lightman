#! /bin/sh

rm -rf build
mkdir build
cd build
cmake .. -GXcode
cmake --build . --config Debug 