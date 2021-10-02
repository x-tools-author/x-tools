qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_TCP_SERVER
}

contains(DEFINES, SAK_IMPORT_MODULE_TCP_SERVER){
FORMS += \
    $$PWD/SAKTcpServerController.ui

HEADERS += \
    $$PWD/SAKTcpServerController.hh \
    $$PWD/SAKTcpServerDebugger.hh \
    $$PWD/SAKTcpServerDevice.hh

SOURCES += \
    $$PWD/SAKTcpServerController.cc \
    $$PWD/SAKTcpServerDebugger.cc \
    $$PWD/SAKTcpServerDevice.cc

INCLUDEPATH += \
    $$PWD
}
