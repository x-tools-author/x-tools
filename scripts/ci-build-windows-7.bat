@echo off
setlocal

rem 脚本所在目录的上一级
pushd "%~dp0.."
set "ROOT_DIR=%CD%"
popd

echo ROOT_DIR is: "%ROOT_DIR%"

if not exist "%ROOT_DIR%\build" (
    mkdir "%ROOT_DIR%\build"
)
cd /d "%ROOT_DIR%\build"

if not exist "win_7" (
    mkdir "win_7"
)
cd /d "win_7"

set "PATH=%ROOT_DIR%\tools\MinGW810_32\bin;%PATH%"

:: Download and install Qt 5.15.14 if not exists
echo Building...

cmake -S "%ROOT_DIR%" ^
 -DCMAKE_PREFIX_PATH:PATH="%ROOT_DIR%\tools\Qt5.15.14" ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DX_VERSION_OVERRIDE:STRING=%1 ^
 -DX_SETUP_BUILDING_FOR_WIN_7_ENV:BOOL=ON ^
 -G "MinGW Makefiles"

cmake --build . --target all
cmake --build . --target xTools_zip
pause()

endlocal