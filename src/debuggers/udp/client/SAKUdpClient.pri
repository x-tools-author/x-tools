qtHaveModule(network){
    QT  += network
    DEFINES+=SAK_IMPORT_MODULE_UDP_CLIENT
}

contains(DEFINES, SAK_IMPORT_MODULE_UDP_CLIENT){
FORMS += \
    $$PWD/SAKUdpClientController.ui

HEADERS += \
    $$PWD/SAKUdpClientController.hh \
    $$PWD/SAKUdpClientDebugger.hh \
    $$PWD/SAKUdpClientDevice.hh

SOURCES += \
    $$PWD/SAKUdpClientController.cc \
    $$PWD/SAKUdpClientDebugger.cc \
    $$PWD/SAKUdpClientDevice.cc \

INCLUDEPATH += \
    $$PWD
}
