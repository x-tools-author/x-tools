cmake ../ -DCMAKE_PREFIX_PATH='/opt/qt' -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja"
cmake --build . --target xTools
cmake --build . --target xTools_linux