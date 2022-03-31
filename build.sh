#! /bin/sh

rm -rf build
mkdir build
cd build
cmake .. -GXcode -DLIGHTMAN_OPENGL=ON
cmake --build . --config Debug 