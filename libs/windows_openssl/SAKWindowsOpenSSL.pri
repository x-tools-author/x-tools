contains(CONFIG, static){
    # Nothing to do if compiling with static edition.
}else{
    CONFIG(debug, debug|release){
        DLL_TARGET_DIRECTORY = $${OUT_PWD}/debug
    } else {
        DLL_TARGET_DIRECTORY = $${OUT_PWD}/release
    }

    # -------------------------------------------------------------------------
    # OpenSSL 1.0 for Qt.
    equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 12) {
        contains(QT_ARCH, i386) {
            DLL_FILES += $$PWD/x86/ssleay32.dll $$PWD/x86/libeay32.dll
        } else {
            DLL_FILES +=
        }
    }

    # -------------------------------------------------------------------------
    # OpenSSL 1.1 for Qt.
    # They(Qt5.12 5.13 5.14 5.15) use openssl 1.1.
    OPENSSL_1_1_FOR_QT5_MINOR_VERSION_LIST = 12 13 14 15
    OPENSSL_1_1_FOR_QT6_MINOR_VERSION_LIST = 0

    # The value of QT_ARCH can be: i386, x86_64...
    contains(QT_ARCH, i386) {
        # For Qt5.
        equals(QT_MAJOR_VERSION, 5):contains(OPENSSL_1_1_FOR_QT5_MINOR_VERSION_LIST, $$QT_MINOR_VERSION) {
            DLL_FILES += $$PWD/x86/libssl-1_1.dll $$PWD/x86/libcrypto-1_1.dll
        }
        # For Qt6.
        equals(QT_MAJOR_VERSION, 6):contains(OPENSSL_1_1_FOR_QT6_MINOR_VERSION, $$QT_MINOR_VERSION) {
            DLL_FILES += $$PWD/x86/libssl-1_1.dll $$PWD/x86/libcrypto-1_1.dll
        }
    } else {
        equals(QT_MAJOR_VERSION, 5):contains(OPENSSL_1_1_FOR_QT5_MINOR_VERSION, $$QT_MINOR_VERSION) {
            DLL_FILES += $$PWD/x64/libssl-1_1-x64.dll $$PWD/x64/libcrypto-1_1-x64.dll
        }

        equals(QT_MAJOR_VERSION, 6):contains(OPENSSL_1_1_FOR_QT6_MINOR_VERSION, $$QT_MINOR_VERSION) {
            DLL_FILES += $$PWD/x64/libssl-1_1-x64.dll $$PWD/x64/libcrypto-1_1-x64.dll
        }
    }

    # -------------------------------------------------------------------------
    # Copy the dlls to applicaation directory.
    for (DLL_FILE, DLL_FILES) {
        msvc {
            QMAKE_POST_LINK+='$$escape_expand(\\n) xcopy /y $$replace(DLL_FILE, "/", "\\") $$replace(DLL_TARGET_DIRECTORY, "/", "\\") $$escape_expand(\\n)'
        } else {
            QMAKE_POST_LINK+='$$escape_expand(\\n\\t) xcopy /y $$replace(DLL_FILE, "/", "\\") $$replace(DLL_TARGET_DIRECTORY, "/", "\\") $$escape_expand(\\n\\t)'
        }
    }
}
