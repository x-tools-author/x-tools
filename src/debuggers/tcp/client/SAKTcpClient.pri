qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_TCP_CLIENT
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP_CLIENT){
FORMS += \
    $$PWD/SAKTcpClientController.ui

HEADERS += \
    $$PWD/SAKTcpClientController.hh \
    $$PWD/SAKTcpClientDebugger.hh \
    $$PWD/SAKTcpClientDevice.hh

SOURCES += \
    $$PWD/SAKTcpClientController.cc \
    $$PWD/SAKTcpClientDebugger.cc \
    $$PWD/SAKTcpClientDevice.cc

INCLUDEPATH += \
    $$PWD
}
