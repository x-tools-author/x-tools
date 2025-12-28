@echo off

::setlocal是 Windows 批处理里的“环境变量作用域开关”。
setlocal

rem 脚本所在目录（带尾部反斜杠）
set "SCRIPT_DIR=%~dp0"

rem 项目根目录：脚本在 scripts\ 下，根目录是上一级
pushd "%SCRIPT_DIR%.."
set "ROOT_DIR=%CD%"
popd

cmake -S "%ROOT_DIR%" ^
 -DCMAKE_PREFIX_PATH:PATH="%ROOT_DIR%\tools\Qt5.6.3-Windows-x86-MinGW4.9.4-20200104" ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DX_SETUP_BUILDING_FOR_WIN_XP_ENV:BOOL=ON ^
 -G "MinGW Makefiles"

rem MinGW 4.9.2 根目录和 bin 目录
set "MINGW_ROOT=%ROOT_DIR%\tools\Tools\mingw492_32"
set "MINGW_BIN=%MINGW_ROOT%\bin"

if not exist "%MINGW_BIN%\gcc.exe" (
    echo [ERROR] gcc.exe not found in "%MINGW_BIN%".
    echo Please check that MinGW 4.9.2 is extracted to tools\Tools\mingw492_32.
    goto :EOF
)

rem 确保 CMake 能从 PATH 找到 gcc/g++/mingw32-make
set "PATH=%MINGW_BIN%;%PATH%"

if not exist "%ROOT_DIR%\build" (
    mkdir "%ROOT_DIR%\build"
)
cd /d "%ROOT_DIR%\build"

if not exist "win_xp" (
    mkdir "win_xp"
)
cd /d "win_xp"

cmake -S "%ROOT_DIR%" ^
 -DCMAKE_PREFIX_PATH:PATH="%ROOT_DIR%\tools\Qt5.6.3-Windows-x86-MinGW4.9.4-20200104" ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DX_SETUP_BUILDING_FOR_WIN_XP_ENV:BOOL=ON ^
 -G "MinGW Makefiles"
cmake --build . --target all
cmake --build . --target xTools_zip

endlocal