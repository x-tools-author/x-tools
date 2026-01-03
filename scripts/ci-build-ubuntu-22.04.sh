# The workspace is the root of the repository
# The script is run from the root of the repository
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH='/opt/qt/6.8.3/gcc_64' \
 -DCMAKE_BUILD_TYPE:STRING=Release \
 -DX_VERSION_OVERRIDE:STRING=$1 \
 -DX_UBUNTU_VERSION:STRING=22.04 \
 -DX_USING_PROJECT_DEPLOY_TOOL:BOOL=ON \
 -G "Ninja" ../
ls -l
cmake --build . --target all
cmake --build . --target xTools_linux