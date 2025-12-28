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

if not exist "win_xp" (
    mkdir "win_xp"
)
cd /d "win_xp"

set "PATH=%ROOT_DIR%\tools\Qt5.6.3\Tools\mingw492_32\bin;%PATH%"

:: Download and install Qt 5.6.3 if not exists
cmake -S "%ROOT_DIR%" ^
 -DCMAKE_PREFIX_PATH:PATH="%ROOT_DIR%\tools\Qt5.6.3\5.6.3\mingw49_32" ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DX_SETUP_BUILDING_FOR_WIN_XP_ENV:BOOL=ON ^
 -G "MinGW Makefiles"

cmake --build . --target all
cmake --build . --target xTools_zip

endlocal