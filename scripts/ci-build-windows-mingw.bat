cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64" -DCMAKE_BUILD_TYPE:STRING=Release -G "MinGW Makefiles"
cmake --build . --parallel 4 --target xTools 
cmake --build . --target xTools_zip