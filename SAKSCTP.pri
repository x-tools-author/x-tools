# sctp模块是从qt5.8开始引入的
greaterThan(QT_MAJOR_VERSION, 4){
    greaterThan(QT_MINOR_VERSION, 7){
        qtHaveModule(network){
            DEFINES+=SAK_IMPORT_SCTP_MODULE
        }
    }
}

contains(DEFINES, SAK_IMPORT_SCTP_MODULE){
    INCLUDEPATH += \
        $$PWD/src/pages/sctp/client \
        $$PWD/src/pages/sctp/server

    FORMS += \
        $$PWD/src/pages/sctp/client/SAKSctpClientDeviceController.ui \
        $$PWD/src/pages/sctp/server/SAKSctpServerDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/sctp/client/SAKSctpClientDebugPage.hh \
        $$PWD/src/pages/sctp/client/SAKSctpClientDevice.hh \
        $$PWD/src/pages/sctp/client/SAKSctpClientDeviceController.hh \
        $$PWD/src/pages/sctp/server/SAKSctpServerDebugPage.hh \
        $$PWD/src/pages/sctp/server/SAKSctpServerDevice.hh \
        $$PWD/src/pages/sctp/server/SAKSctpServerDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/sctp/client/SAKSctpClientDebugPage.cc \
        $$PWD/src/pages/sctp/client/SAKSctpClientDevice.cc \
        $$PWD/src/pages/sctp/client/SAKSctpClientDeviceController.cc \
        $$PWD/src/pages/sctp/server/SAKSctpServerDebugPage.cc \
        $$PWD/src/pages/sctp/server/SAKSctpServerDevice.cc \
        $$PWD/src/pages/sctp/server/SAKSctpServerDeviceController.cc
}else {
    message("SCTP debugging is not supported by the Qt edition.")
}
