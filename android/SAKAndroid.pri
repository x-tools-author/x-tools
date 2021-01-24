NO_OPENSSL_PRI_MESSAGE = "You should execute the command line: git submodule update --init"
#------------------------------------------------------------------------------
# The configuration is for Qt5.12-Qt5.14(MINI API is 16(Android 4.1))
# Know more about the tools and version, you can see:https://wiki.qt.io/Qt_5.12_Tools_and_Versions
SAK_ANDROID_CONFIRUEATION_FRO_QT5_MINOR_VERSION_LIST = 12 13 14
equals(QT_MAJOR_VERSION, 5):contains(SAK_ANDROID_CONFIRUEATION_FRO_QT5_MINOR_VERSION_LIST, $$QT_MINOR_VERSION) {
    exists($$PWD/../libs/android_openssl/openssl.pri) {
        include($$PWD/../libs/android_openssl/openssl.pri)
    } else {
        message($$NO_OPENSSL_PRI_MESSAGE)
    }

    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/5.12/armeabi-v7a
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
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/5.12/x86
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
        ANDROID_PACKAGE_SOURCE_DIR = $$PWD/5.12/arm64-v8a
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

#------------------------------------------------------------------------------
# The configuration is for Qt5.15(MINI API is 21(Android 5.0))
# Know more about the tools and version, you can see:https://wiki.qt.io/Qt_5.15_Tools_and_Versions
equals(QT_MAJOR_VERSION, 5):equals(QT_MINOR_VERSION, 15) {
    exists($$PWD/../libs/android_openssl/openssl.pri) {
        include($$PWD/../libs/android_openssl/openssl.pri)
    } else {
        message($$NO_OPENSSL_PRI_MESSAGE)
    }

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/5.15
    DISTFILES += \
        $$PWD/5.15/AndroidManifest.xml \
        $$PWD/5.15/build.gradle \
        $$PWD/5.15/gradle.properties \
        $$PWD/5.15/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/5.15/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/5.15/gradlew \
        $$PWD/5.15/gradlew.bat \
        $$PWD/5.15/res/values/libs.xml
}

#------------------------------------------------------------------------------
# The configuration is for Qt6.0-Qt6.0(MINI API is 21(Android 5.0))
# Know more about the tools and version, you can see:https://wiki.qt.io/Qt_6.0_Tools_and_Versions
SAK_ANDROID_CONFIRUEATION_FRO_QT6_MINOR_VERSION_LIST = 0 1  2 3 4 5 6 7 8 9 10 11 12 13 14 15
equals(QT_MAJOR_VERSION, 6):contains(SAK_ANDROID_CONFIRUEATION_FRO_QT6_MINOR_VERSION_LIST, $$QT_MINOR_VERSION) {
    exists($$PWD/../libs/android_openssl/openssl.pri) {
        include($$PWD/../libs/android_openssl/openssl.pri)
    } else {
        message($$NO_OPENSSL_PRI_MESSAGE)
    }

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/5.15
    DISTFILES += \
        $$PWD/5.15/AndroidManifest.xml \
        $$PWD/5.15/build.gradle \
        $$PWD/5.15/gradle.properties \
        $$PWD/5.15/gradle/wrapper/gradle-wrapper.jar \
        $$PWD/5.15/gradle/wrapper/gradle-wrapper.properties \
        $$PWD/5.15/gradlew \
        $$PWD/5.15/gradlew.bat \
        $$PWD/5.15/res/values/libs.xml
}
