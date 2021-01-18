equals(QT_MAJOR_VERSION, 5)|equals(QT_MINOR_VERSION, 12) {
    exists($$PWD/../libs/android_openssl/openssl.pri) {
        include($$PWD/../libs/android_openssl/openssl.pri)
    } else {
        message("You should execute the command line: git submodule update --init")
    }

    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/5.12/armeabi-v7a

        DISTFILES += \
            $$PWD/5.12/armeabi-v7a/AndroidManifest.xml \
            $$PWD/5.12/armeabi-v7a/build.gradle \
            $$PWD/5.12/armeabi-v7a/gradle/wrapper/gradle-wrapper.jar \
            $$PWD/5.12/armeabi-v7a/gradle/wrapper/gradle-wrapper.properties \
            $$PWD/5.12/armeabi-v7a/gradlew \
            $$PWD/5.12/armeabi-v7a/gradlew.bat \
            $$PWD/5.12/armeabi-v7a/res/values/libs.xml
    }

    contains(ANDROID_TARGET_ARCH,x86) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/5.12/x86

        DISTFILES += \
            $$PWD/5.12/x86/AndroidManifest.xml \
            $$PWD/5.12/x86/build.gradle \
            $$PWD/5.12/x86/gradle/wrapper/gradle-wrapper.jar \
            $$PWD/5.12/x86/gradle/wrapper/gradle-wrapper.properties \
            $$PWD/5.12/x86/gradlew \
            $$PWD/5.12/x86/gradlew.bat \
            $$PWD/5.12/x86/res/values/libs.xml
    }



    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        ANDROID_PACKAGE_SOURCE_DIR = \
            $$PWD/5.12/arm64-v8a

        DISTFILES += \
            $$PWD/5.12/arm64-v8a/AndroidManifest.xml \
            $$PWD/5.12/arm64-v8a/build.gradle \
            $$PWD/5.12/arm64-v8a/gradle/wrapper/gradle-wrapper.jar \
            $$PWD/5.12/arm64-v8a/gradle/wrapper/gradle-wrapper.properties \
            $$PWD/5.12/arm64-v8a/gradlew \
            $$PWD/5.12/arm64-v8a/gradlew.bat \
            $$PWD/5.12/arm64-v8a/res/values/libs.xml
    }
}
