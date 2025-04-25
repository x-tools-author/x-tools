# The workspace is the root of the repository
# The script is run from the root of the repository
cmake -DCMAKE_PREFIX_PATH='/opt/qt/6.8.3/android_armv7' -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja"
cmake --build . --target xTools