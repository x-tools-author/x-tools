echo "Please run this script in the root directory of the project"
:: if build is exist, remove it
:: if exist build rmdir /s /q build

:: Create build directory and set up environment
cls

set JAVA_HOME=D:\512\Android\Jdk\jdk-11.0.21+9
set ANDROID_NDK_ROOT=D:\512\Android\Sdk\ndk\25.1.8937393
set ANDROID_SDK_ROOT=D:\512\Android\Sdk

if not "%1" == "build" (
  :: Configure for Android
  echo "Configuring for Android..."
  D:\512\Qt\Qt\6.5.3\android_armv7\bin\qt-cmake.bat ^
  -G "MinGW Makefiles" ^
  -S . ^
  -B "build\armeabi_v7a" ^
  -DQT_HOST_PATH:PATH="D:\512\Qt\Qt\6.5.3\mingw_64" ^
  -DCMAKE_BUILD_TYPE:STRING=Release ^
  -DCMAKE_CXX_COMPILER:FILEPATH="%ANDROID_NDK_ROOT%/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe" ^
  -DANDROID_SDK_ROOT:PATH="%ANDROID_SDK_ROOT%" ^
  -DANDROID_NDK_ROOT:PATH="%ANDROID_NDK_ROOT%" ^
  -DBUILD_SHARED_LIBS:BOOL=OFF ^
  -DWITH_TOOLS:BOOL=OFF
)

:: Build for Android
echo "Building for Android..."
cmake --build build\armeabi_v7a --target xTools --config Release
xcopy build\armeabi_v7a\libxTools_armeabi-v7a.so build\armeabi_v7a\libs\libxTools_armeabi-v7a.so /Y

:: Generate APK
set QT_ANDROID_KEYSTORE_PATH=res/android/android_release.keystore
set QT_ANDROID_KEYSTORE_ALIAS=xtools
set QT_ANDROID_KEYSTORE_STORE_PASS=xtools
set QT_ANDROID_KEYSTORE_KEY_PASS=xtools

echo "Generating APK..."
D:\512\Qt\Qt\6.5.3\mingw_64\bin\androiddeployqt.exe ^
--input build\armeabi_v7a\android-xTools-deployment-settings.json ^
--output build\armeabi_v7a\android-build ^
--android-platform android-34
