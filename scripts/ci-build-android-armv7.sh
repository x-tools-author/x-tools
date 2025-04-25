# The workspace is the root of the repository
# The script is run from the root of the repository

# args
# $1: The path of android_release.keystore
# $2: ANDROID_KEYSTORE_ALIAS
# $3: ANDROID_KEYSTORE_STORE_PASS
# $4: ANDROID_KEYSTORE_KEY_PASS

tree -L 3 /opt/qt
chmod +x /opt/qt/6.8.3/android_armv7/bin/qt-cmake
/opt/qt/6.8.3/android_armv7/bin/qt-cmake \
    -DCMAKE_BUILD_TYPE:STRING=Release -G "Unix Makefiles" \
    -S . \
    -B build/android_armv7 \
    -DANDROID_ABI=armeabi-v7a \
    -DQT_HOST_PATH:PATH="/opt/qt/6.8.3/gcc_64" \
    -DCMAKE_CXX_COMPILER:FILEPATH="$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++" \
    -DCMAKE_BUILD_TYPE:STRING=Release

if [ $? -ne 0 ]; then
    echo "CMake configuration failed"
    exit 1
fi

cmake --build build/armeabi_v7a --target all --config Release

if [ $? -ne 0 ]; then
    echo "CMake build failed"
    exit 1
fi

chomod +x /opt/qt/6.8.3/gcc_64/bin/androiddeployqt
/opt/qt/6.8.3/gcc_64/bin/androiddeployqt \
    --input build/armeabi_v7a/android-xTools-deployment-settings.json \
    --output build/armeabi_v7a/android-build \
    --android-platform android-35 \
    --sign $1 $2 --storepass $3 --keypass $4

if [ $? -ne 0 ]; then
    echo "Android deployment failed"
    exit 1
fi