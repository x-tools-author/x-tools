# The workspace is the root of the repository
# The script is run from the root of the repository

# args
# $1: The path of android_release.keystore
# $2: ANDROID_KEYSTORE_ALIAS
# $3: ANDROID_KEYSTORE_STORE_PASS
# $4: ANDROID_KEYSTORE_KEY_PASS

tree -L 3 /opt/qt
cmake -DCMAKE_PREFIX_PATH='/opt/qt/6.8.3/android_armv7' -DCMAKE_BUILD_TYPE:STRING=Release -G "Ninja"
cmake --build . --target xTools