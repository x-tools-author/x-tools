win32|unix {
    exists($${PWD}/../../../3rd/libqrencode/qrencode.h){
        DEFINES += SAK_IMPORT_MODULE_QRCODE
        DEFINES += HAVE_CONFIG_H
    }
}

# libqrencode 源文件
contains(DEFINES, SAK_IMPORT_MODULE_QRCODE){
    INCLUDEPATH += \
        3rd/libqrencode

    HEADERS += \
        3rd/libqrencode/bitstream.h \
        3rd/libqrencode/mask.h \
        3rd/libqrencode/mmask.h \
        3rd/libqrencode/mqrspec.h \
        3rd/libqrencode/qrencode.h \
        3rd/libqrencode/qrencode_inner.h \
        3rd/libqrencode/qrinput.h \
        3rd/libqrencode/qrspec.h \
        3rd/libqrencode/rsecc.h \
        3rd/libqrencode/split.h

    SOURCES += \
        3rd/libqrencode/bitstream.c \
        3rd/libqrencode/mask.c \
        3rd/libqrencode/mmask.c \
        3rd/libqrencode/mqrspec.c \
        3rd/libqrencode/qrencode.c \
        3rd/libqrencode/qrinput.c \
        3rd/libqrencode/qrspec.c \
        3rd/libqrencode/rsecc.c \
        3rd/libqrencode/split.c
}

contains(DEFINES, SAK_IMPORT_MODULE_QRCODE){
    FORMS += \
    $$PWD/src/SAKToolQRCodeCreator.ui

    HEADERS += \
        $$PWD/src/SAKQRCode.hh \
    $$PWD/src/SAKToolQRCodeCreator.hh

    SOURCES += \
        $$PWD/src/SAKQRCode.cc \
    $$PWD/src/SAKToolQRCodeCreator.cc

    HEADERS += \
        $$PWD/src/config.h

    INCLUDEPATH += \
        $$PWD/src
}else {
    message("The QRCode module is not supported by the platform, the program will not has QRCode-Creator.")
}
