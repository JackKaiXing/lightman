echo starting...

rmdir /s /q build
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -DLIGHTMAN_OPENGL=ON 
cmake --build . --config Debug 