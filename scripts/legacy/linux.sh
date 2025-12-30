#!/bin/bash

# if build is exist, remove it
if [ -d "build" ]; then
  rm -rf build
fi

git fetch --tags
mkdir build
cd build
cmake ../../ -DCMAKE_PREFIX_PATH='/home/user/Qt/6.8.2/gcc_64' -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja"
cmake --build . --target xTools
cmake --build . --target xTools_deb
cmake --build . --target xTools_appimage