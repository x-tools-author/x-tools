@echo off

:: %1 CMAKE_PREFIX_PATH
:: %2 CMAKE_MAKE_PROGRAM

echo "Do not use this script unless you know what you are doing."
echo "You must run this script from the root directory of xTools."

if "%1"=="" (
    if exist "D:/512/Qt/Qt/6.10.3/msvc2022_64" (
        set "PREFIX_PATH=D:/512/Qt/Qt/6.10.3/msvc2022_64"
    ) else (
        set "PREFIX_PATH=D:/Qt/Qt/6.10.3/msvc2022_64"
    )
) else (
    set "PREFIX_PATH=%1"
)

if "%2"=="" (
    if exist "D:/512/Qt/Qt/Tools/Ninja/ninja.exe" (
        set "MAKE_PROGRAM=D:/512/Qt/Qt/Tools/Ninja/ninja.exe"
    ) else (
        set "MAKE_PROGRAM=D:/Qt/Qt/Tools/Ninja/ninja.exe"
    )
) else (
    set "MAKE_PROGRAM=%2"
)

echo PREFIX_PATH is: "%PREFIX_PATH%"
echo MAKE_PROGRAM is: "%MAKE_PROGRAM%"

set vs2026c="C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat"
set vs2026e="C:\Program Files\Microsoft Visual Studio\18\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
if exist %vs2026c% (
    call %vs2026c% x64
) else if exist %vs2026e% (
    call %vs2026e% x64
) else (
    echo "Visual Studio Build Tools not found. Please install Visual Studio with C++ workload or Visual Studio Build Tools."
    exit /b 1
)

setlocal

pushd "%~dp0.."
set "ROOT_DIR=%CD%"
popd

echo ROOT_DIR is: "%ROOT_DIR%"

if not exist "%ROOT_DIR%\build" (
    mkdir "%ROOT_DIR%\build"
)
cd /d "%ROOT_DIR%\build"

if exist "msix" (
    rmdir /s /q "msix"
)

mkdir "msix"
cd /d "msix"

cmake -S "%ROOT_DIR%" ^
 -G Ninja ^
 -DCMAKE_PREFIX_PATH:PATH="%PREFIX_PATH%" ^
 -DCMAKE_BUILD_TYPE:STRING=Release ^
 -DCMAKE_MAKE_PROGRAM:FILEPATH="%MAKE_PROGRAM%" 

cmake --build . --target all
cmake --build . --target xTools_msix

endlocal