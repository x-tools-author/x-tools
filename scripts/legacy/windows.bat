:: if build is exist, remove it
if exist build rmdir /s /q build

mkdir build
cd build
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -DCMAKE_PREFIX_PATH='D:/512/Qt/Qt/6.8.2/msvc2022_64' -DCMAKE_BUILD_TYPE=Release -G "Ninja" ../../
cmake --build . --target xTools
cmake --build . --target xTools_zip
dir ..\..\bin\Windows\Release
for %%f in (..\..\bin\Windows\Release\xTools-zip\*.zip) do echo "zip file: %%f"