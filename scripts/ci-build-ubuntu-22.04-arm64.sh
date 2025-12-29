# The workspace is the root of the repository
# The script is run from the root of the repository
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH='/opt/qt/6.7.3/gcc_arm64' -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja" ../
ls -l
cmake --build . --target all
cmake --build . --target xTools_linux